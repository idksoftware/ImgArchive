/*
 * XMLWriter.h
 *
 *  Created on: Jul 1, 2014
 *      Author: wzw7yn
 */

#ifndef XMLWRITER_H_
#define XMLWRITER_H_
#include <string>

namespace simplearchive {

class MetadataObject;
class SideCar;
class XMLWriter {
	std::string writeTag(const char *tag, const std::string& value);
public:
	XMLWriter();
	virtual ~XMLWriter();
	bool writeImage(MetadataObject &container, const char *path);
	bool writeGroup(SideCar &sideCar, const char *path);
};

} /* namespace simplearchive */
#endif /* XMLWRITER_H_ */
