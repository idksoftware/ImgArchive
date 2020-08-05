#pragma once

#include "ResultsPresentation.h"

namespace simplearchive {

	class ImageHistoryResultsPresentation : public ResultsPresentation {
	public:
		ImageHistoryResultsPresentation(ResultsList& resultsList) : ResultsPresentation(resultsList) {};
		~ImageHistoryResultsPresentation() = default;

		bool writeHuman() override;
		bool writeXML() override;
		bool writeCSV() override;
		bool writeJson() override;
		bool writeHtml() override;
	};

}; // namespace
