#pragma once
#include <string>

class Base32 {
	std::string m_str;
	unsigned long m_int;
	void padToBase32(std::string& str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), 'A');
	}
public:
	Base32() {};
	virtual ~Base32() {};
	const char* toBase32(long dec, int padding = 0);
	int toDecimal(std::string encoded);
	const char* getBase32() {
		return m_str.c_str();
	}
};

class Base32Hex {
	std::string m_str;
	unsigned long m_int;
	void padToBase32(std::string& str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), '0');
	}
public:
	Base32Hex() {};
	virtual ~Base32Hex() {};
	const char* toBase32(long dec, int padding = 0);
	int toDecimal(std::string encoded);
	const char* getBase32() {
		return m_str.c_str();
	}
};
