#pragma once

#include <string>
class TokenList;
class AddressScope
{
	std::string m_pattern;
	TokenList *m_list;
	bool m_matchAll;
public:
	AddressScope();
	~AddressScope();
	bool isInScope(const char *date);
	bool scope(const char *str);
	// match all
	void scopeAll();
};

