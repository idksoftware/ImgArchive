#pragma once

#include <string>
#include <list>
#include <memory>
#include "MetaType.h"


namespace simplearchive {

	class ColumnJustification {
		size_t* m_list;
		size_t m_rowSize;
	public:
		ColumnJustification(size_t rowSize)
			: m_rowSize{ rowSize }

		{
			m_list = new size_t[rowSize];
		}
		~ColumnJustification()
		{
			delete[] m_list;
		}

		void header(int idx, const char* name) {
			std::string s = name;
			header(idx, s);
		}

		void header(int idx, std::string& name) {
			size_t len = name.length();
			size_t* c = (m_list + idx);
			if (*c < len) *c = len;
		}

		void readRow(SharedMTRow row) {
			for (size_t i = 0; i < m_rowSize; i++) {
				*(m_list + i) = 0;
			}
			int i = 0;
			for (auto irow = row->begin(); irow != row->end(); irow++) {
				SharedMTColumn column = *irow;
				size_t len = column->toString().length();
				size_t* c = (m_list + i);
				if (*c < len) *c = len;
				i++;
			}
		}

		size_t getSize(int i) {
			size_t* c = (m_list + i);
			return *c;
		}
	};



	class ResultsList;

	class WriteHuman {
	protected:
		ResultsList& m_resultsList;
	public:
		WriteHuman(ResultsList& resultsList) : m_resultsList{ resultsList }
		{}
		~WriteHuman() = default;

		virtual bool write();
	};

	class CheckoutWriteHuman : WriteHuman {
		
	public:
		CheckoutWriteHuman(ResultsList& resultsList);
		~CheckoutWriteHuman() = default;

		bool write() override;
	};

	class ResultsPresentation
	{
	public:
		enum class FormatType {
			XML,
			Json,
			Human,
			csv,
			Html,
			unknown
		};
	protected:
		std::string writeTag(const char* tag, const std::string& value, int tab);
		std::string writeTag(const char* tag, const int value, int tab);
		
		ResultsList& m_resultsList;
		std::string m_title;
		std::string m_description;
		std::string m_image;

		std::string m_filename;

		static bool iequals(const std::string& a, const std::string& b);

	public:
		ResultsPresentation(ResultsList& resultsList) :
			m_resultsList{ resultsList }
		{}
			
		virtual ~ResultsPresentation() = default;

		bool write(FormatType formatType);

		virtual bool writeHuman();
		virtual bool writeXML() { return true; };
		virtual bool writeCSV() { return true; };
		virtual bool writeJson() { return true; };
		virtual bool writeHtml() { return true; };

		void setFilename(const char* f) {
			m_filename = f;
		}

		void setTitle(const char* t) {
			m_title = t;
		}

		void setDescription(const char* d) {
			m_description = d;
		}

		void setImage(const char* i) {
			m_image = i;
		}

		static FormatType parse(const char* str);
	};

};
