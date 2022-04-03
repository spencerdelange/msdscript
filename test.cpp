//
// Created by Spencer DeLange on 1/16/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "test.h"
#include "Expr.h"
#include "msdscript.h"
#include "Val.h"
#include "Env.h"
#include "Step.h"

// Static test assistance method returning a string representing the value of a given string expression
static std::string run(const std::string& s){
    PTR(Expr) e = parse_str(s);
    PTR(Val) v = e->interp(Env::empty);
    return v->to_string();
}

int test(char* argv[]){
    return Catch::Session().run(1, argv);
}
TEST_CASE("equalstests") {
    PTR(NumExpr) n1 = NEW(NumExpr)(1);
    PTR(NumExpr) n2 = NEW(NumExpr)(2);
    PTR(NumExpr) n3 = NEW(NumExpr)(3);
    PTR(NumExpr) n4 = NEW(NumExpr)(4);
    PTR(AddExpr) a1 = NEW(AddExpr)(n1, n2);
    PTR(AddExpr) a2 = NEW(AddExpr)(n3, n4);
    PTR(MultExpr) m = NEW(MultExpr)(n1, n2);
    CHECK(n1->equals(n2) == false);
    CHECK(a1->equals(a2) == false);
    CHECK(a1->equals(a1) == true);
    CHECK(a1->equals(m) == false);
    CHECK(n1->equals(a1) == false);
    PTR(MultExpr) m1 = nullptr;
    CHECK(m->equals(m1) == false);
    PTR(VarExpr) v1 = nullptr;
    PTR(VarExpr) v2 = NEW(VarExpr)("x");
    CHECK(v2->equals(v1) == false);
    PTR(LetExpr) l1 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(2));
    PTR(LetExpr) l2 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(2));
    CHECK(v2->equals(l1) == false);
    CHECK(l1->equals(l2) == true);
    PTR(LetExpr) l3 = NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1), NEW(NumExpr)(2));
    CHECK(l1->equals(l3) == false);
    PTR(LetExpr) l4 = nullptr;
    CHECK(l3->equals(l4) == false);
}
TEST_CASE("interp tests") {
    PTR(NumExpr) n2 = NEW(NumExpr)(2);
    PTR(NumExpr) n3 = NEW(NumExpr)(3);
    PTR(AddExpr) a1 = NEW(AddExpr)(n2, n3);
    PTR(AddExpr) a2 = NEW(AddExpr)(n2, n3);
    PTR(MultExpr) m1 = NEW(MultExpr)(n2, n3);
    PTR(MultExpr) m2 = NEW(MultExpr)(a2, m1);
    CHECK(n2->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK(a1->interp(Env::empty)->equals(NEW(NumVal)(5)));
    CHECK(m1->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK(m2->interp(Env::empty)->equals(NEW(NumVal)(30)));
    PTR(VarExpr) v1 = NEW(VarExpr)("a");
    CHECK_THROWS_WITH(v1->interp(Env::empty), "free variable: a");
    PTR(LetExpr) l1 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(2));
    CHECK(l1->interp(Env::empty)->equals(NEW(NumVal)(2)));
    PTR(LetExpr) l2 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)));
    CHECK(l2->interp(Env::empty)->equals(NEW(NumVal)(7)));

    PTR(LetExpr) l3 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
    PTR(LetExpr) l4 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), l3);
    CHECK(l4->interp(Env::empty)->equals(NEW(NumVal)(8)));

    PTR(LetExpr) l5 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
    PTR(LetExpr) l6 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), l5);
    CHECK(l6->interp(Env::empty)->equals(NEW(NumVal)(7)));

    PTR(LetExpr) l7 = NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
    PTR(LetExpr) l8 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), l7);
    CHECK(l8->interp(Env::empty)->equals(NEW(NumVal)(6)));
}
TEST_CASE("subst tests"){
    PTR(VarExpr) v1 = NEW(VarExpr)("a");
    PTR(VarExpr) v2 = NEW(VarExpr)("a");
    PTR(VarExpr) v3 = NEW(VarExpr)("b");
    CHECK(v1->equals(v2));

    PTR(NumExpr) n1 = NEW(NumExpr)(3);
    PTR(VarExpr) v4 = NEW(VarExpr)("z");
    PTR(AddExpr) a1 = NEW(AddExpr)(v4, n1);
    CHECK(a1->equals(NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(3))));

    PTR(VarExpr) v5 = NEW(VarExpr)("x");
    PTR(NumExpr) n7 = NEW(NumExpr)(7);
    PTR(AddExpr) a2 = NEW(AddExpr)(v5, n7);
    PTR(VarExpr) v6 = NEW(VarExpr)("y");
    PTR(AddExpr) a3 = NEW(AddExpr)(v6, n7);
    CHECK(a2->equals(NEW(AddExpr)(v5, n7)));
    CHECK(a3->equals(NEW(AddExpr)(v6, n7)));
}
TEST_CASE("print"){
    PTR(NumExpr) n1 = NEW(NumExpr)(3);
    std::stringstream out1("");
    n1->print(out1);
    CHECK(out1.str() == "3");

    PTR(VarExpr) v1 = NEW(VarExpr)("var1");
    std::stringstream out2("");
    v1->print(out2);
    CHECK(out2.str() == "var1");

    PTR(AddExpr) a1 = NEW(AddExpr)(n1, v1);
    std::stringstream out3("");
    a1->print(out3);
    CHECK(out3.str() == "(3+var1)");

    PTR(MultExpr) m1 = NEW(MultExpr)(n1, v1);
    std::stringstream out4("");
    m1->print(out4);
    CHECK(out4.str() == "(3*var1)");

    PTR(MultExpr) m2 = NEW(MultExpr)(m1, n1);
    std::stringstream out5("");
    m2->print(out5);
    CHECK(out5.str() == "((3*var1)*3)");

    PTR(AddExpr) a2 = NEW(AddExpr)(n1, a1);
    std::stringstream out6("");
    a2->print(out6);
    CHECK(out6.str() == "(3+(3+var1))");

    CHECK(a2->to_string() == "(3+(3+var1))");

    PTR(LetExpr) l1 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(2));
    CHECK(l1->to_string() == "(_let x=1 _in 2)");

    PTR(LetExpr) l2 = NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)));
    CHECK(l2->to_string() == "(_let y=3 _in (y+2))");

    PTR(LetExpr) l3 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(l2, NEW(VarExpr)("x")));
    CHECK(l3->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");

}
TEST_CASE("pretty_print"){
    PTR(NumExpr) n3 = NEW(NumExpr)(3);
    CHECK(n3->pretty_to_string() == "3");

    PTR(VarExpr) v1 = NEW(VarExpr)("v1");
    CHECK(v1->pretty_to_string() == "v1");

    PTR(AddExpr) a1 = NEW(AddExpr)(n3, v1);
    CHECK(a1->pretty_to_string() == "3 + v1");

    PTR(AddExpr) a2 = NEW(AddExpr)(a1, n3);
    CHECK(a2->pretty_to_string() == "(3 + v1) + 3");

    PTR(MultExpr) m1 = NEW(MultExpr)(n3, v1);
    CHECK(m1->pretty_to_string() == "3 * v1");

    PTR(NumExpr) n1 = NEW(NumExpr)(1);
    PTR(NumExpr) n2 = NEW(NumExpr)(2);
    PTR(MultExpr) m2 = NEW(MultExpr)(n2, n3);
    PTR(AddExpr) a4 = NEW(AddExpr)(n1, m2);
    CHECK(a4->pretty_to_string() == "1 + 2 * 3");

    PTR(MultExpr) m3 = NEW(MultExpr)(n1, NEW(AddExpr)(n2, n3));
    CHECK(m3->pretty_to_string() == "1 * (2 + 3)");

    PTR(MultExpr) m4 = NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4));
    CHECK(m4->pretty_to_string() == "(2 * 3) * 4");

    PTR(MultExpr) m5 = NEW(MultExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4)));
    CHECK(m5->pretty_to_string() == "2 * 3 * 4");

    PTR(MultExpr) m6 = NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(1)));
    CHECK(m6->pretty_to_string() == "(3 * 2) * (2 + 1)");

    PTR(AddExpr) a5 = NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                              NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(7)));
    CHECK(a5->pretty_to_string() == "1 * 2 + 4 + 7");

    PTR(AddExpr) a7 = NEW(AddExpr)(NEW(AddExpr)(n1, n2), NEW(NumExpr)(3));
    CHECK(a7->pretty_to_string() == "(1 + 2) + 3");

    n3 = NEW(NumExpr)(3);
    PTR(AddExpr) a8 = NEW(AddExpr)(n1, NEW(AddExpr)(n2, n3));
    CHECK(a8->pretty_to_string() == "1 + 2 + 3");

    PTR(AddExpr) a9 = NEW(AddExpr)(a7, NEW(NumExpr)(4));
    CHECK(a9->pretty_to_string() == "((1 + 2) + 3) + 4");

    PTR(LetExpr) l1 = NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)));
    CHECK(l1->pretty_to_string() == "_let y = 3\n"
                                    "_in  y + 2");

    PTR(AddExpr) a6 = NEW(AddExpr)(NEW(NumExpr)(7), l1);
    CHECK(a6->pretty_to_string() == "7 + _let y = 3\n"
                                    "    _in  y + 2");

    PTR(LetExpr) l2 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")));
    CHECK(l2->pretty_to_string() == "_let x = 5\n"
                                    "_in  (_let y = 3\n"
                                    "      _in  y + 2) + x");

    PTR(MultExpr) m7 = NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(VarExpr)("x")));
    PTR(AddExpr) a10 = NEW(AddExpr)(m7, NEW(NumExpr)(1));
    CHECK(a10->pretty_to_string() == "5 * (_let x = 5\n"
                                     "     _in  x) + 1");
    CHECK(a10->interp(Env::empty)->equals(NEW(NumVal)(26)));

    PTR(AddExpr) a11 = NEW(AddExpr)(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1));
    CHECK(a11->pretty_to_string() == "(_let x = 5\n"
                                     " _in  x) + 1");

    PTR(MultExpr) m8 = NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))));
    CHECK(m8->pretty_to_string() == "5 * _let x = 5\n"
                                    "    _in  x + 1");
    CHECK(m8->interp(Env::empty)->equals(NEW(NumVal)(30)));
}

