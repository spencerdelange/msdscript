//
// Created by Spencer DeLange on 1/16/22.
//

#include <iostream>
#include "Num.h"
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

