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
#include "md5.h"
#include <gtest/gtest.h>


TEST(EnviromentVariableTest, TestGet)
{
	std::string input = "1234567890qwertyuiopasdfghjklzxcvbnm";
	std::string test = md5(input);
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

