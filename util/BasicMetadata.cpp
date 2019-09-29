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
#include <sstream>
#include "BasicMetadata.h"
#include "JpegExif.h"
#include "SimpleExifReader.h"
#include "DBDefines.h"
#include "SAUtils.h"
#include <type_traits>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

BasicMetadataSchema BasicMetadata::m_BasicMetadataSchema;

BasicMetadata::BasicMetadata() : MTRow(m_BasicMetadataSchema) {
	m_exifFound = false;
}
	
BasicMetadata::~BasicMetadata()
{
	//printf("+++ Deleting BasicMetadata +++\n");
}

std::string FloatToString(float fNumber)
{
	std::stringstream os;
	os << fNumber;
	return os.str();
}

std::string IntToString(int iNumber)
{
	std::stringstream os;
	os << iNumber;
	return os.str();
}

/*
template <typename BasicMetadataIndex>
constexpr auto to_underlying(BasicMetadataIndex e)
{
	return static_cast<std::underlying_type_t<E>>(e);
}
*/





void BasicMetadata::copyJpegExif(JpegEXIF &jpegExif) {
	
	if (!jpegExif.title.empty()) columnAt(static_cast<int>(BasicMetadataIndex::BM_TITLE_IDX)) = jpegExif.title;
	//columnAt(static_cast<int>(BasicMetadataIndex::BM_LABEL_IDX)) = jpegExif.label;
	if (!jpegExif.subject.empty()) columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECT_IDX)) = jpegExif.subject;
	if (jpegExif.rating == 0) columnAt(static_cast<int>(BasicMetadataIndex::BM_RATING_IDX)) = jpegExif.rating;
	if (jpegExif.ratingPercent == 0) columnAt(static_cast<int>(BasicMetadataIndex::BM_RATINGPERCENT_IDX)) = jpegExif.ratingPercent;
	if (!jpegExif.ImageDescription.empty()) columnAt(static_cast<int>(BasicMetadataIndex::BM_DESCRIPTION_IDX)) = jpegExif.ImageDescription;
	if (!jpegExif.keywords.empty()) columnAt(static_cast<int>(BasicMetadataIndex::BM_KEYWORDS_IDX)) = jpegExif.keywords;

	if (!jpegExif.comment.empty()) columnAt(static_cast<int>(BasicMetadataIndex::BM_COMMENT_IDX)) = jpegExif.comment;
	if (!jpegExif.author.empty()) columnAt(static_cast<int>(BasicMetadataIndex::BM_AUTHOR_IDX)) = jpegExif.author;
	if (jpegExif.DateTimeDigitized.length() != 0) {
		ExifDateTime edt(jpegExif.DateTimeDigitized.c_str());
		columnAt(static_cast<int>(BasicMetadataIndex::BM_CAPTUREDATE_IDX)) = edt;
	}
	if (jpegExif.SubSecTimeOriginal.length() != 0) {
		ExifDateTime sdt(jpegExif.SubSecTimeOriginal.c_str());
		columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBSETIMEORIGINAL_IDX)) = sdt;
	}
	columnAt(static_cast<int>(BasicMetadataIndex::BM_COPYRIGHT_IDX)) = jpegExif.copyright;

	columnAt(static_cast<int>(BasicMetadataIndex::BM_WIDTH_IDX)) = jpegExif.ImageWidth;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_HEIGHT_IDX)) = jpegExif.ImageHeight;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_VIEWROTATION_IDX)) = jpegExif.Orientation;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_XRESOLUTION_IDX)) = (int)jpegExif.xResolution;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_YRESOLUTION_IDX)) = (int)jpegExif.yResolution;
	ResolutionUnitValue ru = (ResolutionUnitValue)jpegExif.resolutionUnit;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_RESOLUTIONUNIT_IDX)) = JpegEXIF::toString(ru);

	columnAt(static_cast<int>(BasicMetadataIndex::BM_MAKER_IDX)) = jpegExif.Make;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_MODEL_IDX)) = jpegExif.Model;
	std::stringstream et;
	if (jpegExif.ExposureTime.rational < 1) {
		int n = (int)jpegExif.ExposureTime.numerator;
		int d = (int)jpegExif.ExposureTime.denominator;
		et << n << '/' << d;
	}
	else {
		et << jpegExif.ExposureTime.numerator;
	}
	columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSURETIME_IDX)) = et.str().c_str();
	columnAt(static_cast<int>(BasicMetadataIndex::BM_APERTURE_IDX)) = jpegExif.FNumber.rational;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_ISOSPEEDRATING_IDX)) = jpegExif.ISOSpeedRatings;
	if (jpegExif.ExposureBiasValue.denominator != 0) {
		columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSUREBIAS_IDX)) = (double)(jpegExif.ExposureBiasValue.numerator / jpegExif.ExposureBiasValue.denominator);
	}
	else {
		columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSUREBIAS_IDX)) = 0.0;
	}
	ExposureProgramValue ep = (ExposureProgramValue)jpegExif.exposureProgram;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSUREPROGRAM_IDX)) = JpegEXIF::toString(ep);
	columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECTDISTANCE_IDX)) = jpegExif.SubjectDistance;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_FOCALLENGTH_IDX)) = jpegExif.FocalLength;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_FOCALLENGTHIN35MM_IDX)) = jpegExif.FocalLengthIn35mm;
	FlashValues fv = (FlashValues)jpegExif.Flash;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_FLASH_IDX)) = JpegEXIF::toString(fv);
	MeteringModeValue mm = (MeteringModeValue)jpegExif.meteringMode;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_METERINGMODE_IDX)) = JpegEXIF::toString(mm);
	columnAt(static_cast<int>(BasicMetadataIndex::BM_DIGITALZOOM_IDX)) = jpegExif.DigitalZoomRatio;
	LightSourceValues ls = (LightSourceValues)jpegExif.lightSource;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_LIGHTSOURCE_IDX)) = JpegEXIF::toString(ls);
	ColorSpaceValue cs = (ColorSpaceValue)jpegExif.colorSpace;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_COLORSPACE_IDX)) = JpegEXIF::toString(cs);
	WhiteBalanceValue wb = (WhiteBalanceValue)jpegExif.whiteBalance;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_WHITEBALANCE_IDX)) = JpegEXIF::toString(wb);
	SceneCaptureTypeValue sct = (SceneCaptureTypeValue)jpegExif.SceneCaptureType;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_SCENECAPTURETYPE_IDX)) = JpegEXIF::toString(sct);
	ContrastValue cv = (ContrastValue)jpegExif.Contrast;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_CONTRAST_IDX)) = JpegEXIF::toString(cv);
	SaturationValue sv = (SaturationValue)jpegExif.Saturation;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_SATURATION_IDX)) = JpegEXIF::toString(sv);
	SharpnessValue shv = (SharpnessValue)jpegExif.Sharpness;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_SHARPNESS_IDX)) = JpegEXIF::toString(shv);
	columnAt(static_cast<int>(BasicMetadataIndex::BM_BRIGHTNESSVALUE_IDX)) = jpegExif.brightnessValue.rational;
	SubjectDistanceRangeValue sdrv = (SubjectDistanceRangeValue)jpegExif.SubjectDistanceRange;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECTDISTANCERANGE_IDX)) = JpegEXIF::toString(sdrv);
	columnAt(static_cast<int>(BasicMetadataIndex::BM_MAXAPERTURE_IDX)) = jpegExif.maxApertureValue.rational;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_COMPRESSION_IDX)) = jpegExif.compression;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_BITSPERSAMPLE_IDX)) = jpegExif.BitsPerSample;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_COMPRESSEDBITSPERPIXEL_IDX)) = jpegExif.compressedBitsPerPixel.rational;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_FLASHENERGY_IDX)) = jpegExif.flashEnergy;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_EXIFVERSION_IDX)) = jpegExif.exifVersion;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_SOFTWARE_IDX)) = jpegExif.Software;
	columnAt(static_cast<int>(BasicMetadataIndex::BM_PHOTOMETRICINTERPRETATION_IDX)) = jpegExif.photometricInterpretation;

		//columnAt(static_cast<int>(BasicMetadataIndex::BM_LATITUDE_IDX) = jpegExif
		//columnAt(static_cast<int>(BasicMetadataIndex::BM_LONGITUDE_IDX) = jpegExif
		//columnAt(static_cast<int>(BasicMetadataIndex::BM_GPSTIMESTAMP_IDX
	



	// debug print_();
	m_exifFound = true;

	
}



