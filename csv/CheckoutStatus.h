#pragma once
#include <memory>
#include "HistoryEvent.h"
//#include "LogDocument.h"
#include "DBDefines.h"
#include "MetaType.h"
#include "IndexVisitor.h"
#include "ResultsPresentation.h"
#include "CSVIndexAction.h"
#include "CSVTable.h"

class AddressScope;

namespace simplearchive {
	/**
	 * CheckoutStatus is used to control the checkout status of any image in the archive.
	 * As the derivatives are placed in the same workspace location as the master image
	 * then master archive check disk files are used.
	 */
	class CheckoutRow;
	class CheckoutStatusLog;
	class CheckoutStatusAction;
	
	class CheckoutStatus : public CSVTable
	{
		static std::string m_Master;
		static std::string m_workspace;
		static std::string m_primaryIndex;
		bool add(CheckoutRow &checkoutRow, const char *relpath);
		bool checkInOutUpdate(const char *img, const HistoryEvent::Event& event, const char*comment);
		SharedMTRow getRow(const char *img);
		//bool processFile(std::shared_ptr<CheckoutStatusLog> log, const std::string yyyymmddFile);
		//bool historyLog(const char *filepath, LogDocument::FormatType formatType);
		//std::shared_ptr<CheckoutStatusLog> getEntries(const char *filepath);
		bool isCheckedInOut(const char *img, bool in);
	public:
		CheckoutStatus();
		CheckoutStatus(std::shared_ptr<CheckoutStatusAction> indexAction);

		~CheckoutStatus() = default;
		
		static bool Init(const char *Master, const char *workspace, const char *primaryIndex);
		bool newImage(const char *imagePath, const char*comment);
		bool showCheckedOut(const char *path, ResultsPresentation::FormatType formatType, const char* file);
		bool showUncheckedOutChanges(const char *path, ResultsPresentation::FormatType formatType, const char* file);
		bool checkoutUpdate(const char *imagePath, const char*comment);
		bool checkinUpdate(const char *imagePath, const char*comment);
		bool uncheckoutUpdate(const char *imagePath, const char*comment);
		bool isCheckedIn(const char *img);
		bool isCheckedOut(const char *img);
		bool isChanged(const char *img, unsigned int crc, const char *md5);
		bool isPresent(const char * img);
		const HistoryEvent::Event checkoutStatus(const char *imagePath);
		bool scopedStatus(const char *scope);
		//bool uncheckout(const char *imagePath);
	};

	/// 
	/// 
	/// 

	class CheckoutSchema : public MTTableSchema {
	public:
		CheckoutSchema() noexcept : MTTableSchema("CheckoutProperties") {
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Integer, DB_EVENT));
			add(MTSchema(MTSchema::Integer, DB_VERSION));
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Text, DB_COMMENT));
		} ;
	};

	class CheckoutRow : public MTRow {
		static CheckoutSchema m_tableSchema;
		
	public:
		CheckoutRow() : MTRow(m_tableSchema) {};
		CheckoutRow(const MTRow &row) : MTRow(m_tableSchema) {

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

	

	class CheckoutStatusPartition : public MTTable {
	public:
		CheckoutStatusPartition() : MTTable(new CheckoutSchema) {};
		virtual ~CheckoutStatusPartition() {};
		bool findImage(const char *image);
	};


	

	/*
	class StatusAction : public IndexAction {
		std::shared_ptr<CheckoutStatusLog> log;

	protected:

		//std::shared_ptr<CheckoutRow> m_currentRow;
		//std::shared_ptr<CheckoutPartition> m_currentPartition;

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
		virtual bool onImage();

		//virtual bool action(std::unique_ptr<AddressScope> scope, std::shared_ptr<MTRow> row);

		virtual bool onMetadata(const char* path, const char* name) { return true; };
		/// This function is a factory function used to create new FolderVisitor objects.

	public:
		/// Constructor
		StatusAction() {};
		/// Distructor
		virtual ~StatusAction() {};


	};

	class CheckoutTableIndex : public IndexVisitor {
	protected:
		static std::string m_primaryIndex;
		AddressScope m_addressScope;
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CheckoutTableIndex(std::shared_ptr<IndexAction> indexAction) : IndexVisitor(indexAction) {};
		// Destructor
		virtual ~CheckoutTableIndex() = default;

		static bool Init(const char* primaryIndex);

		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		bool process(const char* rootFolder);
		bool process();
	};
	*/



	/*
	class CheckoutStatusActionTrigger : public CSVIndexAction {

		std::shared_ptr<CheckoutStatusPartition> m_partition;
		std::shared_ptr<CheckoutRow> m_currentRow;

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
		CheckoutStatusActionTrigger() : CSVIndexAction(std::make_shared<CheckoutSchema>())
		{
			m_partition = std::make_shared<CheckoutStatusPartition>();
		};
		/// Distructor
		virtual ~CheckoutStatusActionTrigger() = default;

	};
	*/


	class CheckoutStatusAction : public CSVIndexAction {

	protected:

		std::shared_ptr<CheckoutStatusPartition> m_partition;
		std::shared_ptr<MTRow> m_currentRow;

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
		CheckoutStatusAction() : CSVIndexAction(std::make_shared<CheckoutSchema>())
		{
			m_partition = std::make_shared<CheckoutStatusPartition>();
		};
		/// Distructor
		virtual ~CheckoutStatusAction() = default;

	};


	

	
};
