#include "ErrorCode.h"
#include <iostream>

namespace simplearchive {
	// Move the codes above errno codes 
	const int ErrorCode::errorOffset = 200;
	IMGA_ERROR ErrorCode::m_errorCode = IMGA_ERROR::SUCCESS;

	const char *ErrorCode::toString(IMGA_ERROR value) {
		const char* s = 0;
#define IMGA_ERROR_DEF( identifier, name )  case IMGA_ERROR::identifier: s = name; break;
		switch (value)
		{
			IMGA_ERRORS_DEF
		}
#undef IMGA_ERRORS_DEF
		return s;
	}


	std::ostream& operator<<(std::ostream& out, const IMGA_ERROR value)
	{
		const char* s = ErrorCode::toString(value);
		return out << s;
	}

	const int ErrorCode::toInt(IMGA_ERROR type) {
		int t = static_cast<int>(type);
		return (t + errorOffset);
	}

	ErrorCode::ErrorCode()
	{
	}


	ErrorCode::~ErrorCode()
	{
	}


	

	const IMGA_ERROR ErrorCode::getErrorCode() {
		return m_errorCode;
	}

	void ErrorCode::setErrorCode(IMGA_ERROR errorCode) {
		m_errorCode = errorCode;
	}

	
} /* simplearchive */