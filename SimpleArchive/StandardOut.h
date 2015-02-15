#pragma once
class CStandardOut
{
	static CStandardOut *m_This;
	CStandardOut();
	bool isPrinting;
public:
	static CStandardOut &get();
	void PrintOut(const char *format);
	static void print(const char *format);
	~CStandardOut();
};

