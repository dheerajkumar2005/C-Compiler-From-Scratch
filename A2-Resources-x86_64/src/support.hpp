#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include "AST.hpp"
#include "SymTabEntry.hpp"

ExprAST *process_expr(Operator op = Operator::NOP, ExprAST *opd1 = nullptr, ExprAST *opd2 = nullptr, ExprAST *opd3 = nullptr);

#endif