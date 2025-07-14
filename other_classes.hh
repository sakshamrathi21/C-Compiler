#ifndef OTHER_CLASSES_HH
#define OTHER_CLASSES_HH

#include<vector>
#include<utility>
#include<string>
#include<map>
#include "types.hh"
#include "tac.hh"
using namespace std;

extern void throw_error_and_exit(string error_msg);
extern map<string, TAC_Opd*> return_label_opd;
extern map<string, int> global_symtab;

struct vec_var_type {
    vector<pair<int, string*>> entries;
    vec_var_type() = default;
    vec_var_type(string * name);
    vec_var_type(string * name, int type);
    void add_name (string * name);
    void add_name_and_type (string * name, int type);
    void set_type_to_all (int type);
    void append_another (vec_var_type * other);
};

struct func_signature {
    string func_name;
    data_type dt;
    map<string, int> params;
    vector<int> param_types_in_order;
    vec_var_type* param_list;
    vec_var_type* local_var_list = new vec_var_type();
    func_signature(data_type dt, string* func_name, vec_var_type* param_list) : dt(dt), func_name(*func_name), param_list(param_list) {
        for(const auto& var : param_list->entries) {
            if (params.count(*(var.second))) {
                throw_error_and_exit("Variable is declared twice in the same scope");
            }
            params[*(var.second)] = var.first;
            param_types_in_order.push_back(var.first);
        }
        if (dt != VOID__H && !return_label_opd.count(*func_name)) {
            return_label_opd[*func_name] = new Label_TAC_Opd();
        }
    }
    func_signature(data_type dt, string* func_name) : dt(dt), func_name(*func_name), param_list(new vec_var_type()) {
        if (dt != VOID__H && !return_label_opd.count(*func_name)) {
            return_label_opd[*func_name] = new Label_TAC_Opd();
        }
    }
    bool operator == (const func_signature& other) {
        if (other.func_name != func_name) return false;
        if (other.dt != dt) return false;
        if (other.param_types_in_order.size() != param_types_in_order.size()) return false;
        for (int i = 0 ; i < param_types_in_order.size() ; i ++)
        {
            if (param_types_in_order[i] != other.param_types_in_order[i]) return false;
        }
        return true;
    }
};

extern vector<pair<func_signature*, pair<bool,bool>>> all_func_signatures;
extern map<string, func_signature*> name_to_signature;

void push_func_to_vector(func_signature* curr_func, bool isDecl);

vector<pair<int, string*>> get_local_var_list(string curr_func);
vector<pair<int, string*>> get_formal_var_list(string curr_func);
data_type get_return_dt(string curr_func);
bool whether_void(string curr_func);

#endif