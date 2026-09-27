#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include "ast.h"
#include "macro.h"
#include "clang/AST/Stmt.h"

// Builds a data structure for a custom AST
class ASTBuilder
{
private:
    // Need source manager for info about the source file (like line numbers)
    clang::ASTContext &context;
    clang::SourceManager &sourceManager;
    std::unordered_map<unsigned, MacroInfo> &macros;

public:
    ASTBuilder(clang::ASTContext &context, std::unordered_map<unsigned, MacroInfo> &macros)
        : context(context),
          sourceManager(context.getSourceManager()),
          macros(macros)
    {
    }

    // Gets the line number of a declaration from the source file
    // int getLineNumber(const clang::Decl *decl);

    unsigned getLocationKey(clang::SourceLocation loc);

    // Gets the source text of a given expression (e.g. "x + 5")  
    std::string getSourceText(const clang::Expr &expr);

    // Build a literal with a string value 
    std::unique_ptr<Expression> buildLiteral(const clang::Expr &expr);

    // Build a binary expression
    std::unique_ptr<Expression> buildBinaryExpression(const clang::BinaryOperator &binary);

    // Build a unary expression
    std::unique_ptr<Expression> buildUnaryExpression(const clang::UnaryOperator &unary);

    // Build an initialiser list expression
    std::unique_ptr<Expression> buildInitListExpression(const clang::InitListExpr &initList);

    // Build a constructor expression
    std::unique_ptr<Expression> buildConstructorExpression(const clang::CXXConstructExpr &ctorExpr);

    // Build a function call 
    std::unique_ptr<Expression> buildFunctionCall(const clang::CallExpr &call, const clang::FunctionDecl &function);

    // Build a reference to something already declared
    std::unique_ptr<Expression> buildReference(const clang::DeclRefExpr &ref);

    // Expression dispatcher
    std::unique_ptr<Expression> buildExpression(const clang::Expr &expr);

    // Build a variable declaration
    VariableDeclaration buildVariableDecl(const clang::VarDecl &var);

    VariableDeclarationStatement buildVariableDeclStmt(const clang::VarDecl &var, bool isGlobal = false);

    // Build a parameter
    Parameter buildParameter(const clang::ParmVarDecl &param);

    // Build a type
    Type buildType(const clang::QualType &qualType);

    // Build a function declaration
    FunctionDeclaration buildFunctionDecl(const clang::FunctionDecl &fn, bool isGlobal = false);

    void checkMinAndMax(clang::SourceLocation loc, clang::SourceLocation &min, clang::SourceLocation &max);

    // Recurse through child statements until an expression is found
    const clang::Expr* findExpressionInRange(const clang::Stmt *stmt, const clang::SourceRange targetRange, clang::SourceLocation &min, clang::SourceLocation &max);

    // Dispatcher for building macros
    std::shared_ptr<Statement> buildMacro(const clang::Stmt &stmt, const MacroInfo &macroInfo);

    std::vector<std::shared_ptr<Statement>> buildStatements(const clang::Stmt &stmt);

    std::vector<Section> buildSections(std::vector<std::shared_ptr<Statement>> cumulativeStatements, const clang::CompoundStmt &srcStatements, std::string sectionName);

    // Build a test case
    TestCase buildTestCase(const clang::FunctionDecl &func, const MacroInfo &macroInfo);

    // Build a custom AST
    void buildAST(CustomAST &AST);

    const clang::Expr* findCatchAssertionExpression(const clang::Stmt* stmt);
    const clang::Expr* findBooleanExpression(const clang::Stmt* stmt);
};
