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

#include "XMLWriter.h"
#include "ImageSideCar.h"
#include "MetadataObject.h"
#include "VersionMetadataObject.h"
#include "JpegEXIF.h"
#include <sstream>
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


namespace simplearchive {

XMLWriter::XMLWriter() {
	// TODO Auto-generated constructor stub

}

XMLWriter::~XMLWriter() {
	// TODO Auto-generated destructor stub
}

std::string XMLWriter::writeTag(const char *tag, const std::string& value) {
	std::ostringstream xml;
	if (!value.empty()) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	} else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

bool XMLWriter::writeGroup(SideCar &sideCar, const char *path) {
	std::ofstream xmlFile;
		xmlFile.open(path);
		xmlFile <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				<<	"<Group>\n";
		        //<< writeTag("GroupName", group.getName());

		for (std::vector<SideCarItem>::iterator i = sideCar.begin(); i != sideCar.end(); i++) {
			SideCarItem sideCarItem = *i;
		    xmlFile <<	"<GroupItem>\n"
		            << writeTag("SequenceId", sideCarItem.getSequenceId().c_str())
		            << writeTag("Filename", sideCarItem.getFilename().c_str())
		            << writeTag("Filepath", sideCarItem.getFilepath().c_str())
		            << writeTag("UniqueId", sideCarItem.getUniqueId().c_str())
		            << writeTag("Md5", sideCarItem.getMd5().c_str())
		            << writeTag("Crc", sideCarItem.getCrc().c_str())
		            <<	"</GroupItem>\n";

		            }
			xmlFile <<	"<Group>\n";


		xmlFile.close();

		return true;

	}

bool XMLWriter::writeImage(MetadataObject &container, const char *path) {

	std::ofstream xmlFile;
	xmlFile.open(path);
	xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<Metadata>\n"

		<< "<File>\n"
		<< writeTag(DB_SEQUENCEID, container.getSequenceIdString())	// *
		<< writeTag(DB_FILENAME, container.getFilenameString())
		<< writeTag(DB_ORGINALNAME, container.getOrginalNameString())
		<< writeTag(DB_FILEPATH, container.getFilepathString())
		<< writeTag(DB_UUID, container.getUniqueIdString())
		<< writeTag(DB_MEDIATYPE, container.getMediaTypeString())
		<< writeTag(DB_MD5, container.getMd5String())
		<< writeTag(DB_CRC, container.getCrcString())
		<< writeTag(DB_FILESIZE, container.getFileSizeString())
		<< writeTag(DB_DATECREATE, container.getDateCreateString())
		<< writeTag(DB_DATEMODIFIED, container.getDateModifiedString())
		<< writeTag(DB_DATEADDED, container.getAddedDateString())
		<< "</File>\n"
		<< "<Description>\n"
		<< writeTag(DB_DESCRIPTION, container.getDescriptionString())
		<< writeTag(DB_LABEL, container.getLabelString())
		<< writeTag(DB_TITLE, container.getTitleString())
		<< writeTag(DB_SUBJECT, container.getSubjectString())
		<< writeTag(DB_RATING, container.getRatingString())
		<< writeTag(DB_RATINGPERCENT, container.getRatingPercentString())
		<< writeTag(DB_KEYWORDS, container.getKeywordsString()) // template
		<< writeTag(DB_VERSION, container.getVersionString()) // template
		<< "</Description>\n"
		<< "<Origin>\n"
		<< writeTag(DB_COMMENT, container.getCommentString())// template
		<< writeTag(DB_AUTHOR, container.getAuthorString())
		<< writeTag(DB_CAPTUREDATE, container.getCaptureDateString())
		<< writeTag(DB_SUBSETIMEORIGINAL, container.getSubjectDIstanceRangeString())
		<< writeTag(DB_COPYRIGHT, container.getCopyrightString())
		<< "</Origin>\n"
		<< "<Image>\n"
		// Image
		<< writeTag(DB_WIDTH, container.getWidthString())
		<< writeTag(DB_HEIGHT, container.getHeightString())
		<< writeTag(DB_VIEWROTATION, container.getViewRotationString())
		<< writeTag(DB_BITSPERSAMPLE, container.getBitsPerSampleString())
		<< writeTag(DB_XRESOLUTION, container.getXResolutionString())
		<< writeTag(DB_YRESOLUTION, container.getYResolutionString())
		<< writeTag(DB_RESOLUTIONUNIT, container.getResolutionUnitString())
		<< writeTag(DB_COMPRESSION, container.getCompressionString())
		<< "</Image>\n"
		<< "<Camera>\n"
		// Camera

		<< writeTag(DB_MAKER, container.getMakerString())
		<< writeTag(DB_MODEL, container.getModelString())
		<< writeTag(DB_EXPOSURETIME, container.getExposureTimeString())
		<< writeTag(DB_APERTURE, container.getApertureString())
		<< writeTag(DB_ISOSPEEDRATING, container.getIsoSpeedRatingString())
		<< writeTag(DB_EXPOSUREBIAS, container.getExposureBiasString())
		<< writeTag(DB_EXPOSUREPROGRAM, container.getExposureProgramString())
		<< writeTag(DB_SUBJECTDISTANCE, container.getSubjectDistanceString())
		<< writeTag(DB_FOCALLENGTH, container.getFocalLengthString())
		<< writeTag(DB_FOCALLENGTHIN35MM, container.getFocalLengthIn35mmString())
		<< writeTag(DB_FLASH, container.getFlashString())
		<< writeTag(DB_METERINGMODE, container.getMeteringModeString())
		<< writeTag(DB_DIGITALZOOM, container.getDigitalZoomString())
		<< "</Camera>\n"
		<< "<AdvancedPhoto>\n"
		// Advanced photo
		<< writeTag(DB_LIGHTSOURCE, container.getLightSourceString())
		<< writeTag(DB_COLORSPACE, container.getColorSpaceString())
		<< writeTag(DB_WHITEBALANCE, container.getWhiteBalanceString())
		<< writeTag(DB_SCENECAPTURETYPE, container.getSceneCaptureTypeString())
		<< writeTag(DB_CONTRAST, container.getContrastString())
		<< writeTag(DB_SATURATION, container.getSaturationString())
		<< writeTag(DB_SHARPNESS, container.getSharpnessString())
		<< writeTag(DB_BRIGHTNESSVALUE, container.getBrightnessValueString())
		<< writeTag(DB_SUBJECTDISTANCERANGE, container.getSubjectDIstanceRangeString())
		<< writeTag(DB_MAXAPERTURE, container.getMaxApertureString())
		<< writeTag(DB_COMPRESSEDBITSPERPIXEL, container.getCompressedBitsPerPixelString())
		<< writeTag(DB_FLASHENERGY, container.getFlashEnergyString())
		<< writeTag(DB_EXIFVERSION, container.getExifVersionString())
		<< writeTag(DB_SOFTWARE, container.getSoftwareString())
		<< writeTag(DB_PHOTOMETRICINTERPRETATION, container.getPhotometricInterpretationString())
		<< writeTag(DB_GPSTIMESTAMP, container.getGpsTimeStampString())
		<< "</AdvancedPhoto>\n"
		<< "<Location>\n"
		// Location
		<< writeTag(DB_LATITUDE, container.getLatitudeString())
		<< writeTag(DB_LONGITUDE, container.getLongitudeString())
		<< writeTag(DB_LOCATION, container.getLoctionString())
		<< writeTag(DB_SCENE, container.getSceneString())// template
		<< "</Location>\n"
		<< "<IPTC>\n"
		// IPTC
		<< writeTag(DB_SOURCEURL, container.getSourceUrlString())		// template
		<< writeTag(DB_USAGERIGHTS, container.getUsageRightsString())	// template
		<< writeTag(DB_COPYRIGHTURL, container.getCopyrightString())    // template
		<< writeTag(DB_HEADLINE, container.getHeadlineString())         // template
		<< writeTag(DB_CATEGORY, container.getCategoryString())			// template
		<< writeTag(DB_SOURCE, container.getSourceString())             // template
		<< writeTag(DB_INSTRUCTIONS, container.getInstructionsString()) // template
		<< writeTag(DB_CREATOR, container.getCreatorString())           // template
		<< writeTag(DB_JOBTITLE, container.getJobTitleString())         // template
		<< writeTag(DB_ADDRESS, container.getAddressString())			// template
		<< writeTag(DB_CITY, container.getCityString())					// template
		<< writeTag(DB_STATE, container.getStateString())				// template
		<< writeTag(DB_POSTALCODE, container.getPostalCodeString())		// template
		<< writeTag(DB_COUNTRY, container.getCountryString())			// template
		<< writeTag(DB_PHONE, container.getPhoneString())				// template
		<< writeTag(DB_EMAIL, container.getEmailString())				// template
		<< writeTag(DB_WEBSITE, container.getWebsiteString())			// template
		<< "</IPTC>\n"
		<< "</Metadata>\n";


	xmlFile.close();

	return true;

}

bool XMLWriter::writeIndexImage(VersionMetadataObject &container, const char *path) {

	std::ofstream xmlFile;
	xmlFile.open(path);
	xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<VersionData>\n"

		<< "<Version>\n"
		<< writeTag(DB_VERSION, container.toString())
		<< writeTag(DB_SEQUENCEID, container.toString())
		<< writeTag(DB_FILENAME, container.toString())
		<< writeTag(DB_ORGINALNAME, container.toString())
		<< writeTag(DB_FILEPATH, container.toString())
		<< writeTag(DB_CRC, container.toString())
		<< writeTag(DB_MD5, container.toString())
		<< writeTag(DB_UUID, container.toString())
		<< "</Version>\n"
		
		<< "</VersionData>\n";


	xmlFile.close();

	return true;

}

} /* namespace simplearchive */
