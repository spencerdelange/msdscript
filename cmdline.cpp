//
// Created by Spencer DeLange on 1/11/22.
//

#include "cmdline.h"

void use_arguments(int argc, char* argv[]){
    string argvArr[argc];
    for(int i = 0; i < argc; i++){
        argvArr[i] = argv[i];
    }
    
    bool tested = false;
    for(int i = 1; i < argc; i++){
        if(argvArr[i] == "--help"){
            help();
        } else if(argvArr[i] == "--test"){
            if(tested){
                cerr << "ERROR: --test has already been completed" << endl;
                exit(1);
            } else{
                tested = true;
                test();
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
void test(){
    cout << "Tests passed." << endl;
}