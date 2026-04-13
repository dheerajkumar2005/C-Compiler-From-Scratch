#ifndef __RTL_CODE__
#define __RTL_CODE__

#include <list>

#include "RTL.hpp"

class RTL_Code
{
public:
    std::list<RTL_Statement *> *stmt_list;

    RTL_Code();

    void append_statement(RTL_Statement *rtl_statement);
    void append_list(RTL_Code *rtl_code);
    bool is_empty() const;

    std::string to_string() const;
};

#endif