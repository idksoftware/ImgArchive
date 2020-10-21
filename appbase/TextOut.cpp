
#include "TextOut.h"
//#include "AppConfig.h"
#include "SAUtils.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

#define PLAIN_TEXT_LABEL		"plain"
#define XML_LABEL				"xml"
#define JSON_LABEL				"json"
#define HTML_LABEL				"html"

std::string TextOut::writePlainTag(const char* tag, const char* v) {
	const std::string& value = v;
	return writePlainTag(tag, value);
}

std::string TextOut::writePlainTag(const char* tag, const std::string& value) {
	std::ostringstream text;
	if (!value.empty()) {
		text <<  tag << " = " << value << "\n";
	}
	else {
		text << tag << " = none\n";
	}
	return text.str();
}

std::string TextOut::writeXMLTag(const char* tag, bool b) {
	const std::string& value = (b)? "true" : "false";
	return writeXMLTag(tag, value);
}

std::string TextOut::writeXMLTag(const char* tag, const char* v) {
	const std::string& value = v;
	return writeXMLTag(tag, value);
}

std::string TextOut::writeXMLTag(const char* tag, const std::string& value) {
	std::ostringstream xml;
	if (!value.empty()) {
		xml << "<" << tag << ">" << value << "</" << tag << ">" << std::endl;
	}
	else {
		xml << "<" << tag << "/>" << std::endl;
	}
	return xml.str();
}

std::string TextOut::writeJsonTag(const char* tag, const char* v, bool end) {
	const std::string& value = v;
	return writeJsonTag(tag, value, end);
}


std::string TextOut::writeJsonTag(const char* tag, const std::string& value, bool end) {
	std::ostringstream jason;
	if (!value.empty()) {
		std::string tmp = value;
		std::replace(tmp.begin(), tmp.end(), '\\', '/');
		if (!end) {
			jason << "\"" << tag << "\"" << ":\"" << tmp << "\"" << "," << std::endl;
		}
		else {
			jason << "\"" << tag << "\"" << ":\"" << tmp << "\"" << std::endl;
		}
	}
	
	return jason.str();
}

std::string TextOut::writeHtmlTag(const char* tag, const char* v) {
	const std::string& value = v;
	return writeHtmlTag(tag, value);
}

std::string TextOut::writeHtmlTag(const char* tag, const std::string& value) {
	std::ostringstream html;
	if (!value.empty()) {
		html << "\"" << tag << "\":\"" << value << "\"\n";
	}
	else {
		//xml << "<" << tag << "/>\n";
	}
	return html.str();
}

bool TextOut::parseTextOutType(const char *s)
{
	std::string optionString = s;
	if (SAUtils::isEquals(PLAIN_TEXT_LABEL, optionString)) {
		m_textOutType = TextOutType::plain;
		return true;
	}
	if (SAUtils::isEquals(XML_LABEL, optionString)) {
		m_textOutType = TextOutType::xml;
		return true;
	}
	if (SAUtils::isEquals(JSON_LABEL, optionString)) {
		m_textOutType = TextOutType::json;
		return true;
	}
	if (SAUtils::isEquals(HTML_LABEL, optionString)) {
		m_textOutType = TextOutType::html;
		return true; 
	}
	m_textOutType = TextOutType::plain;
	return true;
}

std::string TextOut::toString() {
	switch (m_textOutType) {
	case TextOutType::plain:
		return writePlain();
	case TextOutType::xml:
		return writeXML();
	case TextOutType::json:
		return writeJson();
	case TextOutType::html:
		return writeHtml();
	case TextOutType::unknown:
	default:
		return std::string();
	}
	return std::string();
}

void TextOut::process() {
	if (m_filename.empty()) {
		std::cout << toString();
	}
	std::ofstream file(m_filename.c_str());
	if (file.is_open() == true) {
		switch (m_textOutType) {
		case TextOutType::xml:
			file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
			break;
		//case TextOutType::json:
		//case TextOutType::html:
		//	return writeHtml();
		}
		file << toString();
	}
	file.close();
}
