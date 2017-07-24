#pragma once
#include <string>

namespace simplearchive {

	class ContentsLister
	{
		std::string m_MasterPath;
	public:
		ContentsLister(const char *MasterPath);
		~ContentsLister();

		bool List();
	};

}; // namespace

