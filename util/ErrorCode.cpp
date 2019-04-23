#include "ErrorCode.h"
#include <iostream>

namespace simplearchive {
	// Move the codes above errno codes 
	const int ErrorCode::errorOffset = 200;
	SIA_ERROR ErrorCode::m_errorCode = SIA_ERROR::SUCCESS;

	const char *ErrorCode::toString(SIA_ERROR value) {
		const char* s = 0;
#define SIA_ERROR_DEF( identifier, name )  case SIA_ERROR::identifier: s = name; break;
		switch (value)
		{
			SIA_ERRORS_DEF
		}
#undef SIA_ERRORS_DEF
		return s;
	}


	std::ostream& operator<<(std::ostream& out, const SIA_ERROR value)
	{
		const char* s = ErrorCode::toString(value);
		return out << s;
	}

	const int ErrorCode::toInt(SIA_ERROR type) {
		int t = static_cast<int>(type);
		return (t + errorOffset);
	}

	ErrorCode::ErrorCode()
	{
	}


	ErrorCode::~ErrorCode()
	{
	}


	

	const SIA_ERROR ErrorCode::getErrorCode() {
		return m_errorCode;
	}

	void ErrorCode::setErrorCode(SIA_ERROR errorCode) {
		m_errorCode = errorCode;
	}

	
} /* simplearchive */