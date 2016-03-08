#include "gtest/gtest.h"
#include "SIALib.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "CatalogManager.h"
#include "CreateArchive.h"

#define UNITTEST 1

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif
using namespace simplearchive;



CreateArchive createArchive;

SIALib siaLib;

class SIALibTest : public ::testing::Test {
	bool initalise();
	bool initTest();
protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	SIALibTest() {
		// You can do set-up work for each test here.
	}

	virtual ~SIALibTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
		initTest();
		initalise();
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};





/*
TEST_F(SIALibTest, initTest) {
	ASSERT_EQ(true, initTest());

}


TEST(SIALibTest, initalise) {
	ASSERT_EQ(true, initalise());
}
*/


TEST_F(SIALibTest, import) {
	
	ASSERT_EQ(true, siaLib.Import());
}
TEST_F(SIALibTest, checkout) {
	ASSERT_EQ(true, siaLib.checkout("2013-11-10/PB100362.JPG", "CHECK-OUT"));
}

TEST_F(SIALibTest, checkin) {
	ASSERT_EQ(true, siaLib.checkin("2013-11-10/PB100362.JPG", "CHECK-IN"));
}

TEST_F(SIALibTest, inituncheckout) {
	ASSERT_EQ(true, siaLib.checkout("2013-11-10/PB100363.JPG", "CHECK-OUT"));
}

TEST_F(SIALibTest, uncheckout) {
	ASSERT_EQ(true, siaLib.uncheckout("2013-11-10/PB100363.JPG", "UNCHECK-OUT"));
}

TEST_F(SIALibTest, catalogMakeList) {

	CAppConfig &config = CAppConfig::get();
	CatalogManager::setPaths("C:/Users/Iain/SIAImages/Catalog", config.getShadowPath());
	CatalogManager catalogManager = CatalogManager::get();
	catalogManager.makeList();
}

TEST_F(SIALibTest, catalogAddFile) {

	CAppConfig &config = CAppConfig::get();
	CatalogManager::setPaths("C:/Users/Iain/SIAImages/Catalog", config.getShadowPath());
	CatalogManager catalogManager = CatalogManager::get();
	catalogManager.addFile("2013-10-11", "15atest1.jpg");
}

bool SIALibTest::initalise() {
	

	if (siaLib.initalise() < 0) {
		return false;
	}
	return true;
}



bool SIALibTest::initTest() {
	const std::string key = "SA_HOME";
	std::string temp = SAUtils::GetEnvironment(key);
	std::string homePath = temp;
	if (homePath.empty() == true) {
		return false;
	}
	if (createArchive.all(temp.c_str()) == false) {
		return false;
	}
	CAppConfig &config = CAppConfig::get();
	ConfigReader configReader;
	std::string configfile = homePath + "/config/" + "config.dat";
	if (configReader.read(configfile.c_str(), config) == false) {
		return false;
	}
	config.setHomePath(homePath.c_str());
	return true;
}