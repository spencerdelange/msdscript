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
static Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        int currPos = (int)in.tellg();
        consume(in, '=');
        c = in.peek();
        if(c == '='){
            consume(in, '=');
            Expr *rhs = parse_expr(in);
            return new EqExpr(e, rhs);
        } else {
            in.seekg(currPos);
            return e;
        }
    } else
        return e;
}
Expr *parse_comparg(std::istream &in){
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new AddExpr(e, rhs);
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
        return new MultExpr(e, rhs);
    } else
        return e;
}
Expr *parse_multicand(std::istream &in) {
    Expr* e;
    e = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        Expr *actual_arg = parse_expr(in);
        consume(in, ')');
        CallExpr *temp = new CallExpr(e, actual_arg);
        e = temp;
    }
    return e;
}
Expr *parse_inner(std::istream &in) {
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
        string keyword;
        in >> keyword;
        if(keyword == "_let")
            return parse_let(in);
        else if(keyword == "_if")
            return parse_if(in);
        else if(keyword == "_fun")
            return parse_fun(in);
        else if(keyword == "_false")
            return new BoolExpr(false);
        else if(keyword == "_true")
            return new BoolExpr(true);
        else
            throw std::runtime_error("invalid input: " + keyword);
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input: ");
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
    return new NumExpr(n);
}
Expr *parse_if(std::istream &in){
    Expr* _if = parse_expr(in);

    skip_whitespace(in);
    if(!parse_keyword(in, "_then"))
        throw std::runtime_error("invalid input for '_if'");

    Expr* _then = parse_expr(in);

    skip_whitespace(in);
    if(!parse_keyword(in, "_else"))
        throw std::runtime_error("invalid input for '_if'");

    Expr* _else = parse_expr(in);
    return new IfExpr(_if, _then, _else);
}
Expr *parse_let(std::istream &in){
    Expr* e = parse_expr(in);
    VarExpr* lhs = dynamic_cast<VarExpr*>(e);
    if(lhs == nullptr)
        throw std::runtime_error("invalid input for '_let'");

    skip_whitespace(in);
    if(!parse_keyword(in, "="))
        throw std::runtime_error("invalid input for '_let'");

    Expr* rhs = parse_expr(in);

    skip_whitespace(in);
    if(!parse_keyword(in, "_in"))
        throw std::runtime_error("invalid input for '_let'");
    Expr* body = parse_expr(in);
    return new LetExpr(lhs, rhs, body);
}
Expr *parse_fun(std::istream &in){
    Expr* e = parse_expr(in);
    VarExpr* actual_arg = dynamic_cast<VarExpr*>(e);
    if(actual_arg == nullptr)
        throw std::runtime_error("invalid input for '_fun'");
    Expr* body = parse_expr(in);
    return new FunExpr(actual_arg, body);
}
Expr *parse_var(std::istream &in){
    string varName;
    char c;
    while (in.get(c)){
        varName += c;
        if(in.peek() == ' ' || in.peek() == ')' || in.peek() == '(')
            break;
    }
    return new VarExpr(varName);
}
Expr *parse_bool(std::istream &in){
    string boolStr;
    in >> boolStr;
    if(boolStr=="_true")
        return new BoolExpr(true);
    else
        return new BoolExpr(false);
}
bool parse_keyword(std::istream &in, const string& expected){
    string keyword;
    in >> keyword;
    return keyword == expected;
}
Expr* parse_str(std::string s){
    istringstream str(s);
    Expr* e = parse_expr(str);
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