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
	 * ArchiveHistory is the history of actions performed on the archive.
	 */
	class ArchiveHistoryRow;
	//class ArchiveHistoryLog;

	

	class ArchiveHistorySchema : public MTTableSchema {
	public:
		ArchiveHistorySchema() noexcept : MTTableSchema("ArchiveHistory") {
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Integer, DB_EVENT));
			add(MTSchema(MTSchema::Integer, DB_VERSION));
			add(MTSchema(MTSchema::Text, DB_COMMENT));
		};
	};
	
	
	class ArchiveHistoryRow : public MTRow {
		static ArchiveHistorySchema m_tableSchema;
		
	public:
		ArchiveHistoryRow() : MTRow(m_tableSchema) {};
		ArchiveHistoryRow(const MTRow &row) : MTRow(m_tableSchema) {

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
	
	

	class ArchiveHistoryPartition : public MTTable {
	public:
		ArchiveHistoryPartition() : MTTable(new ArchiveHistorySchema) {};
		virtual ~ArchiveHistoryPartition() {};
		bool findEvent(const char *Event);
	};

	/*
	class ArchiveHistoryLog : public LogDocument {

	public:
		ArchiveHistoryLog();
		virtual ~ArchiveHistoryLog();
		bool writeHuman();
		bool writeXML();
		bool writeCSV();
		bool writeJson();
		bool writeHtml() {
			return true;
		};
	};
	*/

	/*
	class ArchiveHistoryAction : public CSVIndexAction {
		
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
		//virtual bool onImage(const char* name);

		virtual bool onMetadata(const char* path, const char* name) { return true; };
		/// This function is a factory function used to create new FolderVisitor objects.

	public:
		/// Constructor
		ArchiveHistoryAction() : CSVIndexAction(std::make_shared<ArchiveHistorySchema>()) {};
		/// Distructor
		virtual ~ArchiveHistoryAction() = default;

	};
	*/
	/*

	class ArchiveHistoryIndex {
		
		static std::string m_indexPath;
		std::shared_ptr<IndexAction> m_indexAction;
		std::unique_ptr<AddressScope> m_addressScope;
		bool process(const char* rootFolder);
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		ArchiveHistoryIndex(std::shared_ptr<ArchiveHistoryAction> indexAction);
		//static bool Init(const char* Master, const char* workspace, const char* primaryIndex);
		bool setScope(const char* scope);
		// Destructor
		virtual ~ArchiveHistoryIndex();
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		bool process();
		static void setPath(const char* indexRoot);
		
	};

	*/

	class ArchiveHistory : public CSVTable
	{
		
		bool add(ArchiveHistoryRow& archiveHistoryRow, const char* relpath);

		SharedMTRow getRow(const char* img);
		//bool processFile(std::shared_ptr<ResultsList> log, const std::string yyyymmddFile);
		bool historyLog(const char* filepath, ResultsPresentation::FormatType formatType);
		std::shared_ptr<ResultsList> getEntries(const char* filepath);
		bool save(ExifDateTime dateAdded, ArchiveHistoryRow archiveHistoryRow);
		
	public:
		ArchiveHistory() : CSVTable(std::make_shared<ArchiveHistorySchema>()) {};
		~ArchiveHistory() = default;

		bool newImage(const char* imagePath, const char* comment);
		bool add(const char* filepath, int version, const char* comment, const HistoryEvent& he);
		bool add(const char* imagePath, const char* comment);
		
		static ArchiveHistory& get();
		static void setPath(const char* indexRoot);



	};

};