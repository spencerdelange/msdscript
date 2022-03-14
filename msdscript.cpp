//
// Created by Spencer DeLange on 1/11/22.
//

#include <sstream>
#include "msdscript.h"
#include "Expr.h"
#include "test.h"

int main(int argc, char* argv[]) {
    try{
        use_arguments(argc, argv);
        return 0;
    }
    catch(exception& e){
        cerr << e.what() << endl;
        return 1;
    }
}
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
                throw logic_error("Tests failed");
            break;
        } else if(argvArr[i] == "--interp"){
            istringstream str = getEntireInput(cin);
            string interpMe = parse_expr(str)->interp()->to_string();
            cout << interpMe << endl;
            break;
        } else if(argvArr[i] == "--print"){
            istringstream str = getEntireInput(cin);
            printExpr(parse_expr(str));
            break;
        } else if(argvArr[i] == "--pretty-print"){
            istringstream str = getEntireInput(cin);
            printPrettyExpr(parse_expr(str));
            break;
        } else{
            throw invalid_argument("Invalid command");
        }
    }
}
static istringstream getEntireInput(std::istream &in){

    string toReturn;
    string temp;
    while(std::getline(cin, temp)){
        toReturn+=temp;
    }

    istringstream str(toReturn);
    return str;
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
static PTR(Expr) parse_expr(std::istream &in) {
    PTR(Expr) e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        int currPos = (int)in.tellg();
        consume(in, '=');
        c = in.peek();
        if(c == '='){
            consume(in, '=');
            PTR(Expr) rhs = parse_expr(in);
            return NEW(EqExpr)(e, rhs);
        } else {
            in.seekg(currPos);
            return e;
        }
    } else
        return e;
}
PTR(Expr) parse_comparg(std::istream &in){
    PTR(Expr) e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_expr(in);
        return NEW(AddExpr)(e, rhs);
    } else
        return e;
}

PTR(Expr) parse_addend(std::istream &in){
    PTR(Expr) e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if(c == '*'){
        consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    } else
        return e;
}
PTR(Expr) parse_multicand(std::istream &in) {
    PTR(Expr) e;
    e = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');
        PTR(CallExpr) temp = NEW(CallExpr)(e, actual_arg);
        e = temp;
    }
    return e;
}
PTR(Expr) parse_inner(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    // if c is a number parse it
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    // if c is a parens parse the expr inside it
    else if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
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
        string keyword;
        in >> keyword;
        if(keyword == "_let")
            return parse_let(in);
        else if(keyword == "_if")
            return parse_if(in);
        else if(keyword == "_fun")
            return parse_fun(in);
        else if(keyword == "_false")
            return NEW(BoolExpr)(false);
        else if(keyword == "_true")
            return NEW(BoolExpr)(true);
        else
            throw std::runtime_error("invalid input: " + keyword);
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input: ");
    }
}
PTR(Expr) parse_num(std::istream &in) {
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
    return NEW(NumExpr)(n);
}
PTR(Expr) parse_if(std::istream &in){
    PTR(Expr) _if = parse_expr(in);

    skip_whitespace(in);
    if(!parse_keyword(in, "_then"))
        throw std::runtime_error("invalid input for '_if'");

    PTR(Expr) _then = parse_expr(in);

    skip_whitespace(in);
    if(!parse_keyword(in, "_else"))
        throw std::runtime_error("invalid input for '_if'");

    PTR(Expr) _else = parse_expr(in);
    return NEW(IfExpr)(_if, _then, _else);
}
PTR(Expr) parse_let(std::istream &in){
    PTR(Expr) e = parse_expr(in);
    PTR(VarExpr) lhs = CAST(VarExpr)(e);
    if(lhs == nullptr)
        throw std::runtime_error("invalid input for '_let'");

    skip_whitespace(in);
    if(!parse_keyword(in, "="))
        throw std::runtime_error("invalid input for '_let'");

    PTR(Expr) rhs = parse_expr(in);

    skip_whitespace(in);
    if(!parse_keyword(in, "_in"))
        throw std::runtime_error("invalid input for '_let'");
    PTR(Expr) body = parse_expr(in);
    return NEW(LetExpr)(lhs, rhs, body);
}
PTR(Expr) parse_fun(std::istream &in){
    PTR(Expr) e = parse_expr(in);
    PTR(VarExpr) actual_arg = CAST(VarExpr)(e);
    if(actual_arg == nullptr)
        throw std::runtime_error("invalid input for '_fun'");
    PTR(Expr) body = parse_expr(in);
    return NEW(FunExpr)(actual_arg, body);
}
PTR(Expr) parse_var(std::istream &in){
    string varName;
    char c;
    while (in.get(c)){
        varName += c;
        if(in.peek() == ' ' || in.peek() == ')' || in.peek() == '(')
            break;
    }
    return NEW(VarExpr)(varName);
}
PTR(Expr) parse_bool(std::istream &in){
    string boolStr;
    in >> boolStr;
    if(boolStr=="_true")
        return NEW(BoolExpr)(true);
    else
        return NEW(BoolExpr)(false);
}
bool parse_keyword(std::istream &in, const string& expected){
    string keyword;
    in >> keyword;
    return keyword == expected;
}
PTR(Expr) parse_str(std::string s){
    istringstream str(s);
    PTR(Expr) e = parse_expr(str);
    return e;
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