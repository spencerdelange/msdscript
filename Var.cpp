//
// Created by Spencer DeLange on 1/16/22.
//

#include "Var.h"
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
