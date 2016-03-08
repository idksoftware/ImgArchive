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

#ifndef CUNQUENAME_H_
#define CUNQUENAME_H_

namespace simplearchive {

class CUnqueName {
	typedef enum {
		RoundBracket, 	// (7654)
		AngleBracket,	// <7654>
		Underscore		// _7654
	} StandardType;

	typedef enum {
		Hex,
		Num
	} NumberingType;

	typedef enum {
		Front,
		Back
	} Position;

	StandardType m_standardType;
	NumberingType m_numberingType;
	Position m_position;
	int m_countPos;		// Count into the filename.
	std::string m_pre;  // pre string
	std::string m_post; // post string
	int m_dupNameCount;

public:
	CUnqueName();
	virtual ~CUnqueName();

	const char *make(const char *name);

	void setCountPos(int countPos) {
		m_countPos = countPos;
	}

	int getDupNameCount() const {
		return m_dupNameCount;
	}

	void setNumberingType(NumberingType numberingType) {
		m_numberingType = numberingType;
	}

	void setPosition(Position position) {
		m_position = position;
	}

	void setPost(const std::string& post) {
		m_post = post;
	}

	void setPre(const std::string& pre) {
		m_pre = pre;
	}

	NumberingType getNumberingType() const {
		return m_numberingType;
	}


	Position getPosition() const {
		return m_position;
	}


	StandardType getStandardType() const {
		return m_standardType;
	}

	void setStandardType(StandardType standardType) {
		m_standardType = standardType;
	}
};

} /* namespace simplearchive */
#endif /* CUNQUENAME_H_ */
