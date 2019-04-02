#pragma once

namespace simplearchive {

class DayIndexer
{
public:
	DayIndexer();
	~DayIndexer();
	bool validate(const char *root, const char *yymmdd);
	bool createPath(const char *root, const char *yymmdd);
};

} // namespace simplearchive
