#pragma once

#include "ResultsPresentation.h"

namespace simplearchive {

	class SystemHistoryResultsPresentation : public ResultsPresentation {
	public:
		SystemHistoryResultsPresentation(ResultsList& resultsList) : ResultsPresentation(resultsList) {};
		~SystemHistoryResultsPresentation() = default;

		bool writeHuman() override;
		bool writeXML() override;
		bool writeCSV() override;
		bool writeJson() override;
		bool writeHtml() override;
	};

}; // namespace