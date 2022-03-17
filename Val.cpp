//
// Created by Spencer DeLange on 2/13/22.
//

#include "Val.h"
#include "Expr.h"

/////////////////////////////
//// Val implementations ////
/////////////////////////////
std::string Val::to_string() {
    return this->to_expr()->to_string();
}

Val::~Val() { }

////////////////////////////////
//// NumVal implementations ////
////////////////////////////////
NumVal::NumVal(int val){
    this->rep = val;
}
PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr)(rep);
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
    return NEW(NumVal)(rep + other_num->rep);
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

/////////////////////////////////
//// BoolVal implementations ////
/////////////////////////////////
BoolVal::BoolVal(bool val){
    this->rep = val;
}
PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(rep);
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

/////////////////////////////////
//// FunVal implementations ////
/////////////////////////////////
FunVal::FunVal(std::string formal_arg, PTR(Expr) body){
    this->formal_arg = formal_arg;
    this->body = body;
}
PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(NEW(VarExpr)(formal_arg), body);
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
    // this is a function expr turned into its FunVal that is performing the call. Subst the actual_arg as a NumExpr into the formal_arg place and interp
    PTR(Expr) r = this->body->subst(formal_arg, actual_arg->to_expr());
    return r->interp();
}