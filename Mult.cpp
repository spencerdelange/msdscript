//
// Created by Spencer DeLange on 1/16/22.
//

#include "Mult.h"
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Mult::equals(Expr *e) {
    Mult*m = dynamic_cast<Mult*>(e);
    if (m == nullptr)
        return false;
    else
        return (this->lhs == m->lhs
                && this->rhs == m->rhs);
}