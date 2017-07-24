#pragma once
#include <string>
#include <time.h>
#include <stdio.h>
#ifndef _WIN32
#include <errno.h>
#endif

#ifdef _WIN32
errno_t fopen_p(FILE *&streamptr, const char *filename, const char *mode);

errno_t strcpy_p(char *strDestination, size_t numberOfElements, const char *strSource);
errno_t strncpy_p(char *strDest, size_t numberOfElements, const char *strSource, size_t count);
errno_t gmtime_p(struct tm &gmt, const time_t *s);
std::string ctime_p(const time_t *time);
errno_t localtime_p(struct tm &lmt, const time_t * time);

#else
#define errno_t int

errno_t fopen_p(FILE *&streamptr, const char *filename, const char *mode);

errno_t strcpy_p(char *strDestination, int numberOfElements, const char *strSource);
errno_t strncpy_p(char *strDest, size_t numberOfElements, const char *strSource, size_t count);
errno_t gmtime_p(struct tm &gmt, const time_t *s);
std::string ctime_p(const time_t *time);
errno_t localtime_p(struct tm &lmt, const time_t * time);

#endif
void sprintf_p(std::string &s, const std::string fmt, ...);
// getenv

