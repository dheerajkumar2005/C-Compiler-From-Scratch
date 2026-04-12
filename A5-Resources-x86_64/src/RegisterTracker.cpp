#include "RegisterTracker.hpp"

TAC_Operand::TAC_Operand(Type type) : type(type)
{
}

RegisterTracker::RegisterTracker()
    : all_regs(), reg_map(), available_int_regs(), available_float_regs()
{
    // v0 to s7
    for (int i = 1; i <= 19; i++)
    {
        RTL_Register *reg = new RTL_Register(i);
        all_regs[i] = reg;
        available_int_regs[i] = true;
    }

    // f2 to f30
    for (int i = 20; i <= 34; i++)
    {
        RTL_Register *reg = new RTL_Register(i);
        all_regs[i] = reg;
        available_float_regs[i] = true;
    }

    // a0
    RTL_Register *reg = new RTL_Register(PRIORITY_A0);
    all_regs[PRIORITY_A0] = reg;

    // v1
    reg = new RTL_Register(PRIORITY_V1);
    all_regs[PRIORITY_V1] = reg;

    // f0
    reg = new RTL_Register(PRIORITY_F0);
    all_regs[PRIORITY_F0] = reg;

    // The reserved float register is f12
    // TODO: Write a testcase which requires the use of f12 in something else, then use it for printing a float

    // not really RTL registers but what the heck
    sp = new RTL_Register(PRIORITY_SP);
    fp = new RTL_Register(PRIORITY_FP);
}

RTL_Register *RegisterTracker::get_register(TAC_Operand *opd)
{
    if (!opd || reg_map.find(opd) == reg_map.end() || all_regs.find(reg_map[opd]) == all_regs.end())
    {
        return nullptr;
    }

    return all_regs[reg_map[opd]];
}

RTL_Register *RegisterTracker::get_register(int priority)
{
    if (priority == PRIORITY_A0 || priority == PRIORITY_F0 || priority == PRIORITY_V1)
    {
        return all_regs[priority];
    }
    if (all_regs.find(priority) != all_regs.end())
    {
        if (available_int_regs.find(priority) != available_int_regs.end() && available_int_regs[priority])
        {
            return all_regs[priority];
        }
        if (available_float_regs.find(priority) != available_float_regs.end() && available_float_regs[priority])
        {
            return all_regs[priority];
        }
    }

    throw_SemanticError("Expected to find unused register with priority " + std::to_string(priority));
    return nullptr;
}

RTL_Register *RegisterTracker::get_int_register()
{
    for (auto &[priority, is_available] : available_int_regs)
    {
        if (is_available)
        {
            is_available = false;
            return all_regs[priority];
        }
    }

    throw_SemanticError("Out of int registers!!!");
    return nullptr;
}

RTL_Register *RegisterTracker::get_float_register()
{
    for (auto it = available_float_regs.begin(); it != available_float_regs.end(); ++it)
    {
        if (it->second)
        {
            it->second = false;
            return all_regs[it->first];
        }
    }

    throw_SemanticError("Out of float registers!!!");
    return nullptr;
}

void RegisterTracker::mark(TAC_Operand *opd, RTL_Register *reg)
{
    if (opd)
    {
        reg_map[opd] = reg->priority;
    }
}

void RegisterTracker::free_register(TAC_Operand *opd, RTL_Register *reg)
{
    reg_map.erase(opd);

    if (reg)
    {
        int priority = reg->priority;

        if (available_int_regs.find(priority) != available_int_regs.end())
        {
            available_int_regs[priority] = true;
        }
        if (available_float_regs.find(priority) != available_float_regs.end())
        {
            available_float_regs[priority] = true;
        }
    }
}
