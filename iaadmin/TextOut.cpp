#include "TextOut.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>

std::string writePlainTag(const char* tag, const std::string& value) {
	std::ostringstream text;
	if (!value.empty()) {
		text <<  tag << " = " << value << "\n";
	}
	else {
		text << tag << " = none\n";
	}
	return text.str();
}

std::string TextOut::writeXMLTag(const char* tag, const std::string& value) {
	std::ostringstream xml;
	if (!value.empty()) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	}
	else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

std::string writeJsonTag(const char* tag, const std::string& value) {
	std::ostringstream jason;
	if (!value.empty()) {
		jason << "\"" << tag << "\":\"" << value << "\"\n";
	}
	else {
		//xml << "<" << tag << "/>\n";
	}
	return jason.str();
}

std::string writeHtmlTag(const char* tag, const std::string& value) {
	std::ostringstream html;
	if (!value.empty()) {
		html << "\"" << tag << "\":\"" << value << "\"\n";
	}
	else {
		//xml << "<" << tag << "/>\n";
	}
	return html.str();
}
