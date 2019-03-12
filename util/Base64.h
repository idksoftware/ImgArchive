#pragma once

#include <string>

class Base64 {
	std::string m_str;
	unsigned long m_int;
public:
	Base64() {};
	virtual ~Base64() {};
	const char *decimal2base64(long dec);
	int base64ToDecimal(std::string encoded);
	const char *getBase64() {
		return m_str.c_str();
	}
};

