#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <sstream>

#include "Errors.hpp"

enum class Type
{
    VOID,
    INT,
    FLOAT,
    STR,
    BOOL,
};

enum class Operator : int
{
    NOP = 0,
};

enum class Unary_Operator : int
{
    NEGATE = 100,
    LOGICAL_NOT,
};

enum class Binary_Operator : int
{
    ADD = 200,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    LOGICAL_AND,
    LOGICAL_OR,
    LT,
    LE,
    GT,
    GE,
    NE,
    EQ,
};

enum class Ternary_Operator : int
{
    QUESTION_MARK_COLON = 300,
};

std::ostream &operator<<(std::ostream &os, Type t);
std::string type_to_string(Type type);

std::ostream &operator<<(std::ostream &os, Binary_Operator op);
std::ostream &operator<<(std::ostream &os, Unary_Operator op);
std::ostream &operator<<(std::ostream &os, Ternary_Operator op);

std::string op_to_string(Binary_Operator op);
std::string op_to_string(Unary_Operator op);
std::string op_to_string(Ternary_Operator op);

#endif