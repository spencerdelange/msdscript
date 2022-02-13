//
// Created by Spencer DeLange on 2/12/22.
//

#ifndef TEST_MSDSCRIPT_TEST_MSDSCRIPT_H
#define TEST_MSDSCRIPT_TEST_MSDSCRIPT_H


class test_msdscript {

};

std::string random_expr_string(bool includeVar);
std::string randomVar();
void testSingleProgram(const char *msdscriptPath);
void testTwoPrograms(const char *msdscriptPath1, const char *msdscriptPath2);
void checkErrCode(const ExecResult& result, const std::string& command);
void checkTwoProgramResults(const ExecResult& result1, const ExecResult& result2, const std::string& path1, const std::string& path2);
void testBrokenImplementations();

#endif //TEST_MSDSCRIPT_TEST_MSDSCRIPT_H
