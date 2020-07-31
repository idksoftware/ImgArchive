#pragma once
#include "CSVIndexVisitor.h"
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
		std::shared_ptr<MTTable> m_mtTablePartition;
		std::shared_ptr<MTTableSchema> m_mtTableSchema;
		std::shared_ptr<CSVIndexVisitorBase> m_csvIndexVisitor;
		std::shared_ptr<ResultsList> m_resultsList;

		

		bool add(const char* yearDay, MTRow& row);

		bool save(ExifDateTime& dateAdded, MTRow& row);

	public:
		CSVTable(std::shared_ptr<MTTableSchema> mtTableSchema, std::shared_ptr<MTTable> mtTablePartition, std::shared_ptr<CSVIndexVisitorBase> csvIndexVisitor) :
			m_mtTableSchema    { mtTableSchema },
			m_mtTablePartition { mtTablePartition },
			m_csvIndexVisitor  { csvIndexVisitor }
		{};
		~CSVTable() = default;
		const char* getName();

		bool select(const char* scope);
		bool update(const char* option, const char* value);

		std::shared_ptr<ResultsList> getResults() {
			return m_resultsList;
		}

		void setPath(const char* indexRoot);
	};

	

};
