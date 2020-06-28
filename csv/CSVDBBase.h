#pragma once

#include <vector>
#include "CSVTable.h"

namespace simplearchive {

	class CSVDBBase : public std::vector<std::shared_ptr<CSVTable>> {
	public:
		CSVDBBase() = default;
		virtual ~CSVDBBase() = default;
	};

}