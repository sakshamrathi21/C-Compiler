#ifndef ASM_HH
#define ASM_HH

#include <iostream>
#include "types.hh"
#include <string>
#include <map>
#include <set>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;

string ins_to_string(asm_instruction ins);

class ASM_Opd
{
    public:
    virtual string get_operand_string() { return ""; }
};


class ASM_Statement
{
    public:
    asm_instruction asm_ins;
    ASM_Opd* res = NULL;
    ASM_Opd* first = NULL;
    ASM_Opd* second = NULL;
    virtual void print_asm() {}
};


class ASM_Double_Const_Opd : public ASM_Opd
{
    public:
    ASM_Double_Const_Opd(double value);
    double value;
    virtual string get_operand_string();
};


class ASM_Int_Const_Opd : public ASM_Opd
{
    public:
    ASM_Int_Const_Opd(int value);
    int value;
    virtual string get_operand_string();
};


class ASM_Label_Opd : public ASM_Opd
{
    public:
    int label_number;
    ASM_Label_Opd(int label_number);
    virtual string get_operand_string();
};


class ASM_Mem_Opd : public ASM_Opd
{
    public:
    int offset = 0;
    reg base_reg = fp;
    string var_name = "";
    ASM_Mem_Opd(int offset, reg base_reg=fp);
    ASM_Mem_Opd(string var_name);
    virtual string get_operand_string();
};


class ASM_Register_Opd : public ASM_Opd
{
    public:
    reg reg_num;
    ASM_Register_Opd(reg reg_num);
    virtual string get_operand_string();
};


class ASM_String_Const_Opd : public ASM_Opd
{
    public:
    int string_number;
    ASM_String_Const_Opd(int string_number);
    virtual string get_operand_string();
};


class Compute_ASM_Statement : public ASM_Statement
{
    public:
    Compute_ASM_Statement(ASM_Opd* first1, ASM_Opd* second1, ASM_Opd* res1, instruction rtl_ins);
    Compute_ASM_Statement(ASM_Opd* first1, ASM_Opd* second1, ASM_Opd* res1, asm_instruction asm_ins1);
    virtual void print_asm();
};

class Call_ASM_Statement : public ASM_Statement
{
    public:
    string func_name;
    Call_ASM_Statement(string func_name);
    virtual void print_asm();
};

class Goto_ASM_Statement : public ASM_Statement
{
    public:
    Goto_ASM_Statement(ASM_Opd* label);
    ASM_Opd* label;
    virtual void print_asm();
};

class If_Goto_ASM_Statement : public ASM_Statement
{
    public:
    If_Goto_ASM_Statement(ASM_Opd* r, ASM_Opd* l);
    ASM_Opd* reg;
    ASM_Opd* label;
    virtual void print_asm();
};

class Jump_Reg_ASM_Statement : public ASM_Statement
{
    public:
    Jump_Reg_ASM_Statement(ASM_Opd* r);
    ASM_Opd* reg;
    virtual void print_asm();
};

class Label_ASM_Statement : public ASM_Statement
{
    public:
    Label_ASM_Statement(ASM_Opd* first1);
    Label_ASM_Statement(string str);
    virtual void print_asm();
    ASM_Opd* label = NULL;
    string label_str = "";
};

class Move_ASM_Statement : public ASM_Statement
{
    public:
    Move_ASM_Statement(ASM_Opd* first1, ASM_Opd* res1, instruction rtl_ins);
    Move_ASM_Statement(ASM_Opd* first1, ASM_Opd* res1, asm_instruction asm_ins1);
    virtual void print_asm();
};

class Syscall_ASM_Statement : public ASM_Statement
{
    public:
    Syscall_ASM_Statement();
    virtual void print_asm();
};

class Return_ASM_Statement : public ASM_Statement
{
    public:
    Return_ASM_Statement(string curr_func);
    string curr_func;
    virtual void print_asm();
};


#endif