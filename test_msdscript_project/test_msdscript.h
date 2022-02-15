//
// Created by Spencer DeLange on 2/12/22.
//

#ifndef TEST_MSDSCRIPT_TEST_MSDSCRIPT_H
#define TEST_MSDSCRIPT_TEST_MSDSCRIPT_H

// Returns a string representing a random expression. If includeVar, include variables, else only include numbers.
std::string random_expr_string(bool includeVar);
// Returns a string representing a random variable. One char is used. 'x' and 'y' are favored.
std::string randomVar();
// Tests a single msdscript at the given path
void testSingleProgram(const char *msdscriptPath);
// Tests two msdscripts at the two given paths. Also tests them by comparing outputs
void testTwoPrograms(const char *msdscriptPath1, const char *msdscriptPath2);
// Checks the error code of a given ExecResult and prints an error message if it's an error
void checkErrCode(const ExecResult& result, const std::string& command);
// Compares two ExecResult objects to see if the output is the same.
void checkTwoProgramResults(std::string input, std::string test, const ExecResult& result1, const ExecResult& result2, const std::string& path1, const std::string& path2);
// Loops through the msdscripts with broken implementations
void testBrokenImplementations();

#endif //TEST_MSDSCRIPT_TEST_MSDSCRIPT_H
