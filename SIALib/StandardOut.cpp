#include <stdio.h>
#include "StandardOut.h"


CStandardOut::CStandardOut()
{
	isPrinting = true;
}


CStandardOut::~CStandardOut()
{
}

void CStandardOut::PrintOut(const char *format) {
	if (isPrinting) {
		printf(format);
	}
}

void CStandardOut::print(const char *format) {
	CStandardOut::get().PrintOut(format);
}

CStandardOut &CStandardOut::get() {
	if (m_This == 0) {
		m_This = new CStandardOut();
	}
	return *m_This;
}

CStandardOut *CStandardOut::m_This = 0;