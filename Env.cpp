//
// Created by Spencer DeLange on 3/18/22.
//

#include "Env.h"
#include "Expr.h"
#include "Val.h"

/////////////////////////////
//// Env implementations ////
/////////////////////////////
PTR(Env) Env::empty = NEW(EmptyEnv)();

Env::~Env() { }

//////////////////////////////////
//// EmptyEnv implementations ////
//////////////////////////////////
PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

EmptyEnv::EmptyEnv() = default;

/////////////////////////////////////
//// ExtendedEnv implementations ////
/////////////////////////////////////
ExtendedEnv::ExtendedEnv(string name, PTR(Val) val, PTR(Env) env){
    this->name = name;
    this->val = val;
    rest = env;
}
PTR(Val) ExtendedEnv::lookup(string find_name) {
    if (find_name == name)
        return val;
    else
        return rest->lookup(find_name);
}
