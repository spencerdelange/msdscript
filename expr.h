//
// Created by Spencer DeLange on 1/14/22.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H
#include <string>

// Class representing an expression
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
};

#endif //MSDSCRIPT_EXPR_H