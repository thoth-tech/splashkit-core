#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Type
{
    std::string name;
    bool isConst = false;
    bool isPointer = false;
    std::vector<Type> templateArguments;    // e.g. SomeType<TypeA, TypeB>

    bool operator==(const Type& other) const
    {
        return name == other.name &&
            isConst == other.isConst &&
            isPointer == other.isPointer &&
            templateArguments == other.templateArguments;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(Type, name, isConst, isPointer, templateArguments)
};

struct Expression
{
    Type type;

    virtual ~Expression() = default;            // Explicit destructor for safety (pointers in derived classes)
    virtual void serialise(json &j) const = 0;  // Virtual function for base classes to define how they should be serialised
};

// Defines how nlohmann/json converts this type (Expression) to json
inline void to_json(json &j, const Expression &e)
{
    e.serialise(j);
}

// Defines how nlohmann/json converts this type (Unique pointer to Expression) to json
inline void to_json(json &j, const std::unique_ptr<Expression> &e)
{
    if (e) e->serialise(j);
    else   j = nullptr;         // This converts to "null" with nlohmann/json
}

struct LiteralExpression : Expression
{
    std::string value;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(LiteralExpression, value, type)
    // Convert the current object (the derived type) to json, and assign a "kind"
    void serialise(json &j) const override { j = *this; j["kind"] = "LiteralExpression"; }
};

struct Argument
{
    std::unique_ptr<Expression> expression;
    bool isRef = false;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(Argument, expression, isRef)
};

enum class ReferenceKind
{
    Variable,
    EnumConstant
};

struct ReferenceExpression : Expression
{
    std::string name;
    ReferenceKind refKind;
    std::string parentEnumType; // Used for enum constants
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(ReferenceExpression, type, name, refKind, type, parentEnumType)
    void serialise(json &j) const override { j = *this; j["kind"] = "ReferenceExpression"; }
};

struct ConstructorExpression : Expression
{
    std::vector<std::unique_ptr<Expression>> arguments;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(ConstructorExpression, type, arguments)
    void serialise(json &j) const override { j = *this; j["kind"] = "ConstructorExpression"; }
};

struct InitListExpression : Expression
{
    std::vector<std::unique_ptr<Expression>> elements;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(InitListExpression, elements, type)
    void serialise(json &j) const override { j = *this; j["kind"] = "InitListExpression"; }
};

struct CallExpression : Expression
{
    std::string functionName;
    std::string qualifiedFunctionName;
    std::vector<Argument> arguments;
    std::unique_ptr<Expression> parentObject; // nullptr if not a member call
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(CallExpression, type, functionName, qualifiedFunctionName, arguments, parentObject)
    void serialise(json &j) const override { j = *this; j["kind"] = "CallExpression"; }
};

struct IndexExpression : Expression
{
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> index;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(IndexExpression, type, object, index)
    void serialise(json &j) const override { j = *this; j["kind"] = "IndexExpression"; }
};

struct BinaryExpression : Expression
{
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(BinaryExpression, type, op, left, right)
    void serialise(json &j) const override { j = *this; j["kind"] = "BinaryExpression"; }
};

struct UnaryExpression : Expression
{
    std::string op;
    std::unique_ptr<Expression> operand;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(UnaryExpression, type, op, operand)
    void serialise(json &j) const override { j = *this; j["kind"] = "UnaryExpression"; }
};

struct MemberExpression : Expression
{
    std::unique_ptr<Expression> parentObject;
    std::string memberName;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(MemberExpression, type, parentObject, memberName)

    void serialise(json &j) const override { j = *this; j["kind"] = "MemberExpression"; }
};

struct VariableDeclaration
{
    std::string name;
    Type type;
    std::string declarationId;
    std::unique_ptr<Expression> initializer;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(VariableDeclaration, name, type, declarationId, initializer)
};

struct Parameter
{
    std::string name;
    Type type;
    std::unique_ptr<Expression> defaultValue;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(Parameter, name, type, defaultValue)
};

struct Statement
{
    virtual ~Statement() = default;             // Explicit destructor for safety (pointers in derived classes)
    virtual void serialise(json &j) const = 0;  // Virtual function for base classes to define how they should be serialised
};

// Defines how nlohmann/json converts this type (Statement) to json
inline void to_json(json &j, const Statement &e)
{
    e.serialise(j);
}

// Defines how nlohmann/json converts this type (Shared pointer to Statement) to json
inline void to_json(json &j, const std::shared_ptr<Statement> &e)
{
    if (e) e->serialise(j);
    else   j = nullptr;         // This converts to "null" with nlohmann/json
}

struct FunctionDeclaration
{
    std::string name;
    std::vector<Parameter> parameters;
    std::vector<std::shared_ptr<Statement>> body;
    Type returnType;
    bool isGlobal;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(FunctionDeclaration, name, parameters, body, returnType, isGlobal)
};

struct ExpressionStatement : Statement
{
    std::unique_ptr<Expression> expression;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(ExpressionStatement, expression)
    void serialise(json &j) const override { j = *this; j["kind"] = "ExpressionStatement"; }
};

struct VariableDeclarationStatement : Statement
{
    VariableDeclaration variable;
    bool isGlobal;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(VariableDeclarationStatement, variable, isGlobal)
    void serialise(json &j) const override { j = *this; j["kind"] = "VariableDeclarationStatement"; }
};

struct ReturnStatement : Statement
{
    std::unique_ptr<Expression> value;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(ReturnStatement, value)
    void serialise(json &j) const override { j = *this; j["kind"] = "ReturnStatement"; }
};

struct ForStatement : Statement
{
    std::vector<std::shared_ptr<Statement>> initialiser;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Expression> increment;
    std::vector<std::shared_ptr<Statement>> body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(ForStatement, initialiser, condition, increment, body)
    void serialise(json &j) const override { j = *this; j["kind"] = "ForStatement"; }
};

enum class AssertionType
{
    Require,
    RequireFalse,
    RequireThrows,
    Check,
    CheckFalse
};

struct AssertionStatement : Statement
{
    AssertionType type;
    std::unique_ptr<Expression> expression;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(AssertionStatement, type, expression)
    void serialise(json &j) const override { j = *this; j["kind"] = "AssertionStatement"; }
};

struct Section
{
    std::string name;
    std::vector<std::shared_ptr<Statement>> body;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(Section, name, body)
};

struct TestCase
{
    std::string name;
    std::vector<std::string> tags;
    std::vector<Section> sections;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(TestCase, name, tags, sections)
};

struct CustomAST
{
    std::string filename;
    std::vector<VariableDeclarationStatement> globals;
    std::vector<FunctionDeclaration> functions;
    std::vector<TestCase> tests;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(CustomAST, filename, globals, functions, tests)
};
