#include "support.hpp"

IntLiteralAST *process_int_literal(int ival) {
	IntLiteralAST *ptr = new IntLiteralAST(ival);
	return ptr;
}

FloatLiteralAST *process_float_literal(float fval) {
	FloatLiteralAST *ptr = new FloatLiteralAST(fval);
	return ptr;
}

StrLiteralAST *process_str_literal(char *_sval) {
	std::string sval(_sval);
	StrLiteralAST *ptr = new StrLiteralAST(sval);
	return ptr;
}

// TODO: Complete this
VarAST *process_var(char *_var_name) {
    std::string var_name(_var_name);
	VarAST *ptr = new VarAST(var_name);
    return ptr;
}