void BasicMetadata::print_() {
	DEBUG_PRINT("Basic Metadata\n");
	DEBUG_PRINT("==============\n\n");
	DEBUG_PRINT("File\n");
	
	
	MTColumn& col1 = columnAt(static_cast<int>(BasicMetadataIndex::BM_FILENAME_IDX));
	DEBUG_PRINT("%s\n", col1.toString().c_str());
	std::cout << "Name: " << col1.toString() << "\n";
	MTColumn& col2 = columnAt(static_cast<int>(BasicMetadataIndex::BM_ORGINALNAME_IDX));
	std::cout << "orginalNames: " << col1.toString() << "\n";
	MTColumn& col3 = columnAt(static_cast<int>(BasicMetadataIndex::BM_FILEPATH_IDX));
	std::cout << "path: " << col3.toString() << "\n";
	MTColumn& col4 = columnAt(static_cast<int>(BasicMetadataIndex::BM_CRC_IDX));
	std::cout << "crc: " << col4.toString() << "\n";
	MTColumn& col5 = columnAt(static_cast<int>(BasicMetadataIndex::BM_MD5_IDX));
	std::cout << "md5: " << col5.toString() << "\n";
	MTColumn& col6 = columnAt(static_cast<int>(BasicMetadataIndex::BM_UUID_IDX));
	std::cout << "uuid: " << col6.toString() << "\n";
	MTColumn& col7 = columnAt(static_cast<int>(BasicMetadataIndex::BM_MEDIATYPE_IDX));
	std::cout << "mediaType: " << col7.toString() << "\n";
	MTColumn& col8 = columnAt(static_cast<int>(BasicMetadataIndex::BM_FILESIZE_IDX));
	std::cout << "size: " << col8.toString() << "\n";
	MTColumn& col9 = columnAt(static_cast<int>(BasicMetadataIndex::BM_DATEMODIFIED_IDX));
	auto modtime = getModTime();
	std::cout << "Mod Time: " << col9.toString() << "\n";
	MTColumn& col10 = columnAt(static_cast<int>(BasicMetadataIndex::BM_DATECREATE_IDX));
	auto createtime = getCreateTime();
	std::cout << "Create Time: " << col10.toString() << "\n";

	DEBUG_PRINT("\n\nDescription\n");
	MTColumn& col11 = columnAt(static_cast<int>(BasicMetadataIndex::BM_LABEL_IDX));
	std::cout << "label: " << col11.toString() << "\n";
	MTColumn& col12 = columnAt(static_cast<int>(BasicMetadataIndex::BM_TITLE_IDX));
	std::cout << "title: " << col12.toString() << "\n";
	MTColumn& col13 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECT_IDX));
	std::cout << "subject: " << col13.toString() << "\n";
	MTColumn& col14 = columnAt(static_cast<int>(BasicMetadataIndex::BM_RATING_IDX));
	std::cout << "rating: " << col14.toString() << "\n";
	MTColumn& col15 = columnAt(static_cast<int>(BasicMetadataIndex::BM_RATINGPERCENT_IDX));
	std::cout << "ratingPercent: " << col15.toString() << "\n";
	MTColumn& col16 = columnAt(static_cast<int>(BasicMetadataIndex::BM_DESCRIPTION_IDX));
	std::cout << "Image Description: " << col16.toString() << "\n";
	MTColumn& col17 = columnAt(static_cast<int>(BasicMetadataIndex::BM_KEYWORDS_IDX));
	std::cout << "keywords: " << col17.toString() << "\n";	
	MTColumn& col18 = columnAt(static_cast<int>(BasicMetadataIndex::BM_COMMENT_IDX));
	std::cout << "comment: " << col18.toString() << "\n";
	MTColumn& col19 = columnAt(static_cast<int>(BasicMetadataIndex::BM_AUTHOR_IDX));
	std::cout << "author: " << col19.toString() << "\n";
	MTColumn& col20 = columnAt(static_cast<int>(BasicMetadataIndex::BM_CAPTUREDATE_IDX));
	auto dateTimeDigitized = col20.getDate();
	std::cout << "Date time digitized: " << dateTimeDigitized.toString() << "\n";
	MTColumn& col21 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBSETIMEORIGINAL_IDX));
	std::cout << "Subsec time original: " << col21.toString() << "\n";
	MTColumn& col22 = columnAt(static_cast<int>(BasicMetadataIndex::BM_COPYRIGHT_IDX));
	std::cout << "Copyright: " << col22.toString() << "\n";
	
	
	DEBUG_PRINT("\n\nImage\n");
	MTColumn& col23 = columnAt(static_cast<int>(BasicMetadataIndex::BM_WIDTH_IDX));
	std::cout << "Image width: " << col23.toString() << "\n";
	MTColumn& col24 = columnAt(static_cast<int>(BasicMetadataIndex::BM_HEIGHT_IDX));
	std::cout << "Image height: " << col24.toString() << "\n";
	MTColumn& col25 = columnAt(static_cast<int>(BasicMetadataIndex::BM_VIEWROTATION_IDX));
	std::cout << "Orientation: " << col25.toString() << "\n";
	MTColumn& col26 = columnAt(static_cast<int>(BasicMetadataIndex::BM_XRESOLUTION_IDX));
	std::cout << "X Resolution: " << col26.toString() << "\n";
	MTColumn& col27 = columnAt(static_cast<int>(BasicMetadataIndex::BM_YRESOLUTION_IDX));
	std::cout << "Y Resolution: " << col27.toString() << "\n";
	MTColumn& col28 = columnAt(static_cast<int>(BasicMetadataIndex::BM_RESOLUTIONUNIT_IDX));
	std::cout << "Resolution unit: " << col28 .toString() << "\n";
	DEBUG_PRINT("\n\nCamera\n");
	MTColumn& col29 = columnAt(static_cast<int>(BasicMetadataIndex::BM_MAKER_IDX));
	std::cout << "Make: " << col29.toString() << "\n";
	MTColumn& col30 = columnAt(static_cast<int>(BasicMetadataIndex::BM_MODEL_IDX));
	std::cout << "Model: " << col30.toString() << "\n";
	MTColumn& col31 = columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSURETIME_IDX));
	std::cout << "Exposure time: " << col31.toString() << "\n";
	MTColumn& col32 = columnAt(static_cast<int>(BasicMetadataIndex::BM_APERTURE_IDX));
	std::cout << "F Number: " << col32.toString() << "\n";
	MTColumn& col33 = columnAt(static_cast<int>(BasicMetadataIndex::BM_ISOSPEEDRATING_IDX));
	std::cout << "ISO speed ratings: " << col33.toString() << "\n";
	MTColumn& col34 = columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSUREBIAS_IDX));
	std::cout << "Exposure bias value: " << col34.toString() << "\n";
	MTColumn& col35 = columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSUREPROGRAM_IDX));
	std::cout << "Exposure program: " << col35.toString() << "\n";
	MTColumn& col36 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECTDISTANCERANGE_IDX));
	std::cout << "Subject distance: " << col36.toString() << "\n";
	MTColumn& col37 = columnAt(static_cast<int>(BasicMetadataIndex::BM_FOCALLENGTH_IDX));
	std::cout << "Focal length: " << col37.toString() << "\n";
	MTColumn& col38 = columnAt(static_cast<int>(BasicMetadataIndex::BM_FOCALLENGTHIN35MM_IDX));
	std::cout << "Focal length in 35mm: " << col38.toString() << "\n";
	MTColumn& col39 = columnAt(static_cast<int>(BasicMetadataIndex::BM_FLASH_IDX));
	std::cout << "Flash: " << col39.toString() << "\n";
	MTColumn& col40 = columnAt(static_cast<int>(BasicMetadataIndex::BM_METERINGMODE_IDX));
	std::cout << "Metering mode: " << col40.toString() << "\n";
	MTColumn& col41 = columnAt(static_cast<int>(BasicMetadataIndex::BM_DIGITALZOOM_IDX));
	std::cout << "Digital zoom ratio: " << col41.toString() << "\n";
	DEBUG_PRINT("\n\nAdvanced photo\n");
	MTColumn& col42 = columnAt(static_cast<int>(BasicMetadataIndex::BM_LIGHTSOURCE_IDX));
	std::cout << "Light source: " << col42.toString() << "\n";
	MTColumn& col43 = columnAt(static_cast<int>(BasicMetadataIndex::BM_COLORSPACE_IDX));
	std::cout << "Color space: " << col43.toString() << "\n";
	MTColumn& col44 = columnAt(static_cast<int>(BasicMetadataIndex::BM_WHITEBALANCE_IDX));
	std::cout << "White balance: " << col44.toString() << "\n";
	MTColumn& col45 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SCENECAPTURETYPE_IDX));
	std::cout << "Scene capture type: " << col45.toString() << "\n";
	MTColumn& col46 = columnAt(static_cast<int>(BasicMetadataIndex::BM_CONTRAST_IDX));
	std::cout << "Contrast: " << col46.toString() << "\n";
	MTColumn& col47 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SATURATION_IDX));
	std::cout << "Saturation: " << col47.toString() << "\n";
	MTColumn& col48 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SHARPNESS_IDX));
	std::cout << "Sharpness: " << col48.toString() << "\n";
	MTColumn& col49 = columnAt(static_cast<int>(BasicMetadataIndex::BM_BRIGHTNESSVALUE_IDX));
	std::cout << "Brightness value: " << col49.toString() << "\n";
	MTColumn& col50 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECTDISTANCERANGE_IDX));
	std::cout << "Subject distance range: " << col50.toString() << "\n";
	MTColumn& col51 = columnAt(static_cast<int>(BasicMetadataIndex::BM_MAXAPERTURE_IDX));
	std::cout << "MAX Aperture value: " << col51.toString() << "\n";
	MTColumn& col52 = columnAt(static_cast<int>(BasicMetadataIndex::BM_COMPRESSION_IDX));
	std::cout << "Compression: " << col52.toString() << "\n";
	MTColumn& col53 = columnAt(static_cast<int>(BasicMetadataIndex::BM_BITSPERSAMPLE_IDX));
	std::cout << "Bits per sample: " << col53.toString() << "\n";
	MTColumn& col54 = columnAt(static_cast<int>(BasicMetadataIndex::BM_COMPRESSEDBITSPERPIXEL_IDX));
	std::cout << "Compressed bits per pixel: " << col54.toString() << "\n";
	MTColumn& col55 = columnAt(static_cast<int>(BasicMetadataIndex::BM_FLASHENERGY_IDX));
	std::cout << "Flash energy: " << col55.toString() << "\n";
	MTColumn& col56 = columnAt(static_cast<int>(BasicMetadataIndex::BM_EXIFVERSION_IDX));
	std::cout << "Exif version: " << col56.toString() << "\n";
	MTColumn& col57 = columnAt(static_cast<int>(BasicMetadataIndex::BM_SOFTWARE_IDX));
	std::cout << "Software: " << col57.toString() << "\n";
	MTColumn& col58 = columnAt(static_cast<int>(BasicMetadataIndex::BM_PHOTOMETRICINTERPRETATION_IDX));
	std::cout << "Photometric interpretation: " << col58.toString() << "\n";
}

