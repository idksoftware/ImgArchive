/*
 * ExifMapConfig.h
 *
 *  Created on: May 30, 2014
 *      Author: wzw7yn
 */

#ifndef EXIFMAPCONFIG_H_
#define EXIFMAPCONFIG_H_

#include "ConfigReader.h"

namespace simplearchive {

class ExifObject;
/** @brief This class is contains the map EXIF keyword mappings between the external tool output and
 * The internal EXIF mapping
 */

class ExifMapConfig : public Config {

public:
	/// @brief Constructor for class.
	ExifMapConfig();
	/// @brief Destructor for class.
	virtual ~ExifMapConfig();
};

} /* namespace simplearchive */
#endif /* EXIFMAPCONFIG_H_ */
