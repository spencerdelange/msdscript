//
// Created by Spencer DeLange on 1/11/22.
//

#include "msdscript.h"
#include "expr.h"
#include "catch.h"
#include "test.h"

void use_arguments(int argc, char* argv[]){
    string argvArr[argc];
    for(int i = 0; i < argc; i++){
        argvArr[i] = argv[i];
    }
    
    bool tested = false;
    for(int i = 1; i < argc; i++){
        // Options for command line below
        if(argvArr[i] == "--help"){
            help();
        } else if(argvArr[i] == "--test"){
            if(tested){
                cerr << "ERROR: --test has already been completed" << endl;
                exit(1);
            } else{
                tested = true;
                if(test(argv) != 0){
                    exit(1);
                }
            }
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
            "\t--test"
         << endl;
    exit(0);
}