#ifndef __RTL__
#define __RTL__

#include "TAC.hpp"

// TODO: Not even considering float right now, only int

enum class RTL_Operator
{
    // TODO: ADD THESE
};

class RTL_Register
{
    int priority;
};

class RTL_Statement
{
};

class RTL_Code
{
    std::list<RTL_Statement *> *stmt_list;
};

class Load_Int_RTL_Statement : public RTL_Statement
{
    int val;
};

class Load_RTL_Statement : public RTL_Statement
{
    // TODO: Check whether it is Variable_TAC_Operand or Shared_Temporary_TAC_Operand
    TAC_Operand *var;
};

class Store_RTL_Statement : public RTL_Statement
{
    // TODO: Check whether it is Variable_TAC_Operand or Shared_Temporary_TAC_Operand
    TAC_Operand *var;
};

class Compute_RTL_Statement : public RTL_Statement
{
    RTL_Register *lhs;
    RTL_Operator op;
    RTL_Register *opd1;
    RTL_Register *opd2;
};

class Goto_RTL_Statement : public RTL_Statement
{
    int label_number;
};

class If_Goto_RTL_Statement : public RTL_Statement
{
    RTL_Register *predicate;
    int label_number;
};

class Read_RTL_Statement : public RTL_Statement
{
    RTL_Register *var;
};

class Write_RTL_Statement : public RTL_Statement
{
    RTL_Register *var;
};

class Label_RTL_Statement : public RTL_Statement
{
    int label_number;
};

#endif
