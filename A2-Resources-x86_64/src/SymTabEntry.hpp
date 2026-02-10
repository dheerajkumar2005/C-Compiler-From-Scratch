#ifndef SYM_TAB_ENTRY_HPP
#define SYM_TAB_ENTRY_HPP

#include <string>
#include <map>

enum class Type {
    UNKNOWN, // default type
    VOID,
	INT,
	FLOAT,
	STR,
    BOOL,
};

struct SymTabEntry {
    // NOTE: This won't work when we have more than one scope
    // Whenever we introduce multiple functions, compound statements, etc.
    static std::map<std::string, SymTabEntry *> global_scope;
    static std::map<std::string, SymTabEntry *> local_scope;

    bool is_global; // Does this entry correspond to a variable defined in the local scope or the global scope?
    std::string var_name;
    Type type;
};

#endif