TEST_CASE("parse"){
    CHECK(parse_str("5")->interp(Env::empty)->equals(NEW(NumVal)(5)));
    CHECK(parse_str("-5")->interp(Env::empty)->equals(NEW(NumVal)(-5)));
    CHECK(parse_str("5 + 2")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("5 + -10")->interp(Env::empty)->equals(NEW(NumVal)((-5))));
    CHECK(parse_str("    5 + 2")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("4 * 2")->interp(Env::empty)->equals(NEW(NumVal)(8)));
    CHECK(parse_str("(4 * 2)")->interp(Env::empty)->equals(NEW(NumVal)(8)));
    CHECK(parse_str("2 * 2 + 3")->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("2 * (2 + 3)")->interp(Env::empty)->equals(NEW(NumVal)(10)));
    CHECK(parse_str("(4)")->interp(Env::empty)->equals(NEW(NumVal)(4)));

    parse_str("this")->pretty_to_string();

    CHECK(parse_str("this")->pretty_to_string() == "this");
    CHECK(parse_str("this    ")->pretty_to_string() == "this");
    CHECK(parse_str("     this")->pretty_to_string() == "this");

    CHECK(parse_str("this + that")->pretty_to_string() == "this + that");

    CHECK(parse_str("_let x = 4 _in x + -2")->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK(parse_str("_let x = 4 _in x + -2")->pretty_to_string() ==
          "_let x = 4\n_in  x + -2");
    CHECK(parse_str("_let x = 4 _in x + -2")->to_string() == "(_let x=4 _in (x+-2))");
    CHECK_THROWS_WITH(parse_str("_lt x = 4 _in x + -2")->interp(Env::empty), "invalid input: _lt");
}

TEST_CASE("Val Tests"){
    CHECK(run("1 + 2") == "3");
    CHECK((NEW(NumVal)(3))->equals(NEW(NumVal)(3)));
    CHECK((NEW(NumVal)(3))->equals(NEW(NumVal)(2)) == false);
    CHECK(run("1 + 2") != "2");
    CHECK(run("1 + -2") == "-1");
    CHECK((NEW(NumVal)(2))->to_string() == "2");
    PTR(NumVal) n1 = NEW(NumVal)(2);
    CHECK(n1->add_to(NEW(NumVal)(4))->equals(NEW(NumVal)(6)));
    CHECK(n1->mult_to(NEW(NumVal)(4))->equals(NEW(NumVal)(8)));
}

TEST_CASE("Refactor Tests"){
    CHECK(run("_let x = _true\n"
              "_in _if x\n"
              "    _then 1\n"
              "    _else 2") == "1");
    CHECK(run("2 == (1+1)") == "_true");
    CHECK(run("2 == 2") == "_true");
    CHECK(run("2 == 3") == "_false");
    CHECK((run("_let same = 1 == 2\n"
               "_in  _if 1 == 2\n"
               "     _then _false + 5\n"
               "     _else 88")) == "88");

    CHECK(parse_str("_let same = 1 == 2\n"
                    "_in  _if 1 == 2\n"
                    "     _then _false + 5\n"
                    "     _else 88")->pretty_to_string() == "_let same = 1 == 2\n"
                                                            "_in  _if 1 == 2\n"
                                                            "     _then _false + 5\n"
                                                            "     _else 88");
    CHECK_THROWS_WITH(run("_if 4 + 1\n"
                          "_then 2\n"
                          "_else 3"), "NumVal cannot be true or false");
}
TEST_CASE("Function tests"){
    CHECK(parse_str("_fun (x) x + 1")->to_string() == "(_fun (x) (x+1))");
    CHECK(parse_str("_fun(x)x + 1")->to_string() == "(_fun (x) (x+1))");
    CHECK(parse_str("_fun (x) x + 1")->pretty_to_string() == "_fun (x)\n"
                                                             "  x + 1");
    CHECK(parse_str("f(10)")->pretty_to_string() == "f(10)");
    CHECK(parse_str("f(10)")->to_string() == "f(10)");
    CHECK(parse_str("_let f = _fun (x) x + 1\n"
                    "_in f(10)")->to_string() == "(_let f=(_fun (x) (x+1)) _in f(10))");
    CHECK(parse_str("_let f = _fun (x) x + 1\n"
                    "_in f(10)")->pretty_to_string() == "_let f = _fun (x)\n"
                                                        "           x + 1\n"
                                                        "_in  f(10)");
    CHECK_THROWS_WITH(parse_str("f(10)")->interp(Env::empty), "free variable: f");
    CHECK(parse_str("f(10)")->equals(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10))));

    CHECK(parse_str("_let f = _fun (x) x + 1\n"
                    "_in f(10)")->interp(Env::empty)->equals(NEW(NumVal)(11)));
    CHECK(parse_str("_let factrl = _fun (factrl)\n"
                    "                _fun (x)\n"
                    "                  _if x == 1\n"
                    "                  _then 1\n"
                    "                  _else x * factrl(factrl)(x + -1)\n"
                    "_in  factrl(factrl)(10)"
    )->pretty_to_string() ==
          "_let factrl = _fun (factrl)\n"
          "                _fun (x)\n"
          "                  _if x == 1\n"
          "                  _then 1\n"
          "                  _else x * factrl(factrl)(x + -1)\n"
          "_in  factrl(factrl)(10)");
    CHECK(parse_str("_let f = _fun (g) \n"
                    "              g(5) \n"
                    "   _in _let g = _fun (y) \n"
                    "                  y + 2 \n"
                    "       _in f(g)")
                  ->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK(parse_str("_let x = 1\n"
                    "_in _let y = 2\n"
                    "_in _let z = 2\n"
                    "_in _if x == y\n"
                    "    _then 0\n"
                    "    _else _if (y == z) == _true\n"
                    "    _then 3\n"
                    "    _else 0")->interp(Env::empty)->equals(NEW(NumVal)(3)));
    CHECK(parse_str("_let x = _true\n"
                    "_in _if x\n"
                    "    _then 1\n"
                    "    _else 2")->interp(Env::empty)->equals(NEW(NumVal)(1)));
    CHECK(parse_str("_let x = 1 == 2\n"
                    "_in _if x\n"
                    "    _then 1\n"
                    "    _else 2")->interp(Env::empty)->equals(NEW(NumVal)(2)));
    string test = "_let x = 1 == 2 _in _if x   _then 1   _else 2";
}
TEST_CASE("interp_by_step tests"){
    CHECK( Step::interp_by_steps(parse_str("1"))->equals(NEW(NumVal)(1)) );


    string test = "_let x = 1 == 2\n"
                  "_in _if x\n"
                  "    _then 1\n"
                  "    _else 2";
    PTR(Expr) testExpr = parse_str(test);
    PTR(Val) testVal = Step::interp_by_steps(testExpr);
    testVal->equals(NEW(NumVal)(2));

    CHECK(Step::interp_by_steps(parse_str("_let x = 1 == 2\n"
                    "_in _if x\n"
                    "    _then 1\n"
                    "    _else 2"))->equals(NEW(NumVal)(2)));
    CHECK(Step::interp_by_steps(parse_str("_let x = _true\n"
                    "_in _if x\n"
                    "    _then 1\n"
                    "    _else 2"))->equals(NEW(NumVal)(1)));
    CHECK(Step::interp_by_steps(parse_str("_let f = _fun (x) x + 1\n"
                    "_in f(10)"))->equals(NEW(NumVal)(11)));
}






