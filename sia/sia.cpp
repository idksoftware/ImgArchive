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
	return m_sialib.archive();
}