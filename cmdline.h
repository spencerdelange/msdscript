//
// Created by Spencer DeLange on 1/11/22.
//

#ifndef MSDSCRIPT_CMDLINE_H
#define MSDSCRIPT_CMDLINE_H

#include <cstdio>
#include <iostream>

using namespace std;
// calls the appropriate function based on the order and input given
void use_arguments(int argc, char* argv[]);
// displays the help message
void help();
// displays the results of the test
void test();

#endif