#include "CSVTable.h"

#include "CSVIndexVisitor.h"

namespace simplearchive {

	bool CSVTable::scopedStatus(const char* scope)
	{
		CSVIndexVisitor indexVisitor(std::make_shared<CSVIndexAction>(m_mtTableSchema), m_indexRoot.c_str());
		if (!indexVisitor.setScope(scope)) {
			return false;
		}
		indexVisitor.process();
		m_resultsList = indexVisitor.getIndexAction().getResultsList();
		//ArchiveHistoryResultsPresentation resultsPresentation(*m_resultsList);
		//resultsPresentation.writeHuman();
		//resultsPresentation.writeXML();
		return true;
	}
};