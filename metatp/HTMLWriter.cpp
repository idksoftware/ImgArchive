#include "HTMLWriter.h"
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

	HTMLWriter::HTMLWriter() {
		// TODO Auto-generated constructor stub

	}

	HTMLWriter::~HTMLWriter() {
		// TODO Auto-generated destructor stub
	}

	std::string HTMLWriter::writeTag(const char *tag, const std::string &value) {
		std::ostringstream html;
		if (value.length() != 0) {
			html << "<tr> <td>" << tag << "</td> <td>" << value << "</td></tr>\n";
		}
		return html.str();
	}

	bool HTMLWriter::writeImage(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"file_block\">\n"
			<< "<table class=\"image_attr\">\n" 
			<< "<Caption>File</Caption>\n"
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
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"description_block\">\n"
			<< "<table class=\"image_attr\">\n" // Description
			<< "<Caption>Description</Caption>\n"
			<< writeTag(DB_DESCRIPTION, container.getDescriptionString())
			<< writeTag(DB_LABEL, container.getLabelString())
			<< writeTag(DB_TITLE, container.getTitleString())
			<< writeTag(DB_SUBJECT, container.getSubjectString())
			<< writeTag(DB_RATING, container.getRatingString())
			<< writeTag(DB_RATINGPERCENT, container.getRatingPercentString())
			<< writeTag(DB_KEYWORDS, container.getKeywordsString()) // template
			<< writeTag(DB_VERSION, container.getVersionString()) // template
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"origin_block\">\n"
			<< "<table class=\"image_attr\">\n" // Origin
			<< "<Caption>Origin</Caption>\n"
			<< writeTag(DB_COMMENT, container.getCommentString())// template
			<< writeTag(DB_AUTHOR, container.getAuthorString())
			<< writeTag(DB_CAPTUREDATE, container.getCaptureDateString())
			<< writeTag(DB_SUBSETIMEORIGINAL, container.getSubjectDIstanceRangeString())
			<< writeTag(DB_COPYRIGHT, container.getCopyrightString())
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"image_block\">\n"
			<< "<table class=\"image_attr\">\n" // Image
			<< "<Caption>Image</Caption>\n"
			<< writeTag(DB_WIDTH, container.getWidthString())
			<< writeTag(DB_HEIGHT, container.getHeightString())
			<< writeTag(DB_VIEWROTATION, container.getViewRotationString())
			<< writeTag(DB_BITSPERSAMPLE, container.getBitsPerSampleString())
			<< writeTag(DB_XRESOLUTION, container.getXResolutionString())
			<< writeTag(DB_YRESOLUTION, container.getYResolutionString())
			<< writeTag(DB_RESOLUTIONUNIT, container.getResolutionUnitString())
			<< writeTag(DB_COMPRESSION, container.getCompressionString())
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"camera_block\">\n"
			<< "<table class=\"image_attr\">\n" // Camera
			<< "<Caption>Camera</Caption>\n"
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
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"advanced_photo_block\">\n"
			<< "<table class=\"image_attr\">\n" // Advanced photo
			<< "<Caption>Advanced photo</Caption>\n"
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
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"location_block\">\n"
			<< "<table class=\"image_attr\">\n"
			// Location
			<< "<Caption>Location</Caption>\n"
			<< writeTag(DB_LATITUDE, container.getLatitudeString())
			<< writeTag(DB_LONGITUDE, container.getLongitudeString())
			<< writeTag(DB_LOCATION, container.getLoctionString())
			<< writeTag(DB_SCENE, container.getSceneString())// template
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"iptc_block\">\n"
			<< "<table class=\"image_attr\">\n"			// IPTC
			<< "<Caption>IPTC</Caption>\n"
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
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();

		return true;

	}

}