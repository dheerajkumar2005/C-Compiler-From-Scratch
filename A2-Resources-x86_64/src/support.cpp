#include "support.hpp"

ExprAST *process_expr(Operator op, ExprAST *operand1, ExprAST *operand2, ExprAST *operand3) {
    return new ExprAST(Type::UNKNOWN, op, operand1, operand2, operand3);
}