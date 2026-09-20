#include <clang/Tooling/Tooling.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <iostream>
#include <llvm/Support/Casting.h>
#include <memory>
#include <vector>

#include "builder.h"
#include "ast.h"
#include "macro.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/OperatorKinds.h"

// Gets the line number of a declaration from the source file
// int ASTBuilder::getLineNumber(const clang::Decl *decl)
// {
//     return sourceManager.getSpellingLineNumber(decl->getLocation());
// }

unsigned ASTBuilder::getLocationKey(clang::SourceLocation loc)
{
    return sourceManager.getExpansionLoc(loc).getRawEncoding();
}

// Gets the source text of a given expression (e.g. "x + 5")  
std::string ASTBuilder::getSourceText(const clang::Expr &expr)
{
    // Token range uses the entire range of the expression
    auto range = clang::CharSourceRange::getTokenRange(expr.getSourceRange());

    clang::LangOptions options; // Default options (language rules)

    // Use the lexer to ask for the source text at the given location
    return clang::Lexer::getSourceText(range, sourceManager, options).str();
}

// Build a literal with a string value 
std::unique_ptr<Expression> ASTBuilder::buildLiteral(const clang::Expr &expr)
{
    auto result = std::make_unique<LiteralExpression>();

    // String literal
    if (auto *literal = llvm::dyn_cast<const clang::StringLiteral>(&expr))
    {
        result->value = literal->getString().str();

        Type stringType;
        stringType.name = "string";    // This ensures the type isn't "const char[9]" - not useful for translation
        result->type = stringType;
    }
    else
    {
        result->value = getSourceText(expr);
        result->type = buildType(expr.getType());
    }

    return result;
}

// Build a binary expression
std::unique_ptr<Expression> ASTBuilder::buildBinaryExpression(const clang::BinaryOperator &binary)
{
    auto result = std::make_unique<BinaryExpression>();

    result->op = binary.getOpcodeStr().str();
    result->type = buildType(binary.getType());
    result->left = buildExpression(*binary.getLHS());
    result->right = buildExpression(*binary.getRHS());

    return result;
}

// Build a unary expression
std::unique_ptr<Expression> ASTBuilder::buildUnaryExpression(const clang::UnaryOperator &unary)
{
    auto result = std::make_unique<UnaryExpression>();

    result->type = buildType(unary.getType());
    result->op = clang::UnaryOperator::getOpcodeStr(unary.getOpcode()).str();
    result->operand = buildExpression(*unary.getSubExpr());

    return result;
}

// Build initialiser list expression
std::unique_ptr<Expression> ASTBuilder::buildInitListExpression(const clang::InitListExpr &initList)
{
    auto result = std::make_unique<InitListExpression>();

    if (initList.getNumInits() > 0)
    {
        auto initListType = initList.getInit(0)->IgnoreImplicit()->getType();
        result->type = buildType(initListType);
    }

    for (unsigned i = 0; i < initList.getNumInits(); i++)
    {
        auto *element = initList.getInit(i);
        auto expr = buildExpression(*element);
        result->elements.push_back(std::move(expr));
    }

    return result;
}

// Build constructor expression
std::unique_ptr<Expression> ASTBuilder::buildConstructorExpression(const clang::CXXConstructExpr &ctorExpr)
{
    auto result = std::make_unique<ConstructorExpression>();
    result->type = buildType(ctorExpr.getType());

    for (unsigned i = 0; i < ctorExpr.getNumArgs(); i++)
    {
        auto *arg = ctorExpr.getArg(i);

        if (llvm::isa<clang::CXXDefaultArgExpr>(arg)) continue;

        auto expr = buildExpression(*arg);
        result->arguments.push_back(std::move(expr));
    }

    return result;
}

