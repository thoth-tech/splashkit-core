#pragma once

#include <clang/Frontend/FrontendActions.h>
#include <string>
#include "ast.h"
#include "macro.h"

namespace fs = std::filesystem;

std::string getMacroArgumentString(
    const clang::MacroArgs *args,
    unsigned tokenIndex,
    clang::Preprocessor &pp);

// Takes a set of tags in the form "[tag1][tag2][tag3]"
// Returns each tag as a string in a vector
std::vector<std::string> parseTags(std::string tagString);

// PPCallbacks is a pre-processor interface. MacroExpands() is called when the pre-processor encounters a Macro
class TestFinder : public clang::PPCallbacks
{
private:
    CustomAST &AST;
    clang::SourceManager &sourceManager;
    clang::Preprocessor &pp;
    std::unordered_map<unsigned, MacroInfo> &macros;
    unsigned currentTestKey = 0;

public:
    TestFinder(CustomAST &AST, clang::SourceManager &sm, clang::Preprocessor &pp, std::unordered_map<unsigned, MacroInfo> &macros)
        : AST(AST), sourceManager(sm), pp(pp), macros(macros)
    {
    }

    void MacroExpands(
        const clang::Token &macroName,
        const clang::MacroDefinition &macroDefinition,
        clang::SourceRange range,
        const clang::MacroArgs *args
    ) override;
};

// ASTConsumer is an interface used to write generic actions on an AST, regardless of how the AST was produced
class TopLevelConsumer : public clang::ASTConsumer
{
private:
    CustomAST &AST;
    std::unordered_map<unsigned, MacroInfo> &macros;

public:
    TopLevelConsumer(CustomAST &AST, std::unordered_map<unsigned, MacroInfo> &macros) : AST(AST), macros(macros) { }

    void HandleTranslationUnit(clang::ASTContext &context) override;
};

// FrontEndAction is something you want Clang to do after it creates the AST after processing a source file
// It has default ones, this is a custom one
class TopLevelAction : public clang::ASTFrontendAction
{
private:
    CustomAST AST;
    std::unordered_map<unsigned, MacroInfo> macros;

public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &compiler,                  // Reference to Clang's internal compiler state
        llvm::StringRef filename                            // Filename (full path) being processed
    ) override;
};

// Parses all files with the ClangTool
// Gives clang the compilation instructions, files, and action it needs to build the AST
std::vector<CustomAST>* parseTestFiles(const std::vector<std::string>& filepaths, const fs::path &debugOutputDir);
