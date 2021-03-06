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

#if !defined(SIADLL_h)
#define SIADLL_h

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the IDKFCDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IDKFCDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SIADLL_EXPORTS
#define SIADLL_API __declspec(dllexport)
#else
#define SIADLL_API __declspec(dllimport)
#endif
#include <memory>
#include "SIALib.h"
// This class is exported from the sia.dll
using namespace simplearchive;
class SIADLL_API SIADll
{
	SIALib m_sialib;
public:
	SIADll();
	~SIADll();
	int initalise();
	int complete();
	int checkin();

	bool Import();

	bool checkout(const char *filepath, const char *comment);

	bool checkin(const char *filepath, const char *comment);

	bool uncheckout(const char *filepath, const char *comment);

	bool view(const char *name);

	bool mirror(const char *name);

	bool archive();

};

#endif