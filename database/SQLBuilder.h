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

#ifndef SQLBUILDER_H_
#define SQLBUILDER_H_
#include <string>
#include <vector>
#include <sstream>

//namespace simplearchive {

class SQLBuilder {
public:
	SQLBuilder();
	virtual ~SQLBuilder();
	void start();
};

class SQLInsertBulder : public SQLBuilder {
	std::string sql1;
	std::string sql2;
	std::string sql3;
	std::string tokenStr;
	std::string valueStr;
	bool m_isUpdated;
public:
	SQLInsertBulder(const char *dbname) {
		sql1 = "insert into ";
		sql1 += dbname;
		sql1 += " (";
		sql2 = ") values (";
		sql3 = ");";
		m_isUpdated = false;
	}
	virtual ~SQLInsertBulder() {};
	void addfield(const char *field, const std::string &value) {
		if (field != NULL && field[0] != '\0') {
			if (!tokenStr.empty()) {
				tokenStr += ',';
			}
		}
		tokenStr += field;

		if (!valueStr.empty()) {
			valueStr += ',';
		}
		valueStr += "'" + value + "'";
		m_isUpdated = true;
	}
	std::string toString() {
		std::string sql(sql1 + tokenStr + sql2 + valueStr + sql3);
		return sql;
	}

	bool isUpdated() const {
		return m_isUpdated;
	}
};

class SQLUpdateBuilder : public SQLBuilder {
	std::string sql1;
	std::string sql2;
	std::string sql3;
	std::string tokenStr;
	std::string valueStr;

public:
	SQLUpdateBuilder(const char *dbname) {
		sql1 = "insert into ";
		sql1 += dbname;
		sql1 += " (";
		sql2 = ") values (";
		sql3 = ");";
	}
	virtual ~SQLUpdateBuilder() {};
	void addfield(const char *field, const std::string &value) {
		if (field != NULL && field[0] != '\0') {
			if (!tokenStr.empty()) {
				tokenStr += ',';
			}
		}
		tokenStr += field;

		if (!valueStr.empty()) {
			valueStr += ',';
		}
		valueStr += "'" + value + "'";
	}
	std::string toString() {
		std::string sql(sql1 + tokenStr + sql2 + valueStr + sql3);
		return sql;
	}
};

class SQLCreatBulder : public SQLBuilder {
	std::string sql1;
	std::string sql2;

	std::string tokenStr;
	std::string valueStr;
	bool m_first;
public:
	SQLCreatBulder(const char *dbname) {
		sql1 = "create table ";
		sql1 += dbname;
		sql1 += " (";
		sql2 = ");";
		m_first = true;
	}
	virtual ~SQLCreatBulder() {};
	void addfield(const char *field, const char *dtype, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "") {
		const std::string type(dtype);
		std::stringstream tmp;
		const char *endToken = m_first? "" : ",\n\t";
		if (arg1.empty() && arg2.empty() && arg3.empty()) {
			tmp << endToken << field << ' ' << type;
		} else if (!arg1.empty() && arg2.empty() && arg3.empty()) {
			tmp << endToken << field << ' ' << type << ' ' << arg1;
		} else if (!arg1.empty() && !arg2.empty() && arg3.empty()) {
			tmp << endToken << field << ' ' << type + ' ' << arg1 << ' ' << arg2;
		} else if (!arg1.empty() && !arg2.empty() && !arg3.empty()) {
			tmp << endToken <<field << ' ' << type << ' ' << arg1 << ' ' << arg2 << ' ' << arg2;
		}
		m_first = false;
		tokenStr += tmp.str();

	}

	void addfield(const char *field, const char *dtype, std::vector<std::string> &attributes) {
			const std::string type(dtype);
			std::stringstream tmp;
			const char *endToken = m_first? "" : ",\n\t";

			tmp << endToken << field << ' ' << type;
			if (attributes.size() != 0) {
				for (std::vector<std::string>::iterator ii = attributes.begin(); ii != attributes.end(); ++ii) {
					std::string i = *ii;
					tmp << " " << i;
				}
			}


			/*
			} else if (!arg1.empty() && arg2.empty() && arg3.empty()) {
				tmp << endToken << field << ' ' << type << ' ' << arg1;
			} else if (!arg1.empty() && !arg2.empty() && arg3.empty()) {
				tmp << endToken << field << ' ' << type + ' ' << arg1 << ' ' << arg2;
			} else if (!arg1.empty() && !arg2.empty() && !arg3.empty()) {
				tmp << endToken <<field << ' ' << type << ' ' << arg1 << ' ' << arg2 << ' ' << arg2;
			}
			*/
			m_first = false;
			tokenStr += tmp.str();

		}
	std::string toString() {
		std::string sql(sql1 + tokenStr + sql2);
		return sql;
	}
};
//} /* namespace simplearchive */
#endif /* SQLBUILDER_H_ */
