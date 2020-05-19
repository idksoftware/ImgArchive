#pragma once

#include <string>
#include <list>
#include <memory>
#include "MetaType.h"


namespace simplearchive {

	class ResultsList;

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
