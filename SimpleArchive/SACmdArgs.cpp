#include "SACmdArgs.h"

namespace simplearchive {

	SACmdArgs *SACmdArgs::m_this = nullptr;
	bool SACmdArgs::m_isMakeNoChanges = false;
	SACmdArgs::SACmdArgs()
	{
	}


	SACmdArgs::~SACmdArgs()
	{
	}

	SACmdArgs &SACmdArgs::get() {
		if (m_this == nullptr) {
			m_this = new SACmdArgs;
		}
		return *m_this;
	}

}