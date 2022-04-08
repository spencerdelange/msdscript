//
// Created by Spencer DeLange on 2/13/22.
//

#include "Val.h"
#include "Expr.h"
#include "Env.h"
#include "Step.h"

/////////////////////////////
//// Val implementations ////
/////////////////////////////
Val::~Val() { }

////////////////////////////////
//// NumVal implementations ////
////////////////////////////////
NumVal::NumVal(int rep){
    this->rep = rep;
}
bool NumVal::equals(PTR(Val) v) {
    PTR(NumVal) n = CAST(NumVal)(v);
    if (n == nullptr)
        return false;
    else
        return (this->rep == n->rep);
}
PTR(Val) NumVal::add_to(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr) throw std::runtime_error("add of non-number");
    return NEW(NumVal)(this->rep + other_num->rep);
}
PTR(Val) NumVal::mult_to(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr) throw std::runtime_error("mult of non-number");
    return NEW(NumVal)(rep * other_num->rep);
}
bool NumVal::is_true(){
    throw std::runtime_error("NumVal cannot be true or false");
}
PTR(Val) NumVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("NumVal is not a function that can be called");
}
void NumVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    throw runtime_error("num Cannot call_step");
}
std::string NumVal::to_string() {
    return std::to_string(rep);
}

/////////////////////////////////
//// BoolVal implementations ////
/////////////////////////////////
BoolVal::BoolVal(bool val){
    this->rep = val;
}
bool BoolVal::equals(PTR(Val) v) {
    PTR(BoolVal) n = CAST(BoolVal)(v);
    if (n == nullptr)
        return false;
    else
        return (this->rep == n->rep);
}
PTR(Val) BoolVal::add_to(PTR(Val) other_val) {
    throw std::runtime_error("cannot add a boolean value to another value");
}
PTR(Val) BoolVal::mult_to(PTR(Val) other_val) {
    throw std::runtime_error("cannot multiply a boolean value to another value");
}
bool BoolVal::is_true(){
    return this->rep;
}
PTR(Val) BoolVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("BoolVal is not a function that can be called");
}
void BoolVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    throw runtime_error("bool Cannot call_step");
}

std::string BoolVal::to_string() {
    if(rep)
        return "_true";
    else
        return "_false";
}

/////////////////////////////////
//// FunVal implementations ////
/////////////////////////////////
FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env){
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}
bool FunVal::equals(PTR(Val) v) {
    PTR(FunVal) n = CAST(FunVal)(v);
    if (n == nullptr)
        return false;
    else
        return (this->formal_arg == n->formal_arg && this->body->equals(n->body));
}
PTR(Val) FunVal::add_to(PTR(Val) other_val) {
    throw std::runtime_error("cannot add a function value to another value");
}
PTR(Val) FunVal::mult_to(PTR(Val) other_val) {
    throw std::runtime_error("cannot multiply a function value to another value");
}
bool FunVal::is_true(){
    throw std::runtime_error("FunVal cannot be true or false");
}
PTR(Val) FunVal::call(PTR(Val) actual_arg){
    return body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
}
void FunVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(formal_arg, actual_arg_val, env);
    Step::cont = rest;
}

std::string FunVal::to_string() {
    return "[function]";
}
