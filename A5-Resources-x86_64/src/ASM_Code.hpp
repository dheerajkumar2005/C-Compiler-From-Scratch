#ifndef __ASM_CODE__
#define __ASM_CODE__
#include <list>
#include "ASM.hpp"

class ASM_Code{
    public:
        std::list<ASM_Statement *> *stmt_list;
        ASM_Code();
        void append_statement(ASM_Statement *asm_statement);
        void append_list(ASM_Code *asm_code);
        std::string to_string() const;
};

#endif