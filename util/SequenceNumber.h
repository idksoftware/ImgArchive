#pragma once
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>

class Base32 {
	std::string m_str;
	unsigned long m_int;
	void padToBase32(std::string &str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), 'A');
	}
public:
	Base32() {};
	virtual ~Base32() {};
	const char *decimal2Base32(long dec, int padding = 0);
	int Base32ToDecimal(std::string encoded);
	const char *getBase32() {
		return m_str.c_str();
	}
};



class ImageSequence {
protected:
	std::string m_str;
	int m_int;
	std::string m_name;
	std::string m_path;

	void padDec(std::string &str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), '0');
	}
	void padToBase32(std::string &str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), 'A');
	}
	bool isNumber(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}
public:
	ImageSequence() = default;
	virtual ~ImageSequence() = default;
	virtual const char *toSequence() = 0;
	bool options() {};
};

class NumberSequence : public ImageSequence {
	
public:
	NumberSequence(int, int, int, int dec, const char *) {
		m_int = dec;  
	};
	NumberSequence(std::string& encoded) {
		m_str = encoded;
	}
	~NumberSequence() = default;
	const char *toSequence();
	int toDecimal();
};

class DateSequence : public ImageSequence {
	int m_year;
	int m_month;
	int m_day;
	int getNumber(const std::string& filenane);
	std::string toString();
	public:
		DateSequence(int y, int m, int d, long dec, const char *path);
		DateSequence(std::string& encoded);
		~DateSequence() = default;
		const char *toSequence();
		int toDecimal();
		int toDate();
};

/*
Unknown,
UseDate,
UseNumber,
NoRename,
*/
class NoRenameSequence : public ImageSequence {
	
public:
	NoRenameSequence(const char *name, const char *path) {
		m_name = name;
		m_path = path;
	}
	~NoRenameSequence() = default;
	const char *toSequence();
};
