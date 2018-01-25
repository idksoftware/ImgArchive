#include "AddressScope.h"
#include <vector>

class DataString {
	std::string m_day;
	std::string m_month;
	std::string m_year;
	std::string m_image;
	void init(const std::string &s);
public:
	DataString();
	DataString(const std::string &pattern);
	DataString(const char *d);
	int compare(DataString &d);
};

DataString::DataString() {
	m_day = '*';
	m_month = '*';
	m_year = '*';
	m_image = '*';
}

DataString::DataString(const std::string &s) {
	init(s);
}

DataString::DataString(const char *d) {
	std::string tmp = d;
	init(tmp);
}

std::string justify(std::string s) {
	std::string tmp = "0";
	if (s.length() == 1) {
		if (s[0] == '*') {
			return s;
		}
		tmp += s;
		return tmp;
	}
	return s;
}

void DataString::init(const std::string &s) {
	std::string delimiter = "/";
	if (s.find("-") != std::string::npos) {
		delimiter = "-";
	}
	
	size_t last = 0;
	size_t next = 0;
	std::string token[4] = { "*", "*", "*", "*" };
	int c = 0;
	while ((next = s.find(delimiter, last)) != std::string::npos) {
		token[c++] = s.substr(last, next-last);
		//printf("%s", token[c].c_str());
		last = next + 1;
	}
	token[c] = s.substr(last);
	if (delimiter == "-") {
		delimiter = "/";
		if ((next = token[c].find(delimiter)) != std::string::npos) {
			std::string tmp = token[c].substr(0, next);
			token[c + 1] = token[c].substr(next+1, token[c].length() - (next+1));
			token[c] = tmp;
		}
	}
	m_image = token[3];
	m_day = justify(token[2]);
	m_month = justify(token[1]);
	m_year = token[0];
	//printf("%s %s %s\n", token[0].c_str(), token[1].c_str(), token[2].c_str());

}

int DataString::compare(DataString &d) {
	int res = 0;
	if (m_year == "*" || d.m_year == "*" ) {
		return 0;
	} else if ((res = m_year.compare(d.m_year)) != 0) {
		return res;
	} else {
		if (m_month == "*" || d.m_month == "*") {
			return 0;
		}
		else if ((res = m_month.compare(d.m_month)) != 0) {
			return res;
		} else {
			if (m_day == "*" || d.m_day == "*") {
				return 0;
			} else if ((res = m_day.compare(d.m_day)) != 0) {
				return res;
			} else {
				if (m_image == "*") {
					return 0;
				} else if ((res = m_day.compare(d.m_day)) != 0) {
					return res;
				}
			}
		}
	}
	return 0;
}

class ScopeItem {
	DataString m_begin;
	DataString m_end;
	bool m_range;
	void init(std::string &pattern);
public:
	ScopeItem(const char *pattern);
	ScopeItem(std::string &pattern);
	const bool isInScope(DataString &d);
};

void ScopeItem::init(std::string &pattern) {
	// begin-2009
	std::string s = pattern;
	int pos = s.find('-');
	if (pos < 0) {
		m_begin = DataString(s);
		m_range = false;
	} else {
		std::string begin = s.substr(0, pos);
		m_begin = DataString(begin);
		std::string end = s.substr(pos+1, s.length() - (pos+1));
		m_end = DataString(end);
		m_range = true;
	}
	std::string begin = s.substr(0, pos);

}

ScopeItem::ScopeItem(std::string &pattern) {
	init(pattern);
}

ScopeItem::ScopeItem(const char *pattern) {
	// begin-2009
	std::string s = pattern;
	init(s);

}

const bool ScopeItem::isInScope(DataString &d) {
	
	if (m_range) {
		if (m_begin.compare(d) <= 0 && m_end.compare(d) >= 0) {
			return true;
		}
		
		return false;
	}
	if (m_begin.compare(d) == 0) {
		return true;
	}
	return false;
}

class TokenList : public std::vector<ScopeItem> {};

AddressScope::AddressScope()
{
	m_list = new TokenList;
}


AddressScope::~AddressScope()
{
	delete m_list;
}

/*
std::string remove(std::string &str)
{
	
	char *tmp = new char[str.length() + 1];
	char *s = tmp;
	if (str[0] = '{') {
		s++;
	}
	if (str[str.length - 1] = '}') {
		s[str.length - 1] = '\0';
	}
	
	return true;
}
*/
std::string removespace(std::string str)
{    
    int m = str.length();
    int i=0;
    while(i<m)
    {
        while(str[i] == 32) {
			str.erase(i,1);
			m--;
		}
        i++;
    }
	return str;
}
// Range: 2009-2015, 2009-end, begin-2009, 2009/08/16-2015
// Item: 2009/08/16, 2009/08/17,
bool AddressScope::isInScope(const char *d) {

	if (m_matchAll) return true;

	DataString date(d);
	for( TokenList::iterator iter = m_list->begin();  iter != m_list->end(); iter++) {
		ScopeItem &item = *iter;
		if (item.isInScope(date)) {
			return true;
		}
	}
	return false;
}

void AddressScope::scopeAll() {
	m_matchAll = true;
}


bool AddressScope::scope(const char *str) {
	m_matchAll = false;
	std::string tmp = str;
	std::string s = removespace(tmp);
	std::string delimiter = ",";

	size_t last = 0;
	size_t next = 0;
	std::string token;
	while ((next = s.find(delimiter, last)) != std::string::npos) {
		token = s.substr(last, next-last);
		//printf("%s", token.c_str());
		m_list->push_back(ScopeItem(token));
		last = next + 1;
	}
	token = s.substr(last);
	//printf("%s", token.c_str());
	m_list->push_back(ScopeItem(token));
	return true;
}

