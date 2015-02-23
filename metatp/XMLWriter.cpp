/*
 * XMLWriter.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: wzw7yn
 */

#include "XMLWriter.h"
#include "ImageSideCar.h"
#include "MetadataObject.h"
#include <sstream>
#include <iostream>
#include <fstream>

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
	xmlFile <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	            <<	"<Metadata>\n"
	            <<	"<AssetProperties>\n"
	            << writeTag("SequenceId", container.getSequenceId())
	           // << writeTag("Filename", container.getFilename())
	           // << writeTag("Filepath", container.getFilepath())
	           // << writeTag("OrginalName", container.getOrginalName())
	           // << writeTag("UniqueId", container.getUniqueId())
	            << writeTag("Label", container.getLabel())
	            << writeTag("Rating", container.getRating())
	           // << writeTag("MediaType", container.getMediaType())
	           // << writeTag("Md5", container.getMd5())
	           // << writeTag("Crc", container.getCrc())
	           // << writeTag("FileSize", container.getFileSize())
	           // << writeTag("DateCreate", container.getDateCreate())
	           // << writeTag("DateModified", container.getDateModified())
	           //<< writeTag("DateAdded", container.getDateAdded())
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
	            << writeTag("PrinaryEncoding", container.getPrinaryEncoding())
	            << "</MediaProerties>\n"
	            << "<CameraInformation>\n"
	            << writeTag("Maker", container.getMaker())
	            << writeTag("Model", container.getModel())
	            << writeTag("Software", container.getSoftware())
	            << writeTag("SourceURL", container.getSourceUrl())
	            << writeTag("ExifVersion", container.getExifVersion())
	            << writeTag("CaptureDate", container.getCaptureDate())
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