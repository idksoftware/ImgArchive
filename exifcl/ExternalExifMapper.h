/*
 * ExifReader.h
 *
 *  Created on: May 30, 2014
 *      Author: wzw7yn
 */

#ifndef EXTERNALEXIFMAPPER_H_
#define EXTERNALEXIFMAPPER_H_

#include "ConfigReader.h"

namespace simplearchive {

class ExifObject;
class Config;
/**
 *	@brief This class is used to map external EXIF data from tools
 *	such as ExifTool.
 *
 *	This reader is initialised with a keyword map in order to map
 *	keywords from the external tool keywords to SIA keywords. This
 *	map also serves as as a way of mapping only available or required
 *	keywords so there may be equivalents. For  example:
 *
 *	Copyright = Copyright
 *
 *	Both keywords are the same but to include them in the EXIF values
 *	list must be present.
 */
class ExternalExifMapper {
	/// @brief This contains the external to internal EXIF mappings
	Config *m_exifMap;
	ExifObject *create(Config &exifData);
public:
	ExternalExifMapper();
	virtual ~ExternalExifMapper();

	/// This function converts the Exif map file into a Config Object
	bool init(const char *exifMapFile);

	/// @brief This function creates an ExifObject from the EXIF data returned
	/// From the external command. This is in the form of a keyword/value file. This
	/// read into a Config Object thus containing the external EXIF. create will
	/// map the data into internal format using the map config file passed in the
	/// init function.
	ExifObject *create(const char *exiffile);
};

} /* namespace simplearchive */
#endif /* EXTERNALEXIFMAPPER_H_ */
