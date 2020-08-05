
#include <iostream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"
#include <iomanip>
#include "DBDefines.h"
#include "HistoryEvent.h"
#include "MasterMatadataResultsPresentation.h"
#include "ResultsList.h"
#include "ResultsPresentation.h"

namespace simplearchive {

	class MasterDatabaseWriteHuman : WriteHuman {

	public:
		MasterDatabaseWriteHuman(ResultsList& resultsList);
		~MasterDatabaseWriteHuman() = default;

		bool write() override;
	};
	
	MasterDatabaseWriteHuman::MasterDatabaseWriteHuman(ResultsList& resultsList) : WriteHuman(resultsList) {}

	bool MasterDatabaseWriteHuman::write()
	{
		int rowSize = m_resultsList.getTableSchema().size();
		ColumnJustification columnJustification(rowSize);

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			columnJustification.readRow(row);
		}
		int eventIdx = -1;

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			// Row listing
			auto schemaIdx = m_resultsList.getTableSchema().begin();
			auto collumnIdx = row->begin();
			int idx = 0;
			std::cout << "---------------------------------------------------------------------------\n";
			// Discription
			for (int i = 0; i < 6; i++) {

				// Schema
				MTSchema& columnInfo = *schemaIdx;
				std::string s = columnInfo.getName();
				columnJustification.header(idx, s);
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << s;
				schemaIdx++;
				idx++;
			}
			std::cout << '\n';

			idx = 0;
			for (int i = 0; i < 6; i++) {
				// data
				SharedMTColumn column = *collumnIdx;
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << column->toString();

				collumnIdx++;
				idx++;
			}
			std::cout << "\n\n";
			// file
			int offset = idx;
			for (int i = 0; i < 6; i++) {

				// Schema
				MTSchema& columnInfo = *schemaIdx;
				std::string s = columnInfo.getName();
				columnJustification.header(idx, s);
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << s;
				schemaIdx++;
				idx++;
			}
			std::cout << '\n';

			idx = offset;
			for (int i = 0; i < 6; i++) {
				// data
				SharedMTColumn column = *collumnIdx;
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << column->toString();

				collumnIdx++;
				idx++;
			}
			std::cout << "\n\n";
			// next
			offset = idx;
			for (int i = 0; i < 6; i++) {

				// Schema
				MTSchema& columnInfo = *schemaIdx;
				std::string s = columnInfo.getName();
				columnJustification.header(idx, s);
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << s;
				schemaIdx++;
				idx++;
			}
			std::cout << '\n';
			idx = offset;
			for (int i = 0; i < 6; i++) {
				// data
				SharedMTColumn column = *collumnIdx;
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << column->toString();

				collumnIdx++;
				idx++;
			}
			std::cout << '\n';
			std::cout << '\n'; // end
			// next
			offset = idx;
			for (int i = 0; i < 6; i++) {

				// Schema
				MTSchema& columnInfo = *schemaIdx;
				std::string s = columnInfo.getName();
				columnJustification.header(idx, s);
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << s;
				schemaIdx++;
				idx++;
			}
			std::cout << '\n';
			idx = offset;
			for (int i = 0; i < 6; i++) {
				// data
				SharedMTColumn column = *collumnIdx;
				std::cout << std::setw(columnJustification.getSize(idx) + 2) << column->toString();

				collumnIdx++;
				idx++;
			}
			std::cout << '\n';
			std::cout << '\n'; // end
		}
		/*
		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {

			MTSchema& columnInfo = *i;
			std::string s = columnInfo.getName();
			columnJustification.header(idx, s);

			if (columnInfo.getName().compare(DB_EVENT) == 0) {
				eventIdx = idx;
				std::cout << std::setw(HistoryEvent::maxStringSize() + 1) << columnInfo.getName();
			}
			else {
				std::cout << std::setw(columnJustification.getSize(idx++) + 1) << columnInfo.getName();
			}
		}
		printf("\n");
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			idx = 0;
			for (auto i = row->begin(); i != row->end(); i++) {
				SharedMTColumn column = *i;
				if (eventIdx == idx) {
					HistoryEvent::Event evn = static_cast<HistoryEvent::Event>(column->getInt());
					std::cout << std::setw(HistoryEvent::maxStringSize() + 1) << HistoryEvent::getString(evn);
				}
				else {
					std::cout << std::setw(columnJustification.getSize(idx) + 1) << column->toString();
				}
				idx++;
			}
			std::cout << '\n';
		}
		*/
		return true;
	}

	class MasterDatabaseWriteXML : WriteXML {
	public:
		MasterDatabaseWriteXML(ResultsList& resultsList);
		~MasterDatabaseWriteXML() = default;

		bool write() override;
	};

	MasterDatabaseWriteXML::MasterDatabaseWriteXML(ResultsList& resultsList) : WriteXML(resultsList) {}

	bool MasterDatabaseWriteXML::write() {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<Catalog ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			std::cout << "\t<Image>\n";
			for (size_t i = 0; i != row->size(); i++) {
				MTTableSchema tableSchema = m_resultsList.getTableSchema();
				std::cout << writeTag(tableSchema.getColumnName(i).c_str(), row->columnAt((int)i).toString(), 2);
			}
			std::cout << "\t</Image>\n";
		}
		std::cout << "</Catalog>\n";
		return true;
	}


	bool MasterMatadataResultsPresentation::writeHuman() {

		MasterDatabaseWriteHuman masterDatabaseWriteHuman(m_resultsList);
		if (!masterDatabaseWriteHuman.write()) {
			return false;
		}
		return true;

	}

	bool MasterMatadataResultsPresentation::writeXML() {

		MasterDatabaseWriteXML masterDatabaseWriteXML(m_resultsList);
		if (!masterDatabaseWriteXML.write()) {
			return false;
		}
		return true;
	}

	bool MasterMatadataResultsPresentation::writeCSV() {
		return true;
	}

	bool MasterMatadataResultsPresentation::writeJson() {
		return true;
	}

	bool MasterMatadataResultsPresentation::writeHtml() {
		return true;
	}

}; // namespace