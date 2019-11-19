#include <algorithm>
#include <vector>
#include "Base64.h"

static char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+_";

static int LookupDigits[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,           //gap: spc,!"#$%'()*
	62,                   // +
	-1, -1, -1,             // gap ,-.
	63,                   // /
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // 0-9
	-1, -1, -1,             // gap: :;<
	99,                   //  = (end padding)
	-1, -1, -1,             // gap: >?@
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,
	17,18,19,20,21,22,23,24,25, // A-Z
	-1, -1, -1, -1, -1, -1,    // gap: [\]^_`
	26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,
	43,44,45,46,47,48,49,50,51, // a-z    
	-1, -1, -1, -1,          // gap: {|}~ (and the rest...)
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

const char *Base64::decimal2base64(long dec) {
	std::vector<char> list;
	m_str.clear();
	int rem;
	if (dec > 0) {
		while (dec > 0) {
			rem = dec % 64;
			list.push_back(base64[rem]);
			dec = dec / 64;
		}
	}
	else {
		list.push_back(base64[0]);
	}
	std::reverse(list.begin(), list.end());
	for (auto i = list.begin(); i != list.end(); i++) {
		char c = *i;
		m_str += c;
	}
	return m_str.c_str();
}

int Base64::base64ToDecimal(std::string encoded)
{
	int ret = 0;
	unsigned char c = 0;
	unsigned int num;
	int len = encoded.length();
	for (unsigned int index = 0; index < encoded.length(); index++) {
		c = encoded[index];
		if (c >= sizeof(LookupDigits)) {
			return -1;
		}
		num = LookupDigits[c];
		if (num == -1) {
			return -1;
		}
		ret = (64 * ret) + num;
	}
	return ret;
}

static char base32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
