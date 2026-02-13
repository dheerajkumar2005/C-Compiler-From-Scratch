#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include "AST.hpp"
#include "SymTabEntry.hpp"

ExprAST *process_expr(Operator op = Operator::NOP, ExprAST *operand1 = nullptr, ExprAST *operand2 = nullptr, ExprAST *operand3 = nullptr);

#endif