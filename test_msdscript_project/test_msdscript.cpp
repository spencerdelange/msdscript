//
// Created by Spencer DeLange on 2/12/22.
//

#include <cstdio>
#include <iostream>
#include <ctime>

#include "exec.h"
#include "test_msdscript.h"

using namespace std;

int main(int argc, char *argv[]) {
    testBrokenImplementations();

    srand(time(nullptr));
    if(argc == 1 || argc > 3){
        cout << "Enter a path to an msdscript program to test implementation." << endl;
        cout << "Enter 2 paths to msdscript programs to test implementations against each other" << endl;
        return 1;
    } else if(argc == 2){
        testSingleProgram(argv[1]);
        return 0;
    }
    else if(argc == 3){
        testTwoPrograms(argv[1], argv[2]);
        return 0;
    }
    return 0;
}
void testBrokenImplementations(){
    string basePath = "./test_msdscript_project/msdscripts/msdscript";
    string brokenProgPaths[8];

    for(int i = 0; i < 8; i++){
        brokenProgPaths[i] = basePath+to_string(i);
    }
    for(int i = 0; i < 8; i++){
        testSingleProgram(const_cast<char*>(brokenProgPaths[i].c_str()));
        testTwoPrograms("./msdscript", const_cast<char*>(brokenProgPaths[i].c_str()));
    }
}
void testSingleProgram(const char *msdscriptPath) {
    const char * const interp_argv[] = { msdscriptPath, "--interp" };
    const char * const print_argv[] = { msdscriptPath, "--print" };
    const char * const pretty_print_argv[] = { msdscriptPath, "--pretty-print" };

    for (int i = 0; i < 10; i++) {
        ExecResult interp_result = exec_program(2, interp_argv, random_expr_string(false));
        ExecResult print_result = exec_program(2, print_argv, random_expr_string(true));
        ExecResult pretty_print_result = exec_program(2, pretty_print_argv, random_expr_string(true));

        checkErrCode(interp_result, "--interp");
        checkErrCode(print_result, "--print");
        checkErrCode(pretty_print_result, "--pretty-print");
    }
}
void testTwoPrograms(const char *msdscriptPath1, const char *msdscriptPath2) {
    const char * const interp_argv1[] = { msdscriptPath1, "--interp" };
    const char * const print_argv1[] = { msdscriptPath1, "--print" };
    const char * const pretty_print_argv1[] = { msdscriptPath1, "--pretty-print" };

    const char * const interp_argv2[] = { msdscriptPath2, "--interp" };
    const char * const print_argv2[] = { msdscriptPath2, "--print" };
    const char * const pretty_print_argv2[] = { msdscriptPath2, "--pretty-print" };
    
    for (int i = 0; i < 1; i++) {
        string interpInput = random_expr_string(false);
        string printsInput = random_expr_string(true);

        ExecResult interp_result1 = exec_program(2, interp_argv1, interpInput);
        ExecResult print_result1 = exec_program(2, print_argv1, printsInput);
        ExecResult pretty_print_result1 = exec_program(2, pretty_print_argv1, printsInput);

        ExecResult interp_result2 = exec_program(2, interp_argv2, interpInput);
        ExecResult print_result2 = exec_program(2, print_argv2, printsInput);
        ExecResult pretty_print_result2 = exec_program(2, pretty_print_argv2, printsInput);

        checkErrCode(interp_result1, "--interp");
        checkErrCode(print_result1, "--print");
        checkErrCode(pretty_print_result1, "--pretty-print");
        checkErrCode(interp_result2, "--interp");
        checkErrCode(print_result2, "--print");
        checkErrCode(pretty_print_result2, "--pretty-print");

        checkTwoProgramResults(interpInput,"--interp", interp_result1, interp_result2, interp_argv1[0], interp_argv2[0]);
        checkTwoProgramResults(printsInput,"--print", print_result1, print_result2, print_argv1[0], print_argv2[0]);
        checkTwoProgramResults(printsInput, "--pretty-print", pretty_print_result1, pretty_print_result2, pretty_print_argv1[0], pretty_print_argv2[0]);
    }
}
void checkTwoProgramResults(std::string input, std::string test, const ExecResult& result1, const ExecResult& result2, const std::string& path1, const std::string& path2) {
    if(result1.out != result2.out){
        std::cerr << "msdscript results do not match in: " << test << " with input: " << input << endl;
        std::cerr << "Program one path: " << path1 << endl;
        std::cerr << "Program one output: \n" << result1.out << endl;
        std::cerr << "Program two path: " << path2 << endl;
        std::cerr << "Program two output: \n" << result2.out << endl;
    }
}
void checkErrCode(const ExecResult& result, const std::string& command){
    if(result.exit_code != 0){
        std::cerr << command << " non-zero exit: " << result.exit_code << endl;
        std::cerr << "in: " << result << endl;
    }
}
std::string random_expr_string(bool includeVar) {
    int num = rand() % 10;
    if (num <= 5){
        return std::to_string(rand());
    } else if(num == 6){
        if(includeVar){
            return randomVar();
        } else {
            return std::to_string(rand());
        }
    } else if(num == 7){
        return random_expr_string(includeVar) + " * " + random_expr_string(includeVar);
    } else if(num == 8){
        return random_expr_string(includeVar) + " + " + random_expr_string(includeVar);
    } else if(num == 9){
        return " _let " + randomVar() + " = " + random_expr_string(includeVar) + " _in " + random_expr_string(includeVar);
    }
    return "";
}
std::string randomVar(){
    int num = rand() % 5;
    if(num == 0 || num == 1){
        return "x";
    } else if(num == 2 || num == 3){
        return "y";
    } else {
        string toReturn;
        toReturn += rand() % 26 + 'a';
        return toReturn;
    }
}