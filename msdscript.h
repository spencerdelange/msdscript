//
// Created by Spencer DeLange on 1/11/22.
//

#ifndef MSDSCRIPT_MSDSCRIPT_H
#define MSDSCRIPT_MSDSCRIPT_H

#include <cstdio>
#include <iostream>
#include "Expr.h"
#include "Val.h"

using namespace std;
// calls the appropriate function based on the order and input given
void use_arguments(int argc, char* argv[]);
// displays the help message
void help();
// When given an istream containing a number, returns an Expr representation of that number
Expr *parse_num(std::istream &in);
// When given a string, returns an Expr representation of that number
Expr *parse_str(std::string s);
// When given an istream containing a comparg (addend or addend + comparg), return an Expr representing the comparg
Expr *parse_comparg(std::istream &in);
// When given an istream containing an addend (multicand or multicand * addend), return an Expr representing the addend
Expr *parse_addend(std::istream &in);
// When given an istream containing a multicand (number, expr in parens, variable or LetExpr), return an Expr representing the multicand
Expr *parse_multicand(std::istream &in);
// When given an istream containing a variable, return an Expr representing the variable
Expr *parse_var(std::istream &in);
// When given an istream containing a LetExpr, return an Expr representing the LetExpr
Expr *parse_let(std::istream &in);
// When given an istream containing a IfExpr, return an Expr representing the IfExpr
Expr *parse_if(std::istream &in);
// When given an istream containing a BoolExpr, return an Expr representing the BoolExpr
Expr *parse_bool(std::istream &in);
// Returns whether a keyword from in matches the expected keyword
bool parse_keyword(std::istream &in, const string& expected);
// Returns an Expr when given a std::string describing the Expr
static Expr *parse_expr(std::istream &in);
// Gets the next int in the input stream, advancing the stream forward. If the int isn't what is expected, an error message is displayed.
static void consume(std::istream &in, int expect);
// Skips the stream forward until it encounters the first non-whitespace.
static void skip_whitespace(std::istream &in);
// Returns a string of the entire user intput. Waits until ctrl+d or end of file is reached
static istringstream getEntireInput(std::istream &in);

#endif