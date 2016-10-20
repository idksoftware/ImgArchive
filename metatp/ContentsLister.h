#pragma once
#include <string>

namespace simplearchive {

	class ContentsLister
	{
		std::string m_shadowPath;
	public:
		ContentsLister(const char *shadowPath);
		~ContentsLister();

		bool List();
	};

}; // namespace

