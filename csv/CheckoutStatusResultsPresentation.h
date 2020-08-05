#pragma once
#include "ResultsPresentation.h"

namespace simplearchive {

	class CheckoutStatusResultsPresentation : public ResultsPresentation {
	public:
		CheckoutStatusResultsPresentation(ResultsList& resultsList) : ResultsPresentation(resultsList) {};
		~CheckoutStatusResultsPresentation() = default;

		bool writeHuman() override;
		bool writeXML() override;
		bool writeCSV() override;
		bool writeJson() override;
		bool writeHtml() override;
	};

}; // simplearchive

