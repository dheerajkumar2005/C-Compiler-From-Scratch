#ifndef __ASM__
#define __ASM__

#include <string>
#include <unordered_map>
#include <iomanip>
#include "RTL.hpp"
#include "utils.hpp"
#include "support.hpp"

class ASM_Statement{
    public:
        bool is_float;
        ASM_Statement(bool is_float);
        virtual std::string to_string() const = 0;
};

class Load_Int_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register* reg;
        int ival;
        Load_Int_ASM_Statement(RTL_Register* reg, int ival);
        virtual std::string to_string() const override final;
};

class Load_Float_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register* reg;
        float fval;
        Load_Float_ASM_Statement(RTL_Register* reg, float fval);
        virtual std::string to_string() const override final;
};

class Load_String_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register* reg;
        std::string sval;
        Load_String_ASM_Statement(RTL_Register* reg, std::string sval);
        virtual std::string to_string() const override final;
};

class Load_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register* reg;
        std::string var_name;
        Load_ASM_Statement(RTL_Register* reg, std::string var_name, bool is_float = false);
        virtual std::string to_string() const override final;
};

class Store_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register *reg;
        std::string var_name;
        Store_ASM_Statement(RTL_Register *reg, std::string var_name, bool is_float = false);
        virtual std::string to_string() const override final;
};

class Move_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register *dest;
        RTL_Register *src;
        bool is_movtf;
        bool is_movt;
        Move_ASM_Statement(RTL_Register *dest, RTL_Register *src, bool is_movtf, bool is_movt, bool is_float);
        std::string to_string() const final override;
};

class Compute_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register *lhs;
        RTL_Operator op;
        RTL_Register *opd1;
        RTL_Register *opd2;
        Compute_ASM_Statement(RTL_Register *lhs, RTL_Operator op, RTL_Register *opd1, RTL_Register *opd2 = nullptr, bool is_float = false);
        virtual std::string to_string() const override final;
};

class Goto_ASM_Statement : public ASM_Statement{
    public:
        int label_number;
        Goto_ASM_Statement(int label_number);
        virtual std::string to_string() const override final;
};

class If_Goto_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register *predicate;
        int label_number;
        If_Goto_ASM_Statement(RTL_Register *predicate, int label_number);
        virtual std::string to_string() const override final;
};

class Syscall_ASM_Statement : public ASM_Statement{
    public:
        Syscall_ASM_Statement();
        virtual std::string to_string() const override final;
};

class Label_ASM_Statement : public ASM_Statement{
    public:
        int label_number;
        Label_ASM_Statement(int label_number);
        virtual std::string to_string() const override final;
};

class Return_ASM_Statement : public ASM_Statement{
    public:
        RTL_Register *reg;
        Return_ASM_Statement(RTL_Register *_reg, bool _is_float);
        virtual std::string to_string() const override final;
};



#endif