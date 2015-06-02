/*
 * MetadataUpdate.h
 *
 *  Created on: Mar 2, 2015
 *      Author: wzw7yn
 */

#ifndef METADATAUPDATE_H_
#define METADATAUPDATE_H_

namespace simplearchive {

class MetadataDocument;
class MetadataObject;
class MetadataUpdate {
	bool command(std::string &cmd, MetadataObject &mo);
	static std::string m_archivePath;
public:
	MetadataUpdate();
	virtual ~MetadataUpdate();

	bool process(const char *imagePath, const char *imageName, const char *cmd);
	std::auto_ptr<MetadataDocument> get(const char *imagePath, const char *imageName);
	bool loadMetadataDocument(MetadataDocument &metadataDocument, MetadataObject &metadataObject);
};



} /* namespace simplearchive */
#endif /* METADATAUPDATE_H_ */
