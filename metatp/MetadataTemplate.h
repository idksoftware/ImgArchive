/*
 * MetadataTemplate.h
 *
 *  Created on: Jul 7, 2014
 *      Author: wzw7yn
 */
#include <map>
#ifndef METADATATEMPLATE_H_
#define METADATATEMPLATE_H_

namespace simplearchive {
/**
 * @brief Metadata Container
 *
 * This contains the metadata Object that is created as an outcome of the substitution process.
 * This process is contained in this class using the Configuration reader class ConfigReader
 * using the configuration container. As each template is read the contents will be placed
 * into this class. At the end of the process the contents of the Config class is copied into
 * the Metadata Object class MetadataObject. This class will be used for further substitutions
 * by the Exif reading and user defined metadata information.
 *
 */

class MetadataObject;
class MetadataTemplate {
	std::map<std::string, std::string *> m_lookup;
	MetadataObject *m_metadataObject;
public:
	MetadataTemplate();
	virtual ~MetadataTemplate();
	bool read(const char *datafile);
	std::string *getValue(const char *key);

	// change to auto ptr
	MetadataObject* getMetadataObject() {
		return m_metadataObject;
	}
};

} /* namespace simplearchive */
#endif /* METADATATEMPLATE_H_ */
