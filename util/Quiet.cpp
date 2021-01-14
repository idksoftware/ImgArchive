#include "Quiet.h"

#include <memory>
#include <string>
#include <stdarg.h>  // For va_start, etc.
#include <iostream>

bool Quiet::m_quiet = false;

void Quiet::printf(const char* fmt, ...)
{ 
	if (m_quiet) return;
	size_t final_n, n = (strlen(fmt) * 2); // Reserve two times as much as the length of the fmt_str //
	va_list ap;
	while (true)
	{
		//formatted.reset(new char[n]); // Wrap the plain char array into the unique_ptr
		auto formatted = std::make_unique<char[]>(n);

		va_start(ap, fmt);
		final_n = vsnprintf(formatted.get(), n, fmt, ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
		{
			n += abs(static_cast<int>(final_n - n + 1));
		}
		else
		{
			std::cout << formatted.get();
			break;
		}
	}
}
