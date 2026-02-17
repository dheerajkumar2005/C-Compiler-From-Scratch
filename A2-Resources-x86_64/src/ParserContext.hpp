#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include "Program.hpp"

class ParserContext
{
public:
    Program *program_ptr;
    Procedure *func_ptr;
};

#endif
