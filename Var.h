//
// Created by Spencer DeLange on 1/16/22.
//

#ifndef MSDSCRIPT_VAR_H
#define MSDSCRIPT_VAR_H

#include "expr.h"

// Expr subclass representing a variable in an expression
class Var : public Expr {
public:
    std::string name;
    Var(std::string name);
    bool equals(Expr *e) override;

};


#endif //MSDSCRIPT_VAR_H
