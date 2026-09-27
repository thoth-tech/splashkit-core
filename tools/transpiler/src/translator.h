#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "ast.h"

namespace fs = std::filesystem;

class Translator
{
public:
    virtual void translateASTSet(
        const std::vector<CustomAST> &ASTs,
        const fs::path &outputDir) = 0;
};

class CSharpTranslator : public Translator
{
private:
    // Global declaration management
    const std::string globalClassName = "GeneratedGlobals";  
    std::unordered_set<std::string> globals;

    bool isNumericType(const std::string &type);

    // Indentation helpers
    int indentLevel = 0;
    std::string indt() {
        std::string result;
        for (int i = 0; i < indentLevel; i++)
            result += "    ";
        return result;
    }
    void increaseIndent() { indentLevel++; }
    void decreaseIndent() { indentLevel--; }

    // Write functions (write the AST elements to an external file)
    void writeProjectFiles(const fs::path &outputDir, const std::string &projectName);
    void writeAST(const CustomAST &AST, const std::string &projectName, const fs::path &outputFilepath);
    void writeGlobals(const CustomAST &AST, std::ofstream &file);
    void writeTestCase(const TestCase &testCase, std::ofstream &file, std::string category);
    void writeSection(const Section &section, std::ofstream &file, std::string defaultName);
    void writeStatement(const Statement &statement, std::ofstream &file);
    void writeForStmt(const ForStatement &forStmt, std::ofstream &file);
    void writeBody(const std::vector<std::shared_ptr<Statement>> &body, std::ofstream &file);
    void writeFunctionDecl(const FunctionDeclaration &funcDecl, std::ofstream &file);

    // Translation functions (return a string to be written inline)
    std::string translateParameter(const Parameter &parameter);
    std::string translateVarDecl(const VariableDeclaration &varDecl, std::ofstream &file);
    std::string translateExpression(const Expression &expr);
    std::string translateBinaryExpr(const BinaryExpression &expr);
    std::string translateUnaryExpr(const UnaryExpression &expr);
    std::string translateCallExpr(const CallExpression &expr);
    std::string translateRefExpr(const ReferenceExpression &expr);
    std::string translateLiteralExpr(const LiteralExpression &expr);
    std::string translateConstructorExpr(const ConstructorExpression &expr);
    std::string translateIndexExpr(const IndexExpression &expr);
    std::string translateInitListExpr(const InitListExpression &expr);
    std::string translateMemberExpr(const MemberExpression &expr);
    std::string translateAssertion(const AssertionStatement &assertion);
    std::string translateStatement(const Statement &statement);
    std::string translateVarDeclStmt(const VariableDeclarationStatement &varDecl);
    std::string translateReturnStmt(const ReturnStatement &returnStmt);
    std::string translateExprStmt(const ExpressionStatement &exprStmt);
    std::string translateCallArguments(const CallExpression &expr);
    std::string translateMemberCall(const CallExpression &reference);
    std::string translateVectorMember(const CallExpression &expr);
    
    // String manipulation
    std::string toLowerCase(const std::string &name);
    std::string toCamelCase(const std::string &name);
    std::string toPascalCase(const std::string &name);
    std::string translateType(const Type &cppType);
    std::string escapeCSharpString(const std::string &value);

public:
    void translateASTSet(
        const std::vector<CustomAST> &ASTs,
        const fs::path &outputDir) override;
};