/*
void BasicMetadata::print() {
	printf("Basic Metadata\n");
	printf("==============\n\n");
	printf("File\n");
	printf("Name %s\n", this->getName().c_str());
	printf("orginalName %s\n", this->getOrginalName().c_str());
	printf("path %s\n", this->getPath().c_str());
	printf("crc %d\n", this->getCrc());
	printf("md5 %s\n", this->getMd5().c_str());
	printf("uuid %s\n", this->getUuid().c_str());
	printf("mediaType %s\n", this->getMediaType().c_str());
	printf("size %d\n", this->getSize());
	auto modtime = getModTime();
	printf("Mod Time %s\n", modtime.toString().c_str());
	auto createtime = getCreateTime();
	printf("Create Time %s\n", createtime.toString().c_str());

	printf("\n\nDescription\n");
	printf("label %s\n", this->getLabel().c_str());
	//printf("title %s\n", this->get.c_str());
	//printf("subject %s\n", this->get.c_str());
	printf("rating %d\n", this->getRating());
	//printf("ratingPercent %d\n", this->getRatingPercent());
	printf("Image Description %s\n", this->getImageDescription().c_str());
	//printf("keywords %d\n", this->getKeywords().c_str());
	printf("\n\nOrigin\n");
	//printf("comment %d\n", this->getComment().c_str());
	//printf("author %d\n", this->getAuthor().c_str());
	auto dateTimeDigitized = this->getDateTimeDigitized();
	printf("Date time digitized %s\n", dateTimeDigitized.toString().c_str());
	printf("Subsec time original %s\n", this->getSubSecTimeOriginal().c_str());
	printf("Copyright %s\n", this->getCopyright().c_str());
	printf("\n\nImage\n");
	printf("Image width %s\n", this->getImageWidth().c_str());
	printf("Image height %s\n", this->getImageHeight().c_str());
	printf("Orientation %d\n", this->getOrientation().c_str());
	printf("X Resolution %s\n", this->getXResolution().c_str());
	printf("Y Resolution %s\n", this->getYResolution().c_str());
	printf("Resolution unit%s\n", this->getResolutionUnit().c_str());
	printf("\n\nCamera\n");
	printf("Make %s\n", this->getMake().c_str());
	printf("Model %s\n", this->getModel().c_str());
	printf("Exposure time %s\n", this->getExposureTime().c_str());
	printf("F Number %s\n", this->getFNumber().c_str());
	printf("ISO speed ratings %s\n", this->getISoSpeedRatings().c_str());
	printf("Exposure bias value %s\n", this->getExposureBiasValue().c_str());
	printf("Exposure program %s\n", this->getExposureProgram().c_str());
	printf("Subject distance %s\n", this->getSubjectDistance().c_str());
	printf("Focal length %s\n", this->getFocalLength().c_str());
	printf("Focal length in 35mm %s\n", this->getFocalLengthIn35mm().c_str());
	printf("Flash %s\n", this->getFlash().c_str());
	printf("Metering mode %s\n", this->getMeteringMode().c_str());
	printf("Digital zoom ratio %s\n", this->getDigitalZoomRatio().c_str());
	printf("\n\nAdvanced photo\n");
	printf("Light source %s\n", this->getLightSource().c_str());
	printf("Color space %s\n", this->getColorSpace().c_str());
	printf("White balance %s\n", this->getWhiteBalance().c_str());
	printf("Scene capture type %s\n", this->getSceneCaptureType().c_str());
	printf("Contrast %s\n", this->getContrast().c_str());
	printf("Saturation %s\n", this->getSaturation().c_str());
	printf("Sharpness %s\n", this->getSharpness().c_str());
	printf("Brightness value %s\n", this->getBrightnessValue().c_str());
	printf("Subject distance range %s\n", this->getSubjectDistanceRange().c_str());
	printf("MAX Aperture value %s\n", this->getMAXApertureValue().c_str());
	printf("Compression %s\n", this->getCompression().c_str());
	printf("Bits per sample %d\n", this->getBitsPerSample());
	printf("Compressed bits per pixel %d\n", this->getCompressedBitsPerPixel());
	printf("Flash energy %d\n", this->getFlashEnergy());
	printf("Exif version %d\n", this->getExifVersion());
	printf("Software %d\n", this->getSoftware());
	printf("Photometric interpretation %d\n", this->getPhotometricInterpretation());
}
*/



