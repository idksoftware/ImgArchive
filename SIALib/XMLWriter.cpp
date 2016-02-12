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
	if (!value.empty() && (value.compare("null") != 0)) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	} else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

bool XMLWriter::writeGroup(SideCar &group, const char *path) {
	std::ofstream xmlFile;
		xmlFile.open(path);
		xmlFile <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				<<	"<Group>\n";
		        //<< writeTag("GroupName", group.getName());

		for (std::vector<SideCarItem>::iterator i = group.begin(); i != group.end(); i++) {
			SideCarItem groupItem = *i;
		    xmlFile <<	"<GroupItem>\n"
		            << writeTag("SequenceId", groupItem.getSequenceId().c_str())
		            << writeTag("Filename", groupItem.getFilename().c_str())
		            << writeTag("Filepath", groupItem.getFilepath().c_str())
		            << writeTag("UniqueId", groupItem.getUniqueId().c_str())
		            << writeTag("Md5", groupItem.getMd5().c_str())
		            << writeTag("Crc", groupItem.getCrc().c_str())
		            <<	"</GroupItem>\n";

		            }
			xmlFile <<	"<Group>\n";


		xmlFile.close();

		return true;

	}

bool XMLWriter::writeImage(const MetadataObject &container, const char *path) {

	std::ofstream xmlFile;
	xmlFile.open(path);
	xmlFile <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	            <<	"<Metadata>\n"
	            <<	"<AssetProperties>\n"
	            << writeTag("SequenceId", container.getSequenceId())
	            << writeTag("Filename", container.getFilename())
	            << writeTag("Filepath", container.getFilepath())
	            << writeTag("OrginalName", container.getOrginalName())
	            << writeTag("UniqueId", container.getUniqueId())
	            << writeTag("Label", container.getLabel())
	            << writeTag("Rating", container.getRating())
	            << writeTag("MediaType", container.getMediaType())
	            << writeTag("Md5", container.getMd5())
	            << writeTag("Crc", container.getCrc())
	            << writeTag("FileSize", container.getFileSize())
	            << writeTag("DateCreate", container.getDateCreate())
	            << writeTag("DateModified", container.getDateModified())
	            << writeTag("DateAdded", container.getDateAdded())
	            << "</AssetProperties>\n"
	            << "<MediaProerties>\n"
	            << writeTag("Width", container.getWidth())
	            << writeTag("Height", container.getHeight())
	            << writeTag("Resolution", container.getResolution())
	            << writeTag("Depth", container.getDepth())
	            << writeTag("ViewRotation", container.getViewRotation())
	            << writeTag("SampleColor", container.getSampleColor())
	            << writeTag("Page", container.getPage())
	            << writeTag("ColorSpace", container.getColorSpace())
	            << writeTag("Compression", container.getCompression())
	            << writeTag("PrimaryEncoding", container.getPrimaryEncoding())
	            << "</MediaProerties>\n"
	            << "<CameraInformation>\n"
	            << writeTag("Maker", container.getMaker())
	            << writeTag("Model", container.getModel())
	            << writeTag("Software", container.getSoftware())
	            << writeTag("SourceURL", container.getSourceUrl())
	            << writeTag("ExifVersion", container.getExifVersion())
				<< writeTag("CaptureDate", container.columnAt(DB_CAPTUREDATE).toString())
	            << writeTag("ExposureProgram", container.getExposureProgram())
	            << writeTag("ISOSpeedRating", container.getIsoSpeedRating())
	            << writeTag("ExposureBias", container.getExposureBias())
	            << writeTag("ExposureTime", container.getExposureTime())
	            << writeTag("Aperture", container.getAperture())
	            << writeTag("MeteringMode", container.getMeteringMode())
	            << writeTag("LightSource", container.getLightSource())
	            << writeTag("Flash", container.getFlash())
	            << writeTag("FocalLength", container.getFocalLength())
	            << writeTag("SensingMethod", container.getSensingMethod())
	            << writeTag("DigitalZoom", container.getDigitalZoom())
	            << "</CameraInformation>\n"
	            << "<GPS>\n"
	            << writeTag("Latitude", container.getLatitude())
	            << writeTag("Longitude", container.getLongitude())
	            << writeTag("GPSTimeStamp", container.getGpsTimeStamp())
	            << "</GPS>\n"
	            << "<CopyrightProperties>\n"
	            << writeTag("Copyright", container.getCopyright())
	            << writeTag("UsageRights", container.getUsageRights())
	            << writeTag("CopyrightURL", container.getCopyrightUrl())
	            << "</CopyrightProperties>\n"
	            << "</Metadata>\n";


	xmlFile.close();

	return true;

}
} /* namespace simplearchive */
