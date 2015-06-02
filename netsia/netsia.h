// netsia.h

#pragma once
#include <memory>
#include "SIALib.h"

using namespace System;
using namespace simplearchive;
namespace netsia {
	/*
	public ref class NetSIA
	{
	bool initalise();
	};
	*/


	// This class is exported from the sia.dll
	
	public ref class NetSIA
	{
		SIALib m_sialib;
	public:
		NetSIA();
		~NetSIA();
		int initalise();
		int complete();
		int checkin();
		/*
		bool Import();

		bool checkout(const char *filepath, const char *comment);

		bool checkin(const char *filepath, const char *comment);

		bool uncheckout(const char *filepath, const char *comment);

		bool view(const char *name);

		bool mirror(const char *name);

		bool archive();
		*/

	};
}