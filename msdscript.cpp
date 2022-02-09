//
// Created by Spencer DeLange on 1/11/22.
//

#include <sstream>
#include "msdscript.h"
#include "Expr.h"
#include "test.h"

void use_arguments(int argc, char* argv[]){
    string argvArr[argc];
    for(int i = 0; i < argc; i++){
        argvArr[i] = argv[i];
    }
    for(int i = 1; i < argc; i++){
        if(argvArr[i] == "--help"){
            help();
        } else if(argvArr[i] == "--test"){
            if(test(argv) != 0)
                exit(1);
            exit(0);
        } else if(argvArr[i] == "--interp"){
            cout << parse_expr(cin)->interp() << endl;
            exit(0);
        } else if(argvArr[i] == "--print"){
            printExpr(parse_expr(cin));
            exit(0);
        } else if(argvArr[i] == "--pretty-print"){
            printPrettyExpr(parse_expr(cin));
            exit(0);
        } else{
            cerr << "ERROR: invalid command" << endl;
            exit(1);
        }
    }
}
void help(){
    cout << "Welcome to MSDScript!\n"
            "Available commands:\n"
            "\t--help\n"
            "\t--interp\n"
            "\t--print\n"
            "\t--pretty-print\n"
            "\t--test"
         << endl;
    exit(0);
}
static Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    } else
        return e;
}
Expr *parse_addend(std::istream &in){
    Expr* e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if(c == '*'){
        consume(in, '*');
        Expr* rhs = parse_addend(in);
        return new Mult(e, rhs);
    } else
        return e;
}
Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    // if c is a number parse it
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    // if c is a parens parse the expr inside it
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    // if c is alpha, parse it as a variable
    } else if(isalpha(c)){
        return parse_var(in);
    // if c is an underscore, parse it as a let
    } else if(c == '_'){
        return parse_let(in);
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}
Expr *parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n * 10 + (c - '0');
        } else
            break;
    }
    if (negative)
        n = -n;
    return new Num(n);
}
Expr *parse_let(std::istream &in){
    parse_keyword(in, "_let");
    // verify that the next expression is a variable
    Expr* e = parse_expr(in);
    Var* lhs = dynamic_cast<Var*>(e);
    if(lhs == nullptr)
        throw std::runtime_error("invalid input");

    skip_whitespace(in);
    parse_keyword(in, "=");

    Expr* rhs = parse_expr(in);

    skip_whitespace(in);
    parse_keyword(in, "_in");
    Expr* body = parse_expr(in);
    return new Let(lhs, rhs, body);
}
Expr *parse_var(std::istream &in){
    string varName;
    in >> varName;
    return new Var(varName);
}
void parse_keyword(std::istream &in, const string& expected){
    string keyword;
    in >> keyword;
    if(keyword != expected)
        throw std::runtime_error("invalid input for '_let'");
}
Expr* parse_str(std::string s){
    istringstream str(s);
    return parse_expr(str);
}
static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch");
}
static void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}