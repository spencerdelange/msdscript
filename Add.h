//
// Created by Spencer DeLange on 1/16/22.
//

#ifndef MSDSCRIPT_ADD_H
#define MSDSCRIPT_ADD_H

#include "expr.h"

// Expr subclass representing the addition of two expressions
class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};


#endif //MSDSCRIPT_ADD_H