unsigned long BasicMetadata::getCrc() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_CRC_IDX)).getULong();
}

void BasicMetadata::setCrc(unsigned long crc) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_CRC_IDX)) = crc;
}

unsigned long BasicMetadata::getSize() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_FILESIZE_IDX)).getULong();
}

void BasicMetadata::setSize(unsigned long size) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_FILESIZE_IDX)) = size;
}

const std::string& BasicMetadata::getMd5() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_MD5_IDX)).getString();
}

void BasicMetadata::setMd5(const std::string& md5) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_MD5_IDX)).fromString(md5);
}

const std::string& BasicMetadata::getMediaType() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_MEDIATYPE_IDX)).getString();
}

void BasicMetadata::setMediaType(const std::string& mediaType) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_MEDIATYPE_IDX)) = mediaType.c_str();
}

const std::string& BasicMetadata::getUuid() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_UUID_IDX)).getString();
}

void BasicMetadata::setUuid(const std::string& uuid) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_UUID_IDX)) = uuid.c_str();
}

const std::string& BasicMetadata::getLabel() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_LABEL_IDX)).getString();
}

void BasicMetadata::setLabel(const std::string& name) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_LABEL_IDX)) = name.c_str();
}

const std::string& BasicMetadata::getName() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_FILENAME_IDX)).getString();
}

