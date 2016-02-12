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

// sia.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "sia.h"
#include "SIALib.h"

int SIADll::complete() {
	return m_sialib.complete();
	
}

bool SIADll::Import() {
	return m_sialib.Import();
}

bool SIADll::checkout(const char *filepath, const char *comment) {
	return m_sialib.checkout(filepath, comment);
}

bool SIADll::checkin(const char *filepath, const char *comment) {
	return m_sialib.checkin(filepath, comment);
}

bool SIADll::uncheckout(const char *filepath, const char *comment) {
	return m_sialib.uncheckout(filepath, comment);
	

}

bool SIADll::view(const char *name) {
	return m_sialib.view(name);
	
}

bool SIADll::mirror(const char *name) {
	return m_sialib.mirror(name);
	
}
bool SIADll::archive() {
	//return m_sialib.archive();
	return true;
}