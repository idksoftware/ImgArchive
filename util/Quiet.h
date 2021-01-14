#pragma once

class Quiet
{
	static bool m_quiet;
public:
	static void printf(const char* fmt, ...);
	static void setOn() { m_quiet; };
};