// Build a function call 
std::unique_ptr<Expression> ASTBuilder::buildFunctionCall(const clang::CallExpr &call, const clang::FunctionDecl &function)
{
    // Catch2 compiler-generated wrapper
    if (function.getNameAsString() == "__builtin_constant_p")
    {
        if (call.getNumArgs() == 1)
        {
            return buildExpression(*call.getArg(0));
        }
    }

    auto result = std::make_unique<CallExpression>();
    result->functionName = function.getNameAsString();
    result->qualifiedFunctionName = function.getQualifiedNameAsString();
    result->type = buildType(call.getType());

    // If this is a member function call, store the object.
    if (auto *memberCall = llvm::dyn_cast<const clang::CXXMemberCallExpr>(&call))
    {
        if (const clang::Expr *object = memberCall->getImplicitObjectArgument())
        {
            result->parentObject = buildExpression(*object);
        }
    }

    unsigned numArgs = call.getNumArgs();

    // Add all arguments
    for (int i = 0; i < numArgs; i++)
    {
        auto *currentArg = call.getArg(i);

        Argument argument;
        
        argument.expression = buildExpression(*currentArg);

        if (i < function.getNumParams())
        {
            // Check parameter
            const clang::ParmVarDecl *parameter = function.getParamDecl(i);

            // Is it a reference?
            if (parameter->getType()->isLValueReferenceType() &&
                !parameter->getType().getNonReferenceType().isConstQualified())
            {
                argument.isRef = true;
            }
        }

        result->arguments.push_back(std::move(argument));
    }

    return result;
}

// Build a reference to something already declared
std::unique_ptr<Expression> ASTBuilder::buildReference(const clang::DeclRefExpr &ref)
{
    auto result = std::make_unique<ReferenceExpression>();
    result->name = ref.getDecl()->getNameAsString();
    result->type = buildType(ref.getType());

    // Check enum type
    if (auto *eNumConstant = llvm::dyn_cast<clang::EnumConstantDecl>(ref.getDecl()))
    {
        result->refKind = ReferenceKind::EnumConstant;

        // Get enum type name
        if (auto *enumDecl = llvm::dyn_cast<clang::EnumDecl>(eNumConstant->getDeclContext()))
        {
            result->parentEnumType = enumDecl->getNameAsString();
        }
    }
    else if (llvm::isa<clang::VarDecl>(ref.getDecl()))
    {
        result->refKind = ReferenceKind::Variable;
    }

    return result;
}

