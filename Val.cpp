//
// Created by Spencer DeLange on 2/13/22.
//

#include "Val.h"
#include "Expr.h"

std::string Val::to_string() {
    return this->to_expr()->to_string();
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

NumVal::NumVal(int val){
    this->rep = val;
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



