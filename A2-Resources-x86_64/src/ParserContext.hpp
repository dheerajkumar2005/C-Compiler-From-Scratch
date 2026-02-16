#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include "Program.hpp"

class ParserContext
{
public:
    Program *program_ptr;
    Procedure *main_func_ptr;
};

#endif