void BasicMetadata::setName(const std::string& name) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_FILENAME_IDX)) = name.c_str();
}

const std::string& BasicMetadata::getOrginalName() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_ORGINALNAME_IDX)).getString();
}

void BasicMetadata::setOrginalName(const std::string& name) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_ORGINALNAME_IDX)) = name.c_str();
}

const std::string& BasicMetadata::getPath() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_FILEPATH_IDX)).getString();
}

void BasicMetadata::setPath(const std::string& path) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_FILEPATH_IDX)) = path.c_str();
}

const int BasicMetadata::getRating() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_RATING_IDX)).getInt();
}
void BasicMetadata::setRating(const int rating) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_RATING_IDX)) = rating;
}

const std::string BasicMetadata::getExt() const {
	return m_path.substr(m_path.find_last_of(".") + 1);
}

void BasicMetadata::setCreateTime(ExifDateTime &time) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_DATECREATE_IDX)) = time;
}

const ExifDateTime &BasicMetadata::getCreateTime() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_DATECREATE_IDX)).getDate();
}

const ExifDateTime &BasicMetadata::getModTime() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_DATEMODIFIED_IDX)).getDate();
}

void BasicMetadata::setModTime(ExifDateTime &time) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_DATEMODIFIED_IDX)) = time;
}

