#pragma once
#include <memory>
#include "HistoryEvent.h"
#include "LogDocument.h"
#include "DBDefines.h"
#include "MetaType.h"

class AddressScope;

namespace simplearchive {
	/**
	 * CheckoutStatus is used to control the checkout status of any image in the archive.
	 * As the derivatives are placed in the same workspace location as the master image
	 * then master archive check disk files are used.
	 */
	class CheckoutRow;
	class CheckoutStatusLog;
	
	class CheckoutStatus
	{
		

		static std::string m_Master;
		static std::string m_workspace;
		static std::string m_primaryIndex;
		bool add(CheckoutRow &checkoutRow, const char *relpath);
		bool checkInOutUpdate(const char *img, const HistoryEvent::Event& event, const char*comment);
		SharedMTRow getRow(const char *img);
		bool processFile(std::shared_ptr<CheckoutStatusLog> log, const std::string yyyymmddFile);
		bool historyLog(const char *filepath, LogDocument::FormatType formatType);
		std::shared_ptr<CheckoutStatusLog> getEntries(const char *filepath);
		bool isCheckedInOut(const char *img, bool in);
	public:
		CheckoutStatus() = default;
		~CheckoutStatus() = default;
		
		static bool Init(const char *Master, const char *workspace, const char *primaryIndex);
		bool newImage(const char *imagePath, const char*comment);
		bool showCheckedOut(const char *path);
		bool showUncheckedOutChanges(const char *path);
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


	class CheckoutSchema : public MTTableSchema {
	public:
		CheckoutSchema() noexcept : MTTableSchema("CheckoutProperties") {
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Integer, DB_EVENT));
			add(MTSchema(MTSchema::Integer, DB_VERSION));
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Date, DB_COMMENT));
		} ;
	};

	class CheckoutRow : public MTRow {
		static CheckoutSchema m_tableSchema;
		friend class MetadataTemplate;
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

	

	class CheckoutPartition : public MTTable {
	public:
		CheckoutPartition() : MTTable(new CheckoutSchema) {};
		virtual ~CheckoutPartition() {};
		bool findImage(const char *image);
	};

	class CheckoutStatusLog : public LogDocument {

	public:
		CheckoutStatusLog();
		virtual ~CheckoutStatusLog();
		bool writeHuman();
		bool writeXML();
		bool writeCSV();
		bool writeJson();
	};

};