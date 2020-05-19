#pragma once

#include <string>
#include <memory>
class AddressSTokenList;
class AddressScope
{
	std::string m_pattern;
	std::shared_ptr<AddressSTokenList> m_list;
	bool m_matchAll;
public:
	AddressScope();
	~AddressScope();
	AddressScope(AddressScope& as);
	bool isInScope(const char *date);
	bool isImageInScope(const char *image);
	bool scope(const char *str);
	// match all
	void scopeAll();
};


/*
	This is to scope which versions need to be retrieved from the archive

	The format is as follows:
	0-@
	0-10
	3-@
	* or all
	@ or latest
	2,3,[5-@]

	0 is the original
	1 is the first derivation of the original
*/
class VersionTokenList;
class VersionScope
{
	std::string m_pattern;
	int m_maxVersion;
	std::unique_ptr<VersionTokenList> m_list;
	bool m_matchAll;
public:
	VersionScope(int maxVersion);
	~VersionScope();
	bool scope(const char *str);
	bool isInScope(int version);
	// match all
	void scopeAll();
};