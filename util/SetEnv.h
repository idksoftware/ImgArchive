/*
 * SetEnv.h
 *
 *  Created on: Oct 30, 2014
 *      Author: wzw7yn
 */

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
	void process();
};

} /* namespace simplearchive */
#endif /* SETENV_H_ */
