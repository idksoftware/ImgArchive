#pragma once

#include "ResultsPresentation.h"

namespace simplearchive {

	class MasterMatadataResultsPresentation : public ResultsPresentation {
	public:
		MasterMatadataResultsPresentation(ResultsList& resultsList) : ResultsPresentation(resultsList) {};
		~MasterMatadataResultsPresentation() = default;

		bool writeHuman() override;
		bool writeXML() override;
		bool writeCSV() override;
		bool writeJson() override;
		bool writeHtml() override;
	};

}; // namespace