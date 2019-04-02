#include "JSONWriter.h"
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

	JSONWriter::JSONWriter() {
		// TODO Auto-generated constructor stub

	}

	JSONWriter::~JSONWriter() {
		// TODO Auto-generated destructor stub
	}

	std::string JSONWriter::writeTag(const char *tag, const std::string &value, bool end) {
		std::ostringstream json;
		if (end) {
			json << '\"' << tag << "\": \"" << value << "\"\n";
		}
		else {
			json << '\"' << tag << "\": \"" << value << "\",\n";
		}
		return json.str();
	}

	bool JSONWriter::writeImage(MetadataObject &container, const char *path) {

		std::ofstream jsonFile;
		jsonFile.open(path);
		jsonFile << "{\n"
			<< "	\"File\": {\n"
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
			<< writeTag(DB_DATEADDED, container.getAddedDateString(), true)
			<< "	},\n"
			<< "	\"Description\": {\n"
			<< writeTag(DB_DESCRIPTION, container.getDescriptionString())
			<< writeTag(DB_LABEL, container.getLabelString())
			<< writeTag(DB_TITLE, container.getTitleString())
			<< writeTag(DB_SUBJECT, container.getSubjectString())
			<< writeTag(DB_RATING, container.getRatingString())
			<< writeTag(DB_RATINGPERCENT, container.getRatingPercentString())
			<< writeTag(DB_KEYWORDS, container.getKeywordsString()) // template
			<< writeTag(DB_VERSION, container.getVersionString(), true) // template
			<< "	},\n"
			<< "	\"Origin\": {\n"
			<< writeTag(DB_COMMENT, container.getCommentString())// template
			<< writeTag(DB_AUTHOR, container.getAuthorString())
			<< writeTag(DB_CAPTUREDATE, container.getCaptureDateString())
			<< writeTag(DB_SUBSETIMEORIGINAL, container.getSubjectDIstanceRangeString())
			<< writeTag(DB_COPYRIGHT, container.getCopyrightString(), true)
			<< "	},\n"
			<< "	\"Image\": {\n"
			// Image
			<< writeTag(DB_WIDTH, container.getWidthString())
			<< writeTag(DB_HEIGHT, container.getHeightString())
			<< writeTag(DB_VIEWROTATION, container.getViewRotationString())
			<< writeTag(DB_BITSPERSAMPLE, container.getBitsPerSampleString())
			<< writeTag(DB_XRESOLUTION, container.getXResolutionString())
			<< writeTag(DB_YRESOLUTION, container.getYResolutionString())
			<< writeTag(DB_RESOLUTIONUNIT, container.getResolutionUnitString())
			<< writeTag(DB_COMPRESSION, container.getCompressionString(), true)
			<< "	},\n"
			<< "	\"Camera\": {\n"
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
			<< writeTag(DB_DIGITALZOOM, container.getDigitalZoomString(), true)
			<< "	},\n"
			<< "	\"AdvancedPhoto\": {\n"
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
			<< writeTag(DB_GPSTIMESTAMP, container.getGpsTimeStampString(), true)
			<< "	},\n"
			<< "	\"Location\": {\n"
			// Location
			<< writeTag(DB_LATITUDE, container.getLatitudeString())
			<< writeTag(DB_LONGITUDE, container.getLongitudeString())
			<< writeTag(DB_LOCATION, container.getLoctionString())
			<< writeTag(DB_SCENE, container.getSceneString(), true)// template
			<< "	},\n"
			<< "	\"IPTC\": {\n"
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
			<< writeTag(DB_WEBSITE, container.getWebsiteString(),true)			// template
			<< "	},\n"
			<< "}\n";


		jsonFile.close();

		return true;

	}

}