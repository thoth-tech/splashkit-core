#pragma once

#include "clang/Basic/SourceLocation.h"

enum class MacroKind
{
    TestCase,
    Section,
    Require,
    Check,
    RequireFalse,
    CheckFalse,
    RequireThrows
};

struct MacroInfo
{
    MacroKind kind;
    clang::SourceLocation location;
    clang::SourceRange argumentRange;
    std::string name;
    std::vector<std::string> tags;
};
