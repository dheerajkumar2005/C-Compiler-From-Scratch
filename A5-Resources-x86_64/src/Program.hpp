#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "SemanticError.hpp"
#include "utils.hpp"

enum class Entry_Kind
{
    VARIABLE,
    FUNCTION,
    PARAMETER,
    STEMP, // Shared Temporary
};

// std::ostream &operator<<(std::ostream &os, const Entry_Kind &ek);

enum class Scope_Kind
{
    GLOBAL,
    FUNCTION,
    // later: BLOCK
};

// std::ostream &operator<<(std::ostream &os, const Scope_Kind &sk);

struct Func_Signature
{
    std::string name;
    Type return_type;
    std::vector<Type> param_types;

    std::vector<std::string> param_names;

    bool has_return;

    Func_Signature(const std::string &name, Type return_type);

    void add_param(const std::string &param_name, Type type);

    bool operator==(const Func_Signature &other) const;
    bool operator!=(const Func_Signature &other) const;
};

struct Symbol_Table_Entry
{

    Entry_Kind kind;
    Type type;

    Symbol_Table_Entry(Entry_Kind _kind, Type _type);
    virtual ~Symbol_Table_Entry() = 0;
};

struct Data_Entry : public Symbol_Table_Entry
{
    int size;
    int offset;

    Data_Entry(Entry_Kind _kind, Type _type, int _size, int _offset);
};

class Scope
{
    int param_offset;
    int local_offset;

public:
    Scope_Kind kind;
    Scope *parent_scope;

    std::map<std::string, Symbol_Table_Entry *> sym_tab; // Ordered in alphabetical order of identifier
    std::vector<std::pair<std::string, Symbol_Table_Entry *>> sym_vec;
    Func_Signature *func_sig; // nullptr for non-functions

    Scope(Scope_Kind kind, Scope *parent_scope = nullptr, Func_Signature *func_sig = nullptr);

    void add_param(Type param_type, const std::string &param_name);
    void add_local(Type local_type, const std::string &local_name);
    void add_stemp(Type stemp_type, int stemp_id);

    int get_size_of_locals() const;
};

#endif
