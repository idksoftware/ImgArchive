#pragma once
//#include "CSVIndexVisitor.h"
#include "CSVIndexAction.h"
#include "HistoryEvent.h"
#include "ResultsPresentation.h"
#include "ResultsList.h"
#include "DBDefines.h"
#include "MetaType.h"

namespace simplearchive {

	class CSVTable {
	protected:
		std::string m_indexRoot;
		std::shared_ptr<MTTableSchema> m_mtTableSchema;
		std::shared_ptr<ResultsList> m_resultsList;
	public:
		CSVTable(std::shared_ptr<MTTableSchema> mtTableSchema) : m_mtTableSchema(mtTableSchema)
		{};
		~CSVTable() = default;

		bool scopedStatus(const char* scope);
	};

};
