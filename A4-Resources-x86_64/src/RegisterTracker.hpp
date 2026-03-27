#ifndef __REGISTER_TRACKER__
#define __REGISTER_TRACKER__

#include <unordered_map>
#include <set>

#include "TAC.hpp"
#include "RTL.hpp"

class RegisterTracker
{
    std::unordered_map<TAC_Operand *, RTL_Register *> regMap;
    // TODO: Ensure this is sorted in ascending order of register priority
    std::set<RTL_Register *> availableRegs;
};

#endif