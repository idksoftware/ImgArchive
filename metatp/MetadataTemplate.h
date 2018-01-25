/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */


#ifndef METADATATEMPLATE_H_
#define METADATATEMPLATE_H_

#include <map>
#include <mutex>
#include "ConfigReader.h"

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

typedef std::unique_ptr<MetadataObject> MetadataObject_ptr;
typedef std::unique_ptr<ConfigBlock> Config_ptr;
//class Config;
class MetadataTemplate {
	//std::map<std::string, std::string *> m_lookup;
	//MetadataObject *m_metadataObject;
	//static std::unique_ptr<Config> m_templateFile;
	static Config_ptr m_templateFile;
	static std::string &getValue(const char *key);

	static std::unique_ptr<MetadataTemplate> m_instance;
	static std::once_flag m_onceFlag;
	MetadataTemplate() {};
	MetadataTemplate(const MetadataTemplate& src);
	MetadataTemplate& operator=(const MetadataTemplate& rhs);

public:
	static MetadataTemplate& GetInstance();
	virtual ~MetadataTemplate();
	static bool read(const char *datafile);
	static MetadataObject_ptr getMetadataObject();
};

} /* namespace simplearchive */
#endif /* METADATATEMPLATE_H_ */
