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

#ifndef SETENV_H_
#define SETENV_H_
#include <vector>
#include <string>

namespace simplearchive {

class EnvItem {
	std::string m_name;
	std::string m_value;
public:
	EnvItem(const char *name, const char *value) {
		m_name = name;
		m_value = value;
	}

	const std::string& getName() const {
		return m_name;
	}

	void setName(const std::string& name) {
		m_name = name;
	}

	const std::string& getValue() const {
		return m_value;
	}

	void setValue(const std::string& value) {
		m_value = value;
	}
};

class SetEnv : public std::vector<EnvItem> {
public:
	SetEnv();
	virtual ~SetEnv();
	bool process();
};

} /* namespace simplearchive */
#endif /* SETENV_H_ */
