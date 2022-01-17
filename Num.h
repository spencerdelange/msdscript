//
// Created by Spencer DeLange on 1/16/22.
//

#ifndef MSDSCRIPT_NUM_H
#define MSDSCRIPT_NUM_H

#include "expr.h"

// Expr subclass representing a number in an expression
class Num : public Expr {
public:
    int val;
    Num(int val);
    bool equals(Expr *e) override;
};


#endif //MSDSCRIPT_NUM_H
