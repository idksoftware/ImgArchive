#include "cport.h"
#include <time.h>
#include <ctime>
#include <string>
#include <string.h>
#include <stdarg.h>

#define MAX_SIZE	256


errno_t gmtime_p(struct tm &tmTime, const time_t * time) {
	errno_t err = 1;
#ifdef _WIN32
	err = gmtime_s(&tmTime, time); 
#else
	struct tm tmp;
	struct tm *tmpp;
	tmpp = gmtime_r(time, &tmp);
	tmTime = *tmpp;
	if (tmpp == nullptr) {
		err = 0;
	}
#endif
	return err;
}


std::string ctime_p(const time_t *time)
{
	char buffer[MAX_SIZE];
#ifdef _WIN32
	errno_t err = ctime_s(buffer, MAX_SIZE, time);
	std::string str = buffer;
#else
	std::string str = ctime_r(time, buffer);
#endif
	return str; 
}

/*
 char *ctime_r(const time_t *timep, char *buf);

 */

errno_t localtime_p(struct tm &tmTime, const time_t * time)
{
	errno_t err = 1;
#ifdef _WIN32
	err = localtime_s(&tmTime, time); 
#else
	struct tm tmp;
	struct tm *tmpp;
	tmpp = localtime_r(time, &tmp);
	tmTime = *tmpp;
	if (tmpp == nullptr) {
		err = 0;
	}
#endif
	return err;
}

/*
 extern struct tm *localtime_r (const time_t *__restrict __timer,
			       struct tm *__restrict __tp) __THROW;
 */

errno_t fopen_p(FILE *&streamptr, const char *filename, const char *mode) {
	errno_t err = 1;
#ifdef _WIN32
	err = fopen_s(&streamptr, filename, mode);
#else
	FILE *fp = fopen(filename, mode);
	streamptr = fp;
#endif
	return err;
}

errno_t strcpy_p(char *strDestination, size_t numberOfElements, const char *strSource) {
	errno_t err = 1;
#ifdef _WIN32
	err = strcpy_s(strDestination, numberOfElements, strSource);
#else
	err = strcpy_p(strDestination, numberOfElements, strSource);
#endif
	return err;
}

errno_t strncpy_p(char *strDest, size_t numberOfElements, const char *strSource, size_t count) {
	errno_t err = 1;
#ifdef _WIN32
	err = strncpy_s(strDest, numberOfElements, strSource, count);
#else
	strDest = strncpy(strDest, strSource, count);
	if (strDest == nullptr) {
		err = 0;
	}
#endif
	return err;
}

void sprintf_p(std::string &s, const std::string fmt, ...)
{
	int n, size = 100;
	bool b = false;
	va_list marker;

	while (!b)
	{
		s.resize(size);
		va_start(marker, fmt);
		n = vsnprintf((char*)s.c_str(), size, fmt.c_str(), marker);
		va_end(marker);
		if ((n>0) && ((b = (n<size)) == true)) s.resize(n); else size *= 2;
	}
}
