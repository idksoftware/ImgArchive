/*
 * MetadataDocument.h
 *
 *  Created on: Mar 16, 2015
 *      Author: wzw7yn
 */

#ifndef METADATADOCUMENT_H_
#define METADATADOCUMENT_H_

#include "LogDocument.h"

namespace simplearchive {

class MetadataDocument : public LogDocument {
	std::string m_imageName;
	std::string m_imagePath;
public:
	MetadataDocument() = default;
	virtual ~MetadataDocument() = default;
	bool write();
	bool writeXML();
	bool writeHtml(const char *fileName);

	const std::string& getImageName() const {
		return m_imageName;
	}

	void setImageName(const std::string& imageName) {
		m_imageName = imageName;
	}

	const std::string& getImagePath() const {
		return m_imagePath;
	}

	void setImagePath(const std::string& imagePath) {
		m_imagePath = imagePath;
	}
};

} /* namespace simplearchive */
#endif /* METADATADOCUMENT_H_ */
