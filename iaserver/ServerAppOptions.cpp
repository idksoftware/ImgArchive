/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "ConfigReader.h"
#include "ServerAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "SIAArgvParser.h"
#include "Environment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace CommandLineProcessing;
namespace simplearchive {


std::string AppOptions::m_name;
AppOptions::CommandMode AppOptions::m_commandMode = AppOptions::CommandMode::CM_Unknown;
std::string AppOptions::m_comment;
std::string AppOptions::m_imageAddress;
std::string AppOptions::m_homePath;
std::string AppOptions::m_workspacePath;
std::string AppOptions::m_masterPath;
std::string AppOptions::m_configPath;
std::string AppOptions::m_derivativePath;
std::string AppOptions::m_repositoryPath;
std::string AppOptions::m_cataloguePath;
std::string AppOptions::m_picturePath;
std::string AppOptions::m_wwwImagePath;
std::string AppOptions::m_configOption;
std::string AppOptions::m_configValue;
std::string AppOptions::m_optionBlock;


AppOptions::VerifyOperation AppOptions::m_verifyOperation = AppOptions::VerifyOperation::Both;
AppOptions::ShowOperation AppOptions::m_showOperation = AppOptions::ShowOperation::CheckedOut;
bool AppOptions::m_repair = false;
bool AppOptions::m_users = true;
bool AppOptions::m_setHomeEnv = true;

AppOptions &AppOptions::get() {
	static AppOptions _this;
	return _this;
}

AppOptions::AppOptions() {
	m_configured = true;
}




AppOptions::~AppOptions() {
	// TODO Auto-generated destructor stub
}


AppOptions::CommandMode AppOptions::getCommandMode() {
	return m_commandMode;
}

bool AppOptions::setCommandMode(const char *modeString) {
	std::string mode = modeString;

	
	m_commandMode = CommandMode::CM_Unknown;
	return false;
}

void AppOptions::setCommandMode(const AppOptions::CommandMode mode) {
	m_commandMode = mode;
}

bool AppOptions::isAllUsers() {
	return m_users;
}

void AppOptions::setAllUsers(bool allUsers) {
	m_users = allUsers;
}

void AppOptions::setConfigOption(const char* option)
{
	m_configOption = option;
}

void AppOptions::setConfigValue(const char* value)
{
	m_configValue = value;
}

void AppOptions::setConfigOptionBlock(const char* optionBlock)
{
	m_optionBlock = optionBlock;
}

const char *AppOptions::getName() {
	return m_name.c_str();
}

void AppOptions::setName(const char *name) {
	m_name = name;
}

void AppOptions::setHomePath(const char *homePath) {
	m_homePath = homePath;
}

void AppOptions::setWorkspacePath(const char *workspacePath) {
	m_workspacePath = workspacePath;
}
void AppOptions::setDerivativePath(const char *path) {
	m_derivativePath = path;
}

void AppOptions::setCataloguePath(const char *path) {
	m_cataloguePath = path;
}

void AppOptions::setWWWImagePath(const char* wwwImagePath)
{
	m_wwwImagePath = wwwImagePath;
}

void AppOptions::setPicturePath(const char* picturePath)
{
	m_picturePath = picturePath;
}

void AppOptions::setRepositoryPath(const char *path) {
	m_repositoryPath = path;
}

void AppOptions::setMasterPath(const char *masterPath) {
	m_masterPath = masterPath;
}

void AppOptions::setConfigPath(const char *configPath) {
	m_configPath = configPath;
}

const char *AppOptions::getComment() {
	return m_comment.c_str();
}
const char *AppOptions::getImageAddress() {
	return m_imageAddress.c_str();
}

const char *AppOptions::getHomePath() {
	return m_homePath.c_str();
}

const char *AppOptions::getWorkspacePath() {
	return m_workspacePath.c_str();
}
const char *AppOptions::getDerivativePath() {
	return m_derivativePath.c_str();
}
const char *AppOptions::getCataloguePath() {
	return m_cataloguePath.c_str();
}
const char* AppOptions::getWWWImagePath()
{
	return m_wwwImagePath.c_str();
	
}
const char* AppOptions::getPicturePath()
{
	return m_picturePath.c_str();
}
const char* AppOptions::getConfigOption()
{	
	return m_configOption.c_str();
}
const char* AppOptions::getConfigValue()
{
	return m_configValue.c_str();
}
const char* AppOptions::getConfigOptionBlock()
{
	return m_optionBlock.c_str();
}
const char *AppOptions::getRepositoryPath() {
	return m_repositoryPath.c_str();
}
const char *AppOptions::getMasterPath() {
	return m_masterPath.c_str();
}
const char *AppOptions::getConfigPath() {
	return m_configPath.c_str();
}

void AppOptions::setHomeEnv(bool b) {
	m_setHomeEnv = b;
}
} /* namespace simplearchive */
