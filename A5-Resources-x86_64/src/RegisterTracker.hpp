#ifndef __REGISTER_TRACKER__
#define __REGISTER_TRACKER__

#include <map>
#include <unordered_map>

#include "utils.hpp"
#include "RTL.hpp"

class TAC_Operand
{
public:
    Type type;

    TAC_Operand(Type type);
    virtual std::string to_string() const = 0;
};

class RegisterTracker
{

public:
    std::map<int, RTL_Register *> all_regs;

    std::unordered_map<TAC_Operand *, int> reg_map;
    std::map<int, bool> available_int_regs;
    std::map<int, bool> available_float_regs;

    RegisterTracker();

    RTL_Register *get_register(TAC_Operand *opd);
    RTL_Register *get_register(int priority);

    RTL_Register *get_int_register();
    RTL_Register *get_float_register();

    void mark(TAC_Operand *opd, RTL_Register *reg);

    void free_register(TAC_Operand *opd, RTL_Register *reg);
};

#endif
