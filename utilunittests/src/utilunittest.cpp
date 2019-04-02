//============================================================================
// Name        : utilunittest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <string>
#include <iostream>
#include <string>
#include "SAUtils.h"
#include <gtest/gtest.h>

TEST(EnviromentVariableTest, TestSet)
{
	std::string test = "IAIN";
	bool all = true;
	if (SAUtils::SetEnv("MYNAME", test.c_str(), all) == false) {

	}
}

TEST(EnviromentVariableTest, TestGet)
{
	std::string test = "MYNAME";
	bool all = true;
	test = SAUtils::GetEnv(test, all);
	EXPECT_EQ("IAIN", test);
	//if (test.length() == 0) {
	//
	//}
}


int main(int argc, char* argv[])
{

	::testing::InitGoogleTest(&argc,argv);

   return RUN_ALL_TESTS();
}

