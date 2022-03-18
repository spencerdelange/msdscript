//
// Created by Spencer DeLange on 3/18/22.
//

#ifndef MSDSCRIPT_ENV_H
#define MSDSCRIPT_ENV_H

#include <string>
#include "pointer.h"

using namespace std;

// Included so Val and Expr can be in method definitions
class Val;
class Expr;

// Class representing an expression environment
CLASS(Env) {
public:
    // Represents an empty environment for testing
    static PTR(Env) empty;
    // Searches the environment for the value of the given find_name str
    virtual PTR(Val) lookup(std::string find_name) = 0;
    // Satisfies 'virtual destructors' warning
    virtual ~Env();
};

// Env subclass representing an empty environment
class EmptyEnv : public Env {
public:
    // Constructs an empty environment
    EmptyEnv();
    // Throws a runtime error as there is no value in an empty environment
    PTR(Val) lookup(std::string find_name) override;
};

// Env subclass representing an extended environment
class ExtendedEnv : public Env {
public:
    // Name of the key for the value
    std::string name;
    // Value paired with the name
    PTR(Val) val;
    // Rest of the environment
    PTR(Env) rest;
    // Constructs an extended environment
    explicit ExtendedEnv(string name, PTR(Val) val, PTR(Env) env);
    // Searches the environment for the value of the given find_name str
    PTR(Val) lookup(std::string find_name) override;
};

#endif //MSDSCRIPT_ENV_H