const ExifDateTime &BasicMetadata::getAddTime() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_DATEADDED_IDX)).getDate();
}

void BasicMetadata::setAddTime(ExifDateTime &time) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_DATEADDED_IDX)) = time;
}

void BasicMetadata::setVersion(int v) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_VERSION_IDX)) = v;
}

const int BasicMetadata::getVersion() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_VERSION_IDX)).getInt();
}

/*
const std::string& BasicMetadata::getAltitude() const {
	return ""; // columnAt(static_cast<int>(BasicMetadataIndex::BM_ALTITUDE_IDX)).toString();
}

void BasicMetadata::setAltitude(const std::string& altitude) {
	m_altitude = altitude;
}
*/

const std::string& BasicMetadata::getBitsPerSample() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_BITSPERSAMPLE_IDX)).getString();
}

//void BasicMetadata::setBitsPerSample(const std::string& bitsPerSample) {
//	m_bitsPerSample = bitsPerSample;
//}

const std::string& BasicMetadata::getCopyright() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_COPYRIGHT_IDX)).getString();
}

void BasicMetadata::setCopyright(const std::string& copyright) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_COPYRIGHT_IDX)) = copyright.c_str();
}

//const std::string& BasicMetadata::getDateTime() const {
//	return m_dateTime;
//}