// Expression dispatcher
std::unique_ptr<Expression> ASTBuilder::buildExpression(const clang::Expr &expr)
{
    // Literals
    if (llvm::isa<clang::IntegerLiteral>(expr) ||
        llvm::isa<clang::FloatingLiteral>(expr) ||
        llvm::isa<clang::StringLiteral>(expr) ||
        llvm::isa<clang::CXXBoolLiteralExpr>(expr) ||
        llvm::isa<clang::CXXNullPtrLiteralExpr>(expr) ||
        llvm::isa<clang::CharacterLiteral>(expr))
    {
        return buildLiteral(expr);
    }
    else if (auto *binary = llvm::dyn_cast<const clang::BinaryOperator>(&expr))
    {
        return buildBinaryExpression(*binary);
    }
    else if (auto *unary = llvm::dyn_cast<const clang::UnaryOperator>(&expr))
    {
        return buildUnaryExpression(*unary);
    }
    // Reference to variable, function, enum, etc.
    else if (auto *ref = llvm::dyn_cast<const clang::DeclRefExpr>(&expr))
    {
        return buildReference(*ref);
    }
    // Member expression
    else if (auto *member = llvm::dyn_cast<const clang::MemberExpr>(&expr))
    {
        auto memberExpr = std::make_unique<MemberExpression>();

        memberExpr->memberName = member->getMemberDecl()->getNameAsString();
        memberExpr->parentObject = buildExpression(*member->getBase());

        return memberExpr;
    }
        // Overloaded operators (e.g. someString == "testString") - store as binary expression
    else if (auto *opCall = llvm::dyn_cast<const clang::CXXOperatorCallExpr>(&expr))
    {
        // Indexed expression (like someVar[i])
        if (opCall->getOperator() == clang::OO_Subscript)
        {
            auto indexExpr = std::make_unique<IndexExpression>();

            indexExpr->object = buildExpression(*opCall->getArg(0));
            indexExpr->index = buildExpression(*opCall->getArg(1));

            return indexExpr;
        }

        // Create a binary expression node and assign the correct operator
        auto binary = std::make_unique<BinaryExpression>();

        binary->op = clang::getOperatorSpelling(opCall->getOperator());
        binary->left = buildExpression(*opCall->getArg(0));
        binary->right = buildExpression(*opCall->getArg(1));

        return binary;
    }
    // Calls
    else if (auto *call = llvm::dyn_cast<const clang::CallExpr>(&expr))
    {
        // Function call
        if (auto *funcDecl = call->getDirectCallee())
        {
            return buildFunctionCall(*call, *funcDecl);
        }
    }
    // Initialiser list
    else if (auto *initList = llvm::dyn_cast<const clang::InitListExpr>(&expr))
    {
        return buildInitListExpression(*initList);
    }
    // Initialiser list wrapper
    else if (auto *initList = llvm::dyn_cast<const clang::CXXStdInitializerListExpr>(&expr))
    {
        return buildExpression(*initList->getSubExpr());
    }
    // Implicit casts (unwrap them)
    else if (auto *cast = llvm::dyn_cast<const clang::ImplicitCastExpr>(&expr))
    {
        return buildExpression(*cast->getSubExpr());
    }
    // Parentheses (unwrap them)
    else if (auto *paren = llvm::dyn_cast<const clang::ParenExpr>(&expr))
    {
        return buildExpression(*paren->getSubExpr());
    }
    // Expressions with cleanups (cleang lifetime management - unwrap)
    else if (auto *cleanups = llvm::dyn_cast<clang::ExprWithCleanups>(&expr))
    {
        return buildExpression(*cleanups->getSubExpr());
    }
    else if (auto *compoundLiteral = llvm::dyn_cast<clang::CompoundLiteralExpr>(&expr))
    {
        return buildExpression(*compoundLiteral->getInitializer());
    }
    // Constructor expressions (or wrappers like C++ making a string() object for a string literal)
    else if (auto *construct = llvm::dyn_cast<clang::CXXConstructExpr>(&expr))
    {
        // Check the target type
        Type targetType = buildType(construct->getType());

        // If there's one argument and it's the same type as the target (e.g. color(someColor)), unwrap it
        if (construct->getNumArgs() == 1)
        {
            const clang::Expr *arg = construct->getArg(0)->IgnoreImplicit();

            // Clang has generated a copy/temporary constructor around
            // an existing variable reference. Preserve the reference.
            if (llvm::isa<clang::DeclRefExpr>(arg))
            {
                return buildExpression(*arg);
            }
        }

        // Constructor is a string - unwrap the literal
        if ((targetType.name == "string" || targetType.name == "std::string" || targetType.name == "class std::basic_string<char>") && construct->getNumArgs() > 0)
        {
            return buildExpression(*construct->getArg(0));
        }

        // Constructor uses list initialisation
        if (construct->isListInitialization())
        {
            return buildExpression(*construct->getArg(0));
        }

        return buildConstructorExpression(*construct);
    }
    // Another clang wrapper for memory management - unwrap
    else if (auto *bind = llvm::dyn_cast<clang::CXXBindTemporaryExpr>(&expr))
    {
        return buildExpression(*bind->getSubExpr());
    }
    // Another clang wrapper for memory management - unwrap
    else if (auto *mat = llvm::dyn_cast<clang::MaterializeTemporaryExpr>(&expr))
    {
        return buildExpression(*mat->getSubExpr());
    }

    std::cout
        << "Unsupported expression found at line "
        << sourceManager.getSpellingLineNumber(expr.getExprLoc())
        << "\nExpression class: "
        << expr.getStmtClassName()
        << "\nType: "
        << expr.getType().getAsString()
        << "\nSource: "
        << getSourceText(expr)
        << std::endl;
    throw std::runtime_error("Unsupported expression");
}

// Build a variable declaration
VariableDeclaration ASTBuilder::buildVariableDecl(const clang::VarDecl &var)
{
    VariableDeclaration result;

    result.name = var.getNameAsString();
    // result.line = getLineNumber(var);
    // result.location = getLocationKey(var->getLocation());

    result.type = buildType(var.getType());

    // Is it initialised?
    if (var.hasInit())
    {
        result.initializer = buildExpression(*var.getInit());
    }

    return result;
}

// Build a parameter
Parameter ASTBuilder::buildParameter(const clang::ParmVarDecl &param)
{
    Parameter result;

    result.name = param.getNameAsString();
    result.type = buildType(param.getType());

    // Is there a default?
    if (param.hasDefaultArg())
    {
        result.defaultValue = buildExpression(*param.getDefaultArg());
    }

    return result;
}

