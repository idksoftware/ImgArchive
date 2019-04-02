#pragma once

namespace simplearchive {

	class ShowCommand
	{
		bool showSettup();
	public:
		ShowCommand();
		~ShowCommand();
		bool process(const char *str);
		
	};

} // simplearchive
