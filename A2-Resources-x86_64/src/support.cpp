#include "support.hpp"

ExprAST *process_expr(Operator op, ExprAST *opd1, ExprAST *opd2, ExprAST *opd3)
{
    return new ExprAST(Type::UNKNOWN, op, opd1, opd2, opd3);
}