//void BasicMetadata::setDateTime(const std::string& dateTime) {
//	m_dateTime = dateTime;
//}

const ExifDateTime &BasicMetadata::getDateTimeDigitized() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_CAPTUREDATE_IDX)).getDate();
}

void BasicMetadata::setDateTimeDigitized(ExifDateTime& dateTimeDigitized) {
	columnAt(static_cast<int>(BasicMetadataIndex::BM_CAPTUREDATE_IDX)) = dateTimeDigitized;
}

//const std::string& BasicMetadata::getDateTimeOriginal() const {
//	return m_dateTimeOriginal;
//}

//void BasicMetadata::setDateTimeOriginal(const std::string& dateTimeOriginal) {
//	m_dateTimeOriginal = dateTimeOriginal;
//}

const std::string& BasicMetadata::getExposureBiasValue() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSUREBIAS_IDX)).getString();
}

const std::string& BasicMetadata::getExposureProgram() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSUREPROGRAM_IDX)).getString();
}


void BasicMetadata::setExposureBiasValue(const std::string& exposureBiasValue) {
	//m_exposureBiasValue = exposureBiasValue;
}

const std::string& BasicMetadata::getExposureTime() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_EXPOSURETIME_IDX)).getString();
}

void BasicMetadata::setExposureTime(const std::string& exposureTime) {
	//m_exposureTime = exposureTime;
}

const std::string& BasicMetadata::getFlash() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_FLASH_IDX)).getString();
}

void BasicMetadata::setFlash(const std::string& flash) {
//	m_flash = flash;
}

const std::string& BasicMetadata::getFNumber() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_APERTURE_IDX)).getString();
}

void BasicMetadata::setFNumber(const std::string& fNumber) {
//	m_fNumber = fNumber;
}

const std::string& BasicMetadata::getFocalLength() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_FOCALLENGTH_IDX)).getString();
}

void BasicMetadata::setFocalLength(const std::string& focalLength) {
	//m_focalLength = focalLength;
}

const std::string& BasicMetadata::getFocalLengthIn35mm() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_FOCALLENGTHIN35MM_IDX)).getString();
}

//void BasicMetadata::setFocalLengthIn35mm(const std::string& focalLengthIn35mm) {
//	m_focalLengthIn35mm = focalLengthIn35mm;
//}

const std::string& BasicMetadata::getImageDescription() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_DESCRIPTION_IDX)).getString();
}

void BasicMetadata::setImageDescription(const std::string& imageDescription) {
	//m_imageDescription = imageDescription;
}

const std::string& BasicMetadata::getImageHeight() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_HEIGHT_IDX)).getString();
}

void BasicMetadata::setImageHeight(const std::string& imageHeight) {
	//m_imageHeight = imageHeight;
}

const std::string& BasicMetadata::getImageWidth() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_WIDTH_IDX)).getString();
}

void BasicMetadata::setImageWidth(const std::string& imageWidth) {
	//m_imageWidth = imageWidth;
}

const std::string& BasicMetadata::getXResolution() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_XRESOLUTION_IDX)).getString();
}

const std::string& BasicMetadata::getYResolution() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_YRESOLUTION_IDX)).getString();
}

const std::string& BasicMetadata::getResolutionUnit() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_RESOLUTIONUNIT_IDX)).getString();
}

