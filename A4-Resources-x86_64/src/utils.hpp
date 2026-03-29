#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <sstream>

#include "Errors.hpp"

const int PRIORITY_V0 = 1;
const int PRIORITY_F12 = 25;
const int PRIORITY_A0 = 100;
const int PRIORITY_V1 = 200;
const int PRIORITY_F0 = 300;

enum class Type
{
    VOID,
    INT,
    FLOAT,
    STR,
    BOOL,
};

enum class RTL_Operator
{
    NEGATE,      // uminus
    LOGICAL_NOT, // not
    ADD,         // add
    SUBTRACT,    // sub
    MULTIPLY,    // mul
    DIVIDE,      // div
    LOGICAL_AND, // and
    LOGICAL_OR,  // or
    LT,          // slt
    LE,          // sle
    GT,          // sgt
    GE,          // sge
    NE,          // sne
    EQ,          // seq
};

enum class TAC_Operator
{
    NOP,
    NEGATE,
    LOGICAL_NOT,
    ADD,
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

enum class Unary_Operator
{
    NEGATE,
    LOGICAL_NOT,
};

enum class Binary_Operator
{
    ADD,
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

enum class Ternary_Operator
{
    QUESTION_MARK_COLON,
};

std::ostream &operator<<(std::ostream &os, Type t);
std::string type_to_string(Type type);

std::ostream &operator<<(std::ostream &os, TAC_Operator op);

std::ostream &operator<<(std::ostream &os, Binary_Operator op);
std::ostream &operator<<(std::ostream &os, Unary_Operator op);
std::ostream &operator<<(std::ostream &os, Ternary_Operator op);
std::ostream &operator<<(std::ostream &os, RTL_Operator op);

std::string op_to_string(TAC_Operator op);

std::string op_to_string(Binary_Operator op);
std::string op_to_string(Unary_Operator op);
std::string op_to_string(Ternary_Operator op);

std::string op_to_string(RTL_Operator op);
std::string op_float_to_string(RTL_Operator op);

TAC_Operator binary_to_tac(Binary_Operator op);
TAC_Operator unary_to_tac(Unary_Operator op);
RTL_Operator invert_op(RTL_Operator op);
RTL_Operator tac_to_rtl(TAC_Operator op);
std::string rtl_priority_to_register(int priority);

bool is_relational_op(RTL_Operator op);

#endif