// Build a type
Type ASTBuilder::buildType(const clang::QualType &qualType)
{
    Type result;

    result.isConst = qualType.isConstQualified();

    if (qualType->isPointerType())
    {
        result.isPointer = true;
        result.name = qualType.getAsString();
    }
    else
    {
        result.name = qualType.getAsString();

        if (result.isConst)
        {
            result.name = result.name.substr(6);
        }
    }

    // Look through ElaboratedType to find the underlying template.
    clang::QualType underlyingType = qualType;

    if (const auto *elaborated =
        llvm::dyn_cast<clang::ElaboratedType>(
            underlyingType.getTypePtr()))
    {
        underlyingType = elaborated->getNamedType();
    }

    if (const auto *templateType =
        underlyingType->getAs<clang::TemplateSpecializationType>())
    {
        result.name = templateType->getTemplateName()
            .getAsTemplateDecl()
            ->getNameAsString();
        
        if (result.name == "basic_string") result.name = "string";
        else
        {
            for (const auto &arg : templateType->template_arguments())
            {
                // Ensure the argument passed to the template type is a type itself
                if (arg.getKind() != clang::TemplateArgument::Type)
                    continue;
    
                result.templateArguments.push_back(buildType(arg.getAsType())
                );
            }
        };

    }

    return result;
}

// Build a function declaration
FunctionDeclaration ASTBuilder::buildFunctionDecl(const clang::FunctionDecl &fn, bool isGlobal)
{
    FunctionDeclaration result;

    // Use getQualifiedNameAsString(); to check for SplashKit functions "splashkit_lib::draw_bitmap"

    result.name = fn.getNameAsString();
    // result.line = getLineNumber(fn);
    // result.location = getLocationKey(fn->getLocation());
    result.returnType = buildType(fn.getReturnType());
    result.isGlobal = isGlobal;

    // Get params
    for (auto *param : fn.parameters())
    {
        result.parameters.push_back(buildParameter(*param));
    }

    // Build the function body
    auto *body = llvm::cast<clang::CompoundStmt>(fn.getBody());
    for (const auto *stmt : body->body())
    {
        auto statements = buildStatements(*stmt);

        result.body.insert(result.body.end(), statements.begin(), statements.end());
    }

    return result;
}

void ASTBuilder::checkMinAndMax(
    clang::SourceLocation loc,
    clang::SourceLocation &min,
    clang::SourceLocation &max)
{
    if (loc.isInvalid()) return;

    clang::SourceLocation spelling = sourceManager.getSpellingLoc(loc);

    // Not defined in main file - ignore
    if (sourceManager.getFileID(spelling) != sourceManager.getMainFileID())
        return;

    // If this the earliest location yet?
    if (min.isInvalid() ||
        sourceManager.isBeforeInTranslationUnit(spelling, min))
    {
        min = spelling;
    }

    // Is this the latest location yet?
    if (max.isInvalid() ||
        sourceManager.isBeforeInTranslationUnit(max, spelling))
    {
        max = spelling;
    }
}

// Recurse through child statements until an expression is found
const clang::Expr* ASTBuilder::findExpressionInRange(
    const clang::Stmt *stmt,
    const clang::SourceRange targetRange,
    clang::SourceLocation &min,
    clang::SourceLocation &max)
{
    if (!stmt) return nullptr;

    clang::SourceLocation targetBegin = sourceManager.getSpellingLoc(targetRange.getBegin());

    clang::SourceLocation targetEnd = sourceManager.getSpellingLoc(targetRange.getEnd());

    // Include this node's own range
    if (auto *expr = llvm::dyn_cast<const clang::Expr>(stmt))
    {
        checkMinAndMax(sourceManager.getSpellingLoc(expr->getBeginLoc()), min, max);
        checkMinAndMax(sourceManager.getSpellingLoc(expr->getEndLoc()), min, max);
    }

    // Include all children
    for (const clang::Stmt *child : stmt->children())
    {
        clang::SourceLocation childMin;
        clang::SourceLocation childMax;

        if (auto *result = findExpressionInRange(child, targetRange, childMin, childMax))
        {
            return result;
        }

        if (childMin.isValid()) checkMinAndMax(childMin, min, max);

        if (childMax.isValid()) checkMinAndMax(childMax, min, max);
    }

    // Check the entire subtree range
    if (auto *expr = llvm::dyn_cast<const clang::Expr>(stmt))
    {
        if (min.isValid() && max.isValid() && min == targetBegin && max == targetEnd)
        {
            return expr;
        }
    }

    return nullptr;
}

