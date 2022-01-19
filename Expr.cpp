//
// Created by Spencer DeLange on 1/16/22.
//

#include "Expr.h"

// Num implementations
Num::Num(int val) {
    this->val = val;
}
bool Num::equals(Expr *e) {
    Num *n = dynamic_cast<Num*>(e);
    if (n == nullptr)
        return false;
    else
        return (this->val == n->val);
}
int Num::interp() {
    return val;
}
bool Num::has_variable() {
    return false;
}
Expr *Num::subst(std::string to_replace, Expr *substitute_expr) {
    return this;
}

// Var implementations
Var::Var(std::string name){
    this->name = name;
}
bool Var::equals(Expr *e){
    Var *v = dynamic_cast<Var*>(e);
    if (v == nullptr)
        return false;
    else
        return (this->name == v->name);
}
int Var::interp() {
    throw std::runtime_error("Error: a variable has no value.");
}
bool Var::has_variable() {
    return true;
}
Expr *Var::subst(std::string to_replace, Expr* substitute_expr) {
    if(this->name == to_replace){
        return substitute_expr;
    } else{
        return this;
    }
}

// Add implementations
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Add::equals(Expr *e) {
    Add*a = dynamic_cast<Add*>(e);
    if (a == nullptr)
        return false;
    else
        return (this->lhs->equals(a->lhs)
                && this->rhs->equals(a->rhs));
}
int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}
bool Add::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}
Expr *Add::subst(std::string to_replace, Expr *substitute_expr) {
    Add* temp = new Add(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
    return temp;
}

// Mult implementations
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Mult::equals(Expr *e) {
    Mult* m = dynamic_cast<Mult*>(e);
    if (m == nullptr)
        return false;
    else
        return (this->lhs->equals(m->lhs)
                && this->rhs->equals(m->rhs));
}
int Mult::interp() {
    return lhs->interp() * rhs->interp();
}
bool Mult::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
}
Expr *Mult::subst(std::string to_replace, Expr* substitute_expr) {
    return new Mult(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
}
