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

////////////////////////////////
//// NumVal implementations ////
////////////////////////////////
NumVal::NumVal(int val){
    this->rep = val;
}
Expr *NumVal::to_expr() {
    return new NumExpr(rep);
}
bool NumVal::equals(Val *v) {
    NumVal *n = dynamic_cast<NumVal*>(v);
    if (n == nullptr)
        return false;
    else
        return (this->rep == n->rep);
}
Val *NumVal::add_to(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == nullptr) throw std::runtime_error("add of non-number");
    return new NumVal(rep + other_num->rep);
}
Val *NumVal::mult_to(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == nullptr) throw std::runtime_error("mult of non-number");
    return new NumVal(rep * other_num->rep);
}
bool NumVal::is_true(){
    throw std::runtime_error("NumVal cannot be true or false");
}
Val *NumVal::call(Val *actual_arg){
    throw std::runtime_error("NumVal is not a function that can be called");
}

/////////////////////////////////
//// BoolVal implementations ////
/////////////////////////////////
BoolVal::BoolVal(bool val){
    this->rep = val;
}
Expr *BoolVal::to_expr() {
    return new BoolExpr(rep);
}
bool BoolVal::equals(Val *v) {
    BoolVal *n = dynamic_cast<BoolVal*>(v);
    if (n == nullptr)
        return false;
    else
        return (this->rep == n->rep);
}
Val *BoolVal::add_to(Val *other_val) {
    throw std::runtime_error("cannot add a boolean value to another value");
}
Val *BoolVal::mult_to(Val *other_val) {
    throw std::runtime_error("cannot multiply a boolean value to another value");
}
bool BoolVal::is_true(){
    return this->rep;
}
Val *BoolVal::call(Val *actual_arg){
    throw std::runtime_error("BoolVal is not a function that can be called");
}

/////////////////////////////////
//// FunVal implementations ////
/////////////////////////////////
FunVal::FunVal(std::string formal_arg, Expr* body){
    this->formal_arg = formal_arg;
    this->body = body;
}
Expr *FunVal::to_expr() {
    return new FunExpr(new VarExpr(formal_arg), body);
}
bool FunVal::equals(Val *v) {
    FunVal *n = dynamic_cast<FunVal*>(v);
    if (n == nullptr)
        return false;
    else
        return (this->formal_arg == n->formal_arg && this->body->equals(n->body));
}
Val *FunVal::add_to(Val *other_val) {
    throw std::runtime_error("cannot add a function value to another value");
}
Val *FunVal::mult_to(Val *other_val) {
    throw std::runtime_error("cannot multiply a function value to another value");
}
bool FunVal::is_true(){
    throw std::runtime_error("FunVal cannot be true or false");
}
Val *FunVal::call(Val *actual_arg){
    // this is a function expr turned into its FunVal that is performing the call. Subst the actual_arg as a NumExpr into the formal_arg place and interp
    FunExpr* e1 = dynamic_cast<FunExpr *>(this->to_expr());
    Expr* r = e1->body->subst(e1->formal_arg->name, actual_arg->to_expr());
    return r->interp();
}