// Dispatcher for building macros
std::shared_ptr<Statement> ASTBuilder::buildMacro(
    const clang::Stmt &stmt,
    const MacroInfo &macroInfo)
{
    auto result = std::make_shared<AssertionStatement>();

    switch (macroInfo.kind)
    {
        case MacroKind::Require:
        {
            result->type = AssertionType::Require;
            break;
        }
        case MacroKind::RequireFalse:
        {
            result->type = AssertionType::RequireFalse;
            break;
        }
        case MacroKind::RequireThrows:
        {
            result->type = AssertionType::RequireThrows;
            break;
        }
        case MacroKind::Check:
        {
            result->type = AssertionType::Check;
            break;
        }
        case MacroKind::CheckFalse:
        {
            result->type = AssertionType::CheckFalse;
            break;
        }
        default:
        {
            return result;
        }
    }

    clang::SourceLocation min;
    clang::SourceLocation max;

    auto argument = findExpressionInRange(&stmt, macroInfo.argumentRange, min, max);
    
    if (!argument)
    {
        std::cout << "findExpressionInRange returned nullptr!" << std::endl;
        return result;
    }
    
    result->expression = buildExpression(*argument);

    return result;
}

VariableDeclarationStatement ASTBuilder::buildVariableDeclStmt(const clang::VarDecl &var, bool isGlobal)
{
    VariableDeclarationStatement result;
    result.variable = buildVariableDecl(var);
    result.isGlobal = isGlobal;
    return result;
}

std::vector<std::shared_ptr<Statement>> ASTBuilder::buildStatements(const clang::Stmt &stmt)
{
    std::vector<std::shared_ptr<Statement>> result;
    // Start by checking for macros
    // Get the key for this location
    unsigned key = getLocationKey(stmt.getBeginLoc());

    // Check for any macros at this location
    auto it = macros.find(key);

    // If there is one, build it
    if (it != macros.end())
    {
        auto macro = buildMacro(stmt, it->second);
        result.push_back(macro);
    }
    // Declaration statement
    else if (auto *declStmt = llvm::dyn_cast<clang::DeclStmt>(&stmt))
    {
        // Check declarations
        for (clang::Decl *decl : declStmt->decls())
        {
            // Variable declaration
            if (auto *var = llvm::dyn_cast<clang::VarDecl>(decl))
            {
                auto varDecl = std::make_shared<VariableDeclarationStatement>(buildVariableDeclStmt(*var));
                result.push_back(varDecl);
            }
        }
    }
    else if (auto *retrn = llvm::dyn_cast<clang::ReturnStmt>(&stmt))
    {
        auto returnStmt = std::make_shared<ReturnStatement>();
        returnStmt->value = buildExpression(*retrn->getRetValue());
        result.push_back(returnStmt);
    }
    // Expression statement
    else if (auto *expr = llvm::dyn_cast<clang::Expr>(&stmt))
    {
        auto exprStmt = std::make_shared<ExpressionStatement>();
        exprStmt->expression = buildExpression(*expr);
        result.push_back(exprStmt);
    }
    // For loop
    else if (auto *clangForStmt = llvm::dyn_cast<clang::ForStmt>(&stmt))
    {
        auto forStmt = std::make_shared<ForStatement>();

        // If there is an initialiser (or multiple), add it
        if (const auto *init = clangForStmt->getInit())
        {
            auto initStatements = buildStatements(*init);

            if (!initStatements.empty())
                forStmt->initialiser = initStatements;
        }

        // If there's a condition, add it
        if (const auto *cond = clangForStmt->getCond())
        {
            forStmt->condition = buildExpression(*cond);
        }

        // If there's an increment expression, add it
        if (const auto *inc = clangForStmt->getInc())
        {
            forStmt->increment = buildExpression(*inc);
        }

        // If there's a body, add it
        if (const auto *body = llvm::dyn_cast<clang::CompoundStmt>(clangForStmt->getBody()))
        {
            for (const auto *stmt : body->body())
            {
                auto statements = buildStatements(*stmt);

                forStmt->body.insert(
                    forStmt->body.end(),
                    statements.begin(),
                    statements.end()
                );
            }
        }

        result.push_back(forStmt);
    }

    return result;
}

