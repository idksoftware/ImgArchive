
#include <vector>
#include "Base32.h"

static char base32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

static int LookupDigits[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,           //gap: spc,!"#$%'()*
	-1,                   // +
	-1, -1, -1,             // gap ,-.
	-1,                   // /
	-1, -1, 26, 27, 28, 29, 30, 31, -1, -1, // 0-9
	-1, -1, -1,             // gap: :;<
	-1,                   //  = (end padding)
	-1, -1, -1,             // gap: >?@
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,
	17,18,19,20,21,22,23,24,25, // A-Z
	-1, -1, -1, -1, -1, -1,    // gap: [\]^_`
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1, // a-z    
	-1, -1, -1, -1,          // gap: {|}~ (and the rest...)
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};



const char* Base32::decimal2Base32(long dec, int padding) {
	std::vector<char> list;
	m_str.clear();
	int rem = 0;
	if (dec > 0) {
		while (dec > 0) {
			rem = dec % 32;
			list.push_back(base32[rem]);
			dec = dec / 32;
		}
	}
	else {
		list.push_back(base32[0]);
	}
	std::reverse(list.begin(), list.end());
	for (auto i = list.begin(); i != list.end(); i++) {
		const char c = *i;
		m_str += c;
	}
	if (padding != 0) {
		padToBase32(m_str, padding);
	}
	return m_str.c_str();
}

int Base32::Base32ToDecimal(std::string encoded)
{
	int ret = 0;
	unsigned char c = 0;
	unsigned int num = 0;
	const int len = encoded.length();
	for (unsigned int index = 0; index < encoded.length(); index++) {
		c = encoded[index];
		if (c >= sizeof(LookupDigits)) {
			return -1;
		}
		num = LookupDigits[c];
		if (num == -1) {
			return -1;
		}
		ret = (32 * ret) + num;
	}
	return ret;
}

static char base32hex[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

static int LookupDigitsHex[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,           //gap: spc,!"#$%'()*
	-1,                   // +
	-1, -1, -1,             // gap ,-.
	-1,                   // /
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // 0-9
	-1, -1, -1,             // gap: :;<
	-1,                   //  = (end padding)
	-1, -1, -1,             // gap: >?@
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,20,21,22,23,24,25,26,
	27,28,29,30,31, -1, -1, -1, -1, // A-Z
	-1, -1, -1, -1, -1, -1,    // gap: [\]^_`
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1, // a-z    
	-1, -1, -1, -1,          // gap: {|}~ (and the rest...)
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};



const char* Base32Hex::decimal2Base32(long dec, int padding) {
	std::vector<char> list;
	m_str.clear();
	
	int rem = 0;
	if (dec > 0) {
		while (dec > 0) {
			rem = dec % 32;
			list.push_back(base32hex[rem]);
			dec = dec / 32;
		}
	}
	else {
		list.push_back(base32hex[0]);
	}
	
	std::reverse(list.begin(), list.end());
	for (auto i = list.begin(); i != list.end(); i++) {
		const char c = *i;
		m_str += c;
	}
	if (padding != 0) {
		padToBase32(m_str, padding);
	}
	return m_str.c_str();
}

int Base32Hex::Base32ToDecimal(std::string encoded)
{
	int ret = 0;
	unsigned char c = 0;
	unsigned int num = 0;
	const int len = encoded.length();
	for (unsigned int index = 0; index < encoded.length(); index++) {
		c = encoded[index];
		if (c >= sizeof(LookupDigitsHex)) {
			return -1;
		}
		num = LookupDigitsHex[c];
		if (num == -1) {
			return -1;
		}
		ret = (32 * ret) + num;
	}
	return ret;
}
