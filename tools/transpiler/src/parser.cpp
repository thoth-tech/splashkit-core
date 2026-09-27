#include "parser.h"

#include <clang/Tooling/Tooling.h>
#include <clang/Lex/MacroArgs.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/CompilationDatabase.h>

#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

#include "ast.h"
#include "builder.h"
#include "macro.h"

namespace fs = std::filesystem;

// Full set of ASTs generated
std::vector<CustomAST> allASTs;

std::string getMacroArgumentString(
    const clang::MacroArgs *args,
    unsigned tokenIndex,
    clang::Preprocessor &pp)
{
    const clang::Token *argTokens = args->getUnexpArgument(0);  // Get a pointer to the first token
    std::string result = pp.getSpelling(argTokens[tokenIndex]); // Get the source text for the selected token

    // Strip out the quotes
    if (result.size() >= 2 &&
        result.front() == '"' &&
        result.back() == '"')
    {
        result = result.substr(1, result.size() - 2);
    }

    return result;
}

void addMacroArgs(MacroInfo &macro, const clang::MacroArgs *args)
{
    const clang::Token *tokens = args->getUnexpArgument(0);
    unsigned numTokens = args->getArgLength(tokens);

    // Get the location of the first and last tokens in the unexpanded arguments
    clang::SourceLocation begin = tokens[0].getLocation();
    clang::SourceLocation end = tokens[numTokens - 1].getLocation();
    macro.argumentRange = clang::SourceRange(begin, end);
}

// Takes a set of tags in the form "[tag1][tag2][tag3]"
// Returns each tag as a string in a vector
std::vector<std::string> parseTags(std::string tagString)
{
    std::vector<std::string> result;

    int start = 0;

    // Extract substrings between "[]"
    while ((start = tagString.find('[', start)) != std::string::npos)
    {
        int end = tagString.find(']', start);

        if (end == std::string::npos) break;

        result.push_back(tagString.substr(start + 1, end - start - 1));
        start = end + 1;
    }

    return result;
}

// PPCallbacks is a pre-processor interface. MacroExpands() is called when the pre-processor encounters a Macro
void TestFinder::MacroExpands(
    const clang::Token &macroName,
    const clang::MacroDefinition &macroDefinition,
    clang::SourceRange range,
    const clang::MacroArgs *args)
{
    std::string name = macroName.getIdentifierInfo()->getName().str();

    // Record macro info
    MacroInfo macro;
    macro.location = sourceManager.getExpansionLoc(range.getBegin());
    // Get the raw version of location as a key
    unsigned key = macro.location.getRawEncoding();

    if (name == "TEST_CASE")
    {
        macro.kind = MacroKind::TestCase;
        macro.name = getMacroArgumentString(args, 0, pp);

        // Add tags
        std::string allTagsString = getMacroArgumentString(args, 2, pp);
        macro.tags = parseTags(allTagsString);
        std::cout << "- Test case found at " << macro.location.getRawEncoding() << std::endl;
        currentTestKey = key;
    }
    else if (name == "SECTION")
    {
        macro.kind = MacroKind::Section;
        macro.name = getMacroArgumentString(args, 0, pp);
        std::cout << "- Section found at " << macro.location.getRawEncoding() << std::endl;
    }
    else if (name == "REQUIRE")
    {
        macro.kind = MacroKind::Require;
        addMacroArgs(macro, args);
    }
    else if (name == "REQUIRE_FALSE")
    {
        macro.kind = MacroKind::RequireFalse;
        addMacroArgs(macro, args);
    }
    else if (name == "REQUIRE_THROWS")
    {
        macro.kind = MacroKind::RequireThrows;
        addMacroArgs(macro, args);
    }
    else if (name == "CHECK")
    {
        macro.kind = MacroKind::Check;
        addMacroArgs(macro, args);
    }
    else if (name == "CHECK_FALSE")
    {
        macro.kind = MacroKind::CheckFalse;
        addMacroArgs(macro, args);
    }
    else
    {
        return;
    }

    macros[key] = macro;
}

void TopLevelConsumer::HandleTranslationUnit(clang::ASTContext &context)
{
    ASTBuilder builder(context, macros);
    builder.buildAST(AST);
    allASTs.push_back(std::move(AST));
}

// FrontEndAction is something you want Clang to do after it creates the AST after processing a source file
// It has default ones, this is a custom one
std::unique_ptr<clang::ASTConsumer> TopLevelAction::CreateASTConsumer(
    clang::CompilerInstance &compiler,                   // Reference to Clang's internal compiler state
    llvm::StringRef filename)                            // Filename (full path) being processed
{
    // Set filename
    AST.filename = fs::path(filename.str()).stem().string();

    // Create a new "TestFinder" and point the pre-processor at it
    // The pre-processor will populate the source file with all the tests it finds
    compiler.getPreprocessor().addPPCallbacks(
        std::make_unique<TestFinder>(
            AST,
            compiler.getSourceManager(),
            compiler.getPreprocessor(),
            macros
        )
    );

    // Create a new consumer to send the AST to, and return a unique pointer to it
    // This consumer will build out the representation of the test file, and add it to the global list
    return std::make_unique<TopLevelConsumer>(AST, macros);
}

// Parses all files with the ClangTool
// Gives clang the compilation instructions, files, and action it needs to build the AST
std::vector<CustomAST>* parseTestFiles(const std::vector<std::string> &filepaths, const fs::path &debugOutputDir)
{
    // Use these arguments when compiling ("Fixed" because it doesn't change per file)
    clang::tooling::FixedCompilationDatabase compilations(
        fs::current_path().string(),
        {
            std::string("-I") + SPLASHKIT_BACKEND,
            std::string("-I") + SPLASHKIT_CATCH,
            std::string("-I") + SPLASHKIT_EASYLOGGING,
            std::string("-I") + SPLASHKIT_CORESDK
        });

    // Create a tool and assign the filepaths
    clang::tooling::ClangTool tool(compilations, filepaths);
    std::cout << "Catch: " << SPLASHKIT_CATCH << '\n';
    // For every source file, build a clang AST, create a TopLevelAction, and execute it.
    tool.run(clang::tooling::newFrontendActionFactory<TopLevelAction>().get());

    // Ensure output directory exists
    fs::create_directories(debugOutputDir);

    //Convert and save as JSON
    for (const auto &test : allASTs)
    {
        json j = test;
        fs::path outputFile = debugOutputDir / (test.filename + ".json");    // Set path
        std::ofstream file(outputFile);
        file << j.dump(4);
    }

    std::cout << "\nSaved " << filepaths.size() << " tests to " << debugOutputDir << "\n";

    return &allASTs;
}
