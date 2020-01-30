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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#include "SACmdArgs.h"

namespace simplearchive {

	SACmdArgs *SACmdArgs::m_this = nullptr;
	bool SACmdArgs::m_isMakeNoChanges = false;
	bool SACmdArgs::m_isForceChanges = false;
	SACmdArgs::SACmdArgs()
	{
	}


	SACmdArgs::~SACmdArgs()
	{
	}

	SACmdArgs &SACmdArgs::get() {
		static SACmdArgs saCmdArgs;
		return saCmdArgs;
		//if (m_this == nullptr) {
		//	m_this = new SACmdArgs;
		//}
		//return *m_this;
	}

}