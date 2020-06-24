#include "CSVTable.h"

#include "CSVIndexVisitor.h"
#include "SAUtils.h"
#include "LogName.h"
#include "pathcontroller.h"

namespace simplearchive {
	void CSVTable::setPath(const char* indexRoot)
	{
		m_indexRoot = indexRoot;
	}

	bool CSVTable::add(const char* yearDay, MTRow& row) {

		PathController pathController(yearDay);
		std::string pidxPath = m_indexRoot;
		pidxPath += '/';
		pidxPath += pathController.getYear();
		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			if (SAUtils::mkDir(pidxPath.c_str()) == false) {
				return false;
			}
		}

		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;

		if (SAUtils::FileExists(pidxPath.c_str()) == true) {
			m_mtTablePartition->read(pidxPath.c_str());
		}
		m_mtTablePartition->addRow(row);
		m_mtTablePartition->write(pidxPath.c_str());

		return true;
	}




	bool CSVTable::save(ExifDateTime &dateAdded, MTRow &row)
	{
		std::string dateStr = dateAdded.toLogString();

		std::string yearStr = dateStr.substr(0, 4);
		std::string monthStr = dateStr.substr(5, 2);
		std::string dayStr = dateStr.substr(8, 2);

		std::string addressStr = yearStr + '-' + monthStr + '-' + dayStr;

		if (SAUtils::DirExists(m_indexRoot.c_str()) == false) {
			return false;
		}

		std::string indexPath = m_indexRoot + '/' + yearStr;

		if (SAUtils::DirExists(indexPath.c_str()) == false) {
			if (SAUtils::mkDir(indexPath.c_str()) == false) {
				return false;
			}
		}

		indexPath += '/' + addressStr;

		if (SAUtils::DirExists(indexPath.c_str()) == false) {
			if (SAUtils::mkDir(indexPath.c_str()) == false) {
				return false;
			}
		}

		LogName logName;
		std::string currentFilename = logName.makeName(indexPath.c_str(), "", "hst", 1);

		indexPath += '/' + currentFilename;

		//ArchiveHistoryPartition archiveHistoryPartition;
		if (SAUtils::FileExists(indexPath.c_str()) == true) {
			m_mtTablePartition->read(indexPath.c_str());
		}
		m_mtTablePartition->addRow(row);
		m_mtTablePartition->write(indexPath.c_str());

		return true;
	}

	const char* CSVTable::getName()
	{
		return m_mtTableSchema->getName().c_str();
	}

	bool CSVTable::select(const char* scope)
	{
		/*
		CSVIndexVisitor indexVisitor(std::make_shared<CSVIndexAction>(m_mtTableSchema));
		if (!indexVisitor.setScope(scope)) {
			return false;
		}
		indexVisitor.process(m_indexRoot.c_str());
		*/
		if (!m_csvIndexVisitor->setScope(scope)) {
			return false;
		}
		m_csvIndexVisitor->process(m_indexRoot.c_str());
		m_resultsList = m_csvIndexVisitor->getIndexAction().getResultsList();
		for (auto i = m_resultsList->begin(); i != m_resultsList->end(); i++) {
			std::shared_ptr<MTRow> row = *i;
			const MTTableSchema& ts = row->getSchema(); // testing only
			printf("MTTableSchema: %s\n", ts.getName().c_str());
			

		}
		return true;
	}
};