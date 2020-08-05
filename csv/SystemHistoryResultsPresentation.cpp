


#include <iostream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"
#include <iomanip>
#include "DBDefines.h"
#include "HistoryEvent.h"
#include "SystemHistoryResultsPresentation.h"
#include "ResultsList.h"
#include "ResultsPresentation.h"

namespace simplearchive {

	class  SystemHistoryWriteHuman : WriteHuman {

	public:
		SystemHistoryWriteHuman(ResultsList& resultsList);
		~SystemHistoryWriteHuman() = default;

		bool write() override;
	};

	SystemHistoryWriteHuman::SystemHistoryWriteHuman(ResultsList& resultsList) : WriteHuman(resultsList) {}

	bool SystemHistoryWriteHuman::write() {
	
		/*
	std::ofstream file;
	if (!m_filename.empty()) {
		file.open(m_filename.c_str());
		if (file.is_open() == false) {
			return false;
		}
		file << "\n---------------------------------------------------\n";
		file << "Image: " << m_title << '\n';
		file << "Path : " << m_description << '\n';
		file << "=====================================================\n";
		file << "Date Time             version     Event      Comment\n\n";
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			//std::cout << *i << '\n';
			CSVArgs csvArgs(',');
			if (csvArgs.process(i->c_str()) == false) {
				return false;
			}

			file << csvArgs.at(0) << "    ";
			file << csvArgs.at(1) << "      ";
			file << csvArgs.at(4) << "  ";
			file << csvArgs.at(3) << "\n\n";
		}
	}
	else {
	*/
	//std::cout << "\n---------------------------------------------------\n";
	//std::cout << "Image: " << m_title << '\n';
	//std::cout << "Path : " << m_description << '\n';
	//std::cout << "=====================================================\n";
	//std::cout << "Date Time             version     Event      Comment\n\n";

		return true;
	}

	bool SystemHistoryResultsPresentation::writeHuman() {
		SystemHistoryWriteHuman systemHistoryWriteHuman(m_resultsList);
		if (!systemHistoryWriteHuman.write()) {
			return false;
		};
		return true;
	}

	class  SystemHistoryWriteJson : WriteJSON {

	public:
		SystemHistoryWriteJson(ResultsList& resultsList);
		~SystemHistoryWriteJson() = default;

		bool write() override;
	};

	SystemHistoryWriteJson::SystemHistoryWriteJson(ResultsList& resultsList) : WriteJSON(resultsList) {}

	bool SystemHistoryWriteJson::write() {
		return true;
	};

	bool SystemHistoryResultsPresentation::writeJson() {
		SystemHistoryWriteJson systemHistoryWriteJson(m_resultsList);
		if (!systemHistoryWriteJson.write()) {
			return false;
		};
		return true;
	}


	class  SystemHistoryWriteHtml : WriteHtml {

	public:
		SystemHistoryWriteHtml(ResultsList& resultsList);
		~SystemHistoryWriteHtml() = default;

		bool write() override;
	};

	SystemHistoryWriteHtml::SystemHistoryWriteHtml(ResultsList& resultsList) : WriteHtml(resultsList) {}

	bool SystemHistoryWriteHtml::write() {
		return true;
	};

	bool SystemHistoryResultsPresentation::writeHtml()
	{
		return false;
	}

	class  SystemHistorywriteCSV : public WriteCSV {

	public:
		SystemHistorywriteCSV(ResultsList& resultsList);
		~SystemHistorywriteCSV() = default;

		bool write() override;
	};

	SystemHistorywriteCSV::SystemHistorywriteCSV(ResultsList& resultsList) : WriteCSV(resultsList) {}

	bool SystemHistorywriteCSV::write() {
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			std::cout << *rowIt << '\n';
		}
		return true;
	}

	bool SystemHistoryResultsPresentation::writeCSV() {
		SystemHistorywriteCSV systemHistorywriteCSV(m_resultsList);
		if (!systemHistorywriteCSV.write()) {
			return false;
		}
		return true;
	}

	class SystemHistoryWriteXML : WriteXML {
	public:
		SystemHistoryWriteXML(ResultsList& resultsList);
		~SystemHistoryWriteXML() = default;

		bool write() override;
	};

	SystemHistoryWriteXML::SystemHistoryWriteXML(ResultsList& resultsList) : WriteXML(resultsList) {}

	bool SystemHistoryWriteXML::write() {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";


		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			std::cout << "\t<Event>\n";
			for (size_t i = 0; i != row->size(); i++) {
				MTTableSchema tableSchema = m_resultsList.getTableSchema();
				std::cout << writeTag(tableSchema.getColumnName(i).c_str(), row->columnAt((int)i).toString(), 2);
			}
			std::cout << "\t</Event>\n";
		}

		std::cout << "</Catalog>\n";
		return true;
	}

	bool SystemHistoryResultsPresentation::writeXML() {
		SystemHistoryWriteXML systemHistoryWriteXML(m_resultsList);
		if (!systemHistoryWriteXML.write()) {
			return false;
		}
		return true;
	}
}; // namespace