std::vector<Section> ASTBuilder::buildSections(
    std::vector<std::shared_ptr<Statement>> runningStmts,
    const clang::CompoundStmt &srcStatements,
    std::string sectionName)
{
    std::vector<Section> foundSections;

    // For every statement
    for (clang::Stmt *stmt : srcStatements.body())
    {
        // Get the key for this location
        unsigned key = getLocationKey(stmt->getBeginLoc());

        // Check for any macros at this location
        auto it = macros.find(key);

        // Section found
        if (it != macros.end() && it->second.kind == MacroKind::Section)
        {
            MacroInfo macroInfo = it->second;

            // The SECTION macro becomes an if statement during compilation
            auto *ifStmt = llvm::dyn_cast<clang::IfStmt>(stmt);

            if (!ifStmt) break;

            // The block of the if statement becomes the "body" of the section
            auto *compound =
                llvm::dyn_cast<clang::CompoundStmt>(ifStmt->getThen());

            if (!compound) break;

            // Explore section body
            std::vector<Section> subsections = buildSections(runningStmts, *compound, macroInfo.name);
            // Add any subsections to the running list
            foundSections.insert(foundSections.end(), subsections.begin(), subsections.end());
        }
        // Not a section - build statement(s)
        else
        {
            // Build statements
            auto stmts = buildStatements(*stmt);
            
            if (!stmts.empty())
            {
                // Add the new statements to the end of all the sections found at this level
                for (auto &section : foundSections)
                {
                    section.body.insert(section.body.end(), stmts.begin(), stmts.end());
                }

                runningStmts.insert(runningStmts.end(), stmts.begin(), stmts.end());
            }
        }
    }

    if (foundSections.empty() && !runningStmts.empty())
    {
        // No sections found at this level (leaf node)
        // Create the new section
        Section newSection;
        newSection.name = sectionName;
        // Add all the setup statements so far
        newSection.body.insert(newSection.body.end(), runningStmts.begin(), runningStmts.end());
        foundSections.push_back(newSection);
    }

    return foundSections;
}

// Build a test case
TestCase ASTBuilder::buildTestCase(
    const clang::FunctionDecl &func,
    const MacroInfo &macroInfo)
{
    TestCase result;

    // Utilise macro info obtained by the preprocessor
    result.name = macroInfo.name;
    // testCase.line = getLineNumber(func);
    // testCase.location = getLocationKey(macroInfo.location);
    result.tags = macroInfo.tags;

    // Build sections
    clang::Stmt *body = func.getBody();
    auto *compound = llvm::dyn_cast<clang::CompoundStmt>(body);
    if (!compound) return result;
    result.sections = buildSections({}, *compound, "");

    return result;
}

// Build a test file (top level)
void ASTBuilder::buildAST(CustomAST &AST)
{
    // Root node of the AST
    clang::TranslationUnitDecl *translationUnit = context.getTranslationUnitDecl();

    // Top-level declarations
    for (clang::Decl *decl : translationUnit->decls())
    {
        clang::SourceLocation loc = decl->getLocation();
        clang::SourceLocation expansionLoc =
            sourceManager.getExpansionLoc(loc);

        // If the declaration isn't written in the main file, ignore it
        if (!sourceManager.isWrittenInMainFile(expansionLoc))
        {
            continue;
        }

        // Try to cast as each type of declaration, and manage accordingly
        // Variables
        if (auto *var = llvm::dyn_cast<clang::VarDecl>(decl))
        {
            AST.globals.push_back(buildVariableDeclStmt(*var, true));
        }
        else if (auto *func = llvm::dyn_cast<clang::FunctionDecl>(decl))
        {
            if (func != func->getCanonicalDecl())
                continue;

            // Get the key for this location
            unsigned key = getLocationKey(func->getLocation());

            // Check for any macros at this location
            auto it = macros.find(key);

            // If there is one, and it's a test case, build it
            if (it != macros.end() && it->second.kind == MacroKind::TestCase)
            {
                AST.tests.push_back(buildTestCase(*func, it->second));
            }
            // Otherwise it's a top-level function
            else
            {
                AST.functions.push_back(buildFunctionDecl(*func, true));
            }
        }
    }
}
