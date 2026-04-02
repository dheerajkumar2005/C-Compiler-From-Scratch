#ifndef __CODE__
#define __CODE__

#include <list>
#include <string>

#include "TAC.hpp"

class Code
{
public:
    std::list<TAC_Statement *> *stmt_list;
    Code();

    void append_statement(TAC_Statement *s);
    TAC_Statement *pop_statement();
    void append_list(Code *c);

    std::string to_string() const;
};

#endif