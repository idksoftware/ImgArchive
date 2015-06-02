// This is the main DLL file.

#include "stdafx.h"

#include "netsia.h"
#include "SIALib.h"
using namespace simplearchive;

namespace netsia {
	bool NetSIA::initalise() {
		SIALib sialib;
		sialib.initalise();
		return true;
	}
}