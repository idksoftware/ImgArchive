#pragma once
#include <memory>
#include "CSVIndexVisitor.h"
#include "CSVIndexAction.h"
#include "CSVTable.h"
#include "HistoryEvent.h"
#include "ResultsPresentation.h"
#include "ResultsList.h"
#include "DBDefines.h"
#include "MetaType.h"

class AddressScope;

namespace simplearchive {
	
	
	/**
	 * SystemHistory is the history of actions performed on the System.
	 */
	class SystemHistoryRow;
	//class SystemHistoryLog;

	

	class SystemHistorySchema : public MTTableSchema {
	public:
		SystemHistorySchema() noexcept : MTTableSchema("SystemHistory") {
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Integer, DB_EVENT));
			add(MTSchema(MTSchema::Integer, DB_VERSION));
			add(MTSchema(MTSchema::Text, DB_COMMENT));
		};
	};
	
	
	class SystemHistoryRow : public MTRow {
		static SystemHistorySchema m_tableSchema;
		
	public:
		SystemHistoryRow() : MTRow(m_tableSchema) {};
		SystemHistoryRow(const MTRow &row) : MTRow(m_tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}

		}
		
		const char *getFileName() {
			MTColumn& col = columnAt(static_cast<int>(CheckoutIndex::CO_FILENAME_IDX));
			return col.getString().c_str();
		}


	};
	
	

	class SystemHistoryPartition : public MTTable {
	public:
		SystemHistoryPartition() : MTTable(new SystemHistorySchema) {};
		virtual ~SystemHistoryPartition() {};
		bool findEvent(const char *Event);
	};

	class SystemHistoryAction : public CSVIndexAction {
		
		std::shared_ptr<SystemHistoryPartition> m_partition;

	protected:

		/// On the start of each directory found, this function is run.
		virtual bool onStart();
		/// At the end of each directory found, this function is run.
		virtual bool onEnd();
		/// On finding a file, this function is run.
		virtual bool onYearFolder(const char* name) { return true; };
		/// On finding a file, this function is run.
		virtual bool onYearEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onDayFolder(const char* name);
		/// On finding a directory, this function is run.
		virtual bool onDayEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onImage(const char* name);

		virtual bool onMetadata(const char* path, const char* name) { return true; };
		/// This function is a factory function used to create new FolderVisitor objects.

	public:
		/// Constructor
		SystemHistoryAction() : CSVIndexAction(std::make_shared<SystemHistorySchema>())
		{
			m_partition = std::make_shared<SystemHistoryPartition>();
		};
		/// Distructor
		virtual ~SystemHistoryAction() = default;

	};

	

	class SystemHistoryIndex : public CSVIndexSystemHistory {
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		SystemHistoryIndex();
		
		// Destructor
		virtual ~SystemHistoryIndex();
		
	};

	

	class SystemHistory : public CSVTable
	{
		
		bool add(SystemHistoryRow& SystemHistoryRow, const char* relpath);

		SharedMTRow getRow(const char* img);
		//bool processFile(std::shared_ptr<ResultsList> log, const std::string yyyymmddFile);
		bool historyLog(const char* filepath, ResultsPresentation::FormatType formatType);
		std::shared_ptr<ResultsList> getEntries(const char* filepath);
		//bool save(ExifDateTime dateAdded, SystemHistoryRow SystemHistoryRow);
		
	public:
		SystemHistory() :
			CSVTable(std::make_shared<SystemHistorySchema>(),
					std::make_shared<SystemHistoryPartition>(),
					std::make_shared<CSVIndexSystemHistory>(std::make_shared<SystemHistoryAction>())
				)
		{};
		~SystemHistory() = default;

		bool newImage(const char* imagePath, const char* comment);
		bool add(const char* filepath, int version, const char* comment, const HistoryEvent& he);
		bool add(const char* imagePath, const char* comment);
		
		void setPath(const char* indexRoot);
	};

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
};