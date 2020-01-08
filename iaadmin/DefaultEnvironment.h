#pragma once

#include <string>

namespace simplearchive {

	class DefaultEnvironment
	{
		bool m_configured;
	public:
		DefaultEnvironment() = default;
		~DefaultEnvironment() = default;
		bool init();
	};

}