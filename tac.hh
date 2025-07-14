#ifndef CODE_HH
#define CODE_HH
#include <map>
#include "types.hh"
#include <vector>
#include "rtl.hh"
using namespace std;

class TAC_Opd
{
    public:
    TAC_Opd();
    ~TAC_Opd() {}
    virtual void print_opd() = 0;
    virtual string get_operand_string() = 0;
    data_type dt;
    tac_opd tac_opd_type;
};

class Variable_TAC_Opd : public TAC_Opd
{
    public:
    string * name;
    Variable_TAC_Opd(string * s, data_type dt);
    ~Variable_TAC_Opd() {}

    void print_opd();
    string get_operand_string();
};


class Temporary_TAC_Opd : public TAC_Opd
{
    public:
    int temp_number;


    Temporary_TAC_Opd(data_type dt);
    ~Temporary_TAC_Opd() {}
    void print_opd();
    string get_operand_string();
};

class Saved_Temporary_TAC_Opd : public TAC_Opd
{
    public:
    int saved_temp_number;

    Saved_Temporary_TAC_Opd(data_type dt, int label_number);
    Saved_Temporary_TAC_Opd(data_type dt);
    ~Saved_Temporary_TAC_Opd() {}
    void print_opd();
    string get_operand_string();
};

class Label_TAC_Opd : public TAC_Opd
{
    public:
    int label_number;
    Label_TAC_Opd();
    ~Label_TAC_Opd() {}
    void print_opd();
    string get_operand_string();
};

class Int_Const_TAC_Opd : public TAC_Opd
{
    public:
    int num;
    Int_Const_TAC_Opd(int n);
    ~Int_Const_TAC_Opd() {}

    void print_opd();
    string get_operand_string();
};

class Double_Const_TAC_Opd : public TAC_Opd
{
    public:
    double num;
    Double_Const_TAC_Opd(double n);
    ~Double_Const_TAC_Opd() {}

    void print_opd();
    string get_operand_string();
};

class String_Const_TAC_Opd : public TAC_Opd
{
    public:
    string * s;


    String_Const_TAC_Opd(string * s);
    ~String_Const_TAC_Opd() {}

    void print_opd();
    string get_operand_string();
};


class TAC_Statement
{
    public:
    virtual void print_stmt();
    virtual void make_rtl() = 0;
    vector<RTL_Statement*> all_rtl_statements;
};


class Assgn_TAC_Statement : public TAC_Statement
{
    TAC_Opd * lhs;
    op_type op;
    rel_op_type relop;
    log_op_type logop;
    TAC_Opd * opd1;
    TAC_Opd * opd2;
    int operand_type = 1;
    public:
    Assgn_TAC_Statement() { }
    Assgn_TAC_Statement(TAC_Opd * l, op_type o, TAC_Opd *o1, TAC_Opd * o2) { lhs = l ; op = o ; opd1 = o1; opd2 = o2; }
    Assgn_TAC_Statement(TAC_Opd * l, op_type o, TAC_Opd *o1) { lhs = l ; op = o; opd1 = o1 ; opd2 = NULL;}
    Assgn_TAC_Statement(TAC_Opd * l, rel_op_type o, TAC_Opd *o1, TAC_Opd * o2) { lhs = l ; relop = o ; opd1 = o1; opd2 = o2; operand_type = 2; }
    Assgn_TAC_Statement(TAC_Opd * l, log_op_type o, TAC_Opd *o1, TAC_Opd * o2) { lhs = l ; logop = o ; opd1 = o1; opd2 = o2; operand_type = 3; }
    Assgn_TAC_Statement(TAC_Opd * l, log_op_type o, TAC_Opd *o1) { lhs = l ; logop = o; opd1 = o1; opd2 = NULL; operand_type = 3; }
    Assgn_TAC_Statement(TAC_Opd * l, TAC_Opd *o1) {lhs = l; op = COPY__h ; opd1 = o1 ; opd2 = NULL;}
    Assgn_TAC_Statement(TAC_Opd * o1) { lhs = NULL; op = COPY__h ; opd1 = o1 ; opd2 = NULL; }
    ~Assgn_TAC_Statement() {}
    virtual void make_rtl();
    virtual void print_stmt();
};

class If_Goto_TAC_Statement : public TAC_Statement
{
    public:
    TAC_Opd * temp;
    TAC_Opd * label;
    If_Goto_TAC_Statement(TAC_Opd * temp, TAC_Opd* label) : temp(temp), label(label) {}
    ~If_Goto_TAC_Statement() {}
    virtual void make_rtl();
    virtual void print_stmt();
};

class Gotot_TAC_Statement : public TAC_Statement
{
    public:
    TAC_Opd * label;
    Gotot_TAC_Statement(TAC_Opd * label) : label(label) {}
    ~Gotot_TAC_Statement() {}
    virtual void make_rtl();
    virtual void print_stmt();
};

class Label_TAC_Statement : public TAC_Statement
{
    public:
    TAC_Opd * label;
    Label_TAC_Statement(TAC_Opd * label) : label(label) {}
    ~Label_TAC_Statement() {}
    virtual void make_rtl();
    virtual void print_stmt();
};

class Print_TAC_Statement : public TAC_Statement
{
    public:
    TAC_Opd * var;
    data_type var_dt;
    Print_TAC_Statement(TAC_Opd * var, data_type var_dt) : var(var), var_dt(var_dt) {}
    ~Print_TAC_Statement() {}
    virtual void make_rtl();
    virtual void print_stmt();
};

class Read_TAC_Statement : public TAC_Statement
{
    public:
    TAC_Opd * var;
    data_type var_dt;
    Read_TAC_Statement(TAC_Opd * var, data_type var_dt) : var(var), var_dt(var_dt) {}
    ~Read_TAC_Statement() {}
    virtual void make_rtl();
    virtual void print_stmt();
};

class Call_TAC_Statement : public TAC_Statement
{
    public:
    Call_TAC_Statement(vector<TAC_Opd*> actual_args, string func_name, TAC_Opd* res) : actual_args(actual_args), func_name(func_name), res(res) 
    {

    }
    ~Call_TAC_Statement();
    virtual void make_rtl();
    virtual void print_stmt();
    string func_name;
    vector<TAC_Opd*> actual_args;
    TAC_Opd* res = NULL;
};

class Return_TAC_Statement : public TAC_Statement
{
    public:
    Return_TAC_Statement(TAC_Opd* return_temp) : return_temp(return_temp) {}
    ~Return_TAC_Statement() {}
    virtual void make_rtl();
    virtual void print_stmt();
    TAC_Opd* return_temp;
};

class Code
{
    public:
    vector <TAC_Statement*> * stmt_list;
    Code() { stmt_list = new vector <TAC_Statement *> ;}
    ~Code();

    void append_statement(TAC_Statement * s) { stmt_list->push_back(s); }
    void append_list (Code * c);
    vector <TAC_Statement *> * get_list() { return stmt_list; }

    void print_code();
};


#endif