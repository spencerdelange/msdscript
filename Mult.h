//
// Created by Spencer DeLange on 1/16/22.
//

#ifndef MSDSCRIPT_MULT_H
#define MSDSCRIPT_MULT_H

#include "expr.h"

// Expr subclass representing the multiplication of two expressions
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};


#endif //MSDSCRIPT_MULT_H
