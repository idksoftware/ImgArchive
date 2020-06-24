#pragma once
#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include "HistoryEvent.h"
#include "CSVTable.h"
#include "CSVIndexAction.h"
#include "DBDefines.h"
#include "MetaType.h"
#include "ResultsPresentation.h"

namespace simplearchive {

#define HISTORY_FILE "history.dat"
	class EventList;
	class CDate;
	

	class HistoryEvent;

	class ImageHistorySchema : public MTTableSchema {
	public:
		ImageHistorySchema() : MTTableSchema(TABLE_IMAGE_HISTORY) {
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Integer, DB_VERSION));// template
			add(MTSchema(MTSchema::Integer, DB_EVENT));
			add(MTSchema(MTSchema::Text, DB_COMMENT));// template
		}
	};

	

	class ImageHistoryRow : public MTRow {
		static ImageHistorySchema m_tableSchema;
	public:
		ImageHistoryRow();
		ImageHistoryRow(const char* filepath, const char* version, const char* comment, const HistoryEvent& he);
		ImageHistoryRow(const MTRow& row) : MTRow(m_tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}
		}
	};

	class ImageHistoryPartition : public MTTable {
	public:
		ImageHistoryPartition() : MTTable(new ImageHistorySchema) {};
		virtual ~ImageHistoryPartition() {};
		bool findEvent(const char* Event);
	};


	class ImageHistoryAction : public CSVIndexAction {

		std::shared_ptr<ImageHistoryPartition> m_partition;

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
		ImageHistoryAction() : CSVIndexAction(std::make_shared<ImageHistorySchema>())
		{
			m_partition = std::make_shared<ImageHistoryPartition>();
		};
		/// Distructor
		virtual ~ImageHistoryAction() = default;

	};


	/**
		This is the Image only log for one image.
		The history normaly will end in <imagename>.<ext>.hst
	*/
	class HistoryItem;
	class ArchivePath;
	class ImageHistory : public CSVTable
	{
	private:
		static std::string m_workspace;
		static std::string m_indexRoot;
		static std::string m_backup1;
		static std::string m_backup2;
		ImageHistory(const ImageHistory&);
		ImageHistory& operator = (const ImageHistory&) { return *this; };

		bool writeLog(HistoryItem& item, const char* path);
		//bool readLog(const char* logFile, ImageHistoryLog& historyLog);
	public:

		
		ImageHistory() :
			CSVTable(std::make_shared<ImageHistorySchema>(),
				std::make_shared<ImageHistoryPartition>(),
				std::make_shared<CSVIndexImageHistory>(std::make_shared<ImageHistoryAction>()))
		{}
	
		virtual ~ImageHistory() = default;
		
		void setPath(const char* workspacePath, const char* indexPath) {
			m_workspace = workspacePath;
			m_indexRoot = indexPath;
			CSVTable::setPath(indexPath);
		}
		bool init();

		bool newImage(const char* filename, const char* comment);
		bool add(const char* filename, int version, const char* comment, const HistoryEvent& he);
		bool add(const char* filename, const char* version, const char* comment, const HistoryEvent& he);

		//std::shared_ptr<ImageHistoryLog> getEntries(const char* imagePath);
		static void setBackup1Path(const char* path) {
			m_backup1 = path;
		}
		static void setBackup2Path(const char* path) {
			m_backup2 = path;
		}

		static ImageHistory& get();
	};

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
}