const std::string& BasicMetadata::getISoSpeedRatings() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_ISOSPEEDRATING_IDX)).getString();
}

void BasicMetadata::setISoSpeedRatings(const std::string& iSoSpeedRatings) {
	//m_iSOSpeedRatings = iSoSpeedRatings;
}

const std::string& BasicMetadata::getLatitude() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_LATITUDE_IDX)).getString();
}

void BasicMetadata::setLatitude(const std::string& latitude) {
	m_latitude = latitude;
}

const std::string& BasicMetadata::getLongitude() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_LONGITUDE_IDX)).getString();
}

void BasicMetadata::setLongitude(const std::string& longitude) {
	m_longitude = longitude;
}

const std::string& BasicMetadata::getMake() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_MAKER_IDX)).getString();
}

void BasicMetadata::setMake(const std::string& make) {
	m_make = make;
}

const std::string& BasicMetadata::getMeteringMode() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_METERINGMODE_IDX)).getString();
}

void BasicMetadata::setMeteringMode(const std::string& meteringMode) {
	//m_meteringMode = meteringMode;
}

const std::string& BasicMetadata::getModel() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_MODEL_IDX)).getString();
}

void BasicMetadata::setModel(const std::string& model) {
	m_model = model;
}

const std::string& BasicMetadata::getOrientation() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_VIEWROTATION_IDX)).getString();
}

void BasicMetadata::setOrientation(const std::string& orientation) {
	//m_orientation = orientation;
}

//const std::string& BasicMetadata::getShutterSpeedValue() const {
//	return m_shutterSpeedValue;
//}

//void BasicMetadata::setShutterSpeedValue(const std::string& shutterSpeedValue) {
//	m_shutterSpeedValue = shutterSpeedValue;
//}

const std::string& BasicMetadata::getSoftware() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_SOFTWARE_IDX)).getString();
}

void BasicMetadata::setSoftware(const std::string& software) {
	m_software = software;
}

const std::string& BasicMetadata::getSubjectDistance() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECTDISTANCE_IDX)).getString();
}

//void BasicMetadata::setSubjectDistance(const std::string& subjectDistance) {
//	return columnAt(static_cast<int>(BasicMetadataIndex::BM_MEDIATYPE_IDX)).getString();
//}

const std::string& BasicMetadata::getSubSecTimeOriginal() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBSETIMEORIGINAL_IDX)).getString();
}

//void BasicMetadata::setSubSecTimeOriginal(const std::string& subSecTimeOriginal) {
//	m_subSecTimeOriginal = subSecTimeOriginal;
//}

const std::string& BasicMetadata::getDigitalZoomRatio() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_DIGITALZOOM_IDX)).getString();
}
const std::string& BasicMetadata::getLightSource() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_LIGHTSOURCE_IDX)).getString();
}

const std::string& BasicMetadata::getColorSpace() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_COLORSPACE_IDX)).getString();
}

const std::string& BasicMetadata::getWhiteBalance() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_WHITEBALANCE_IDX)).getString();
}

const std::string& BasicMetadata::getSceneCaptureType() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_SCENECAPTURETYPE_IDX)).getString();
}

const std::string& BasicMetadata::getContrast() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_CONTRAST_IDX)).getString();
}

const std::string& BasicMetadata::getSaturation() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_SATURATION_IDX)).getString();
}

const std::string& BasicMetadata::getSharpness() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_SHARPNESS_IDX)).getString();
}

const std::string& BasicMetadata::getBrightnessValue() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_BRIGHTNESSVALUE_IDX)).getString();
}

const std::string& BasicMetadata::getSubjectDistanceRange() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_SUBJECTDISTANCERANGE_IDX)).getString();
}

const std::string& BasicMetadata::getMAXApertureValue() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_MAXAPERTURE_IDX)).getString();
}

const std::string& BasicMetadata::getCompression() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_COMPRESSION_IDX)).getString();
}


const std::string& BasicMetadata::getCompressedBitsPerPixel() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_COMPRESSEDBITSPERPIXEL_IDX)).getString();
}



const std::string& BasicMetadata::getFlashEnergy() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_FLASHENERGY_IDX)).getString();
}


const std::string& BasicMetadata::getExifVersion() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_EXIFVERSION_IDX)).getString();
}


const std::string& BasicMetadata::getPhotometricInterpretation() const {
	return columnAt(static_cast<int>(BasicMetadataIndex::BM_PHOTOMETRICINTERPRETATION_IDX)).getString();
}

} /* namespace simplearchive */
