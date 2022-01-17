//
// Created by Spencer DeLange on 1/16/22.
//

#include "Add.h"
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Add::equals(Expr *e) {
    Add*a = dynamic_cast<Add*>(e);
    if (a == nullptr)
        return false;
    else
        return (this->lhs == a->lhs
                && this->rhs == a->rhs);
}
