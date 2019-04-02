#pragma once

#include "DBLite.h"
#include "DBDefines.h"

using namespace DBLite;

namespace SIADBLite {
	

	class DerivativeMetadataSchema : public DBLiteTableSchema {
	public:
		DerivativeMetadataSchema() : DBLiteTableSchema("DerivativeMetadata") {
			add(DBLiteSchema(DBLiteSchema::Integer, DB_SEQUENCEID));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_VERSION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_DATABASEID));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FILENAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_ORGINALNAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FILEPATH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_CRC));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MD5));
			add(DBLiteSchema(DBLiteSchema::Text, DB_UUID));


			add(DBLiteSchema(DBLiteSchema::Integer, DB_FILESIZE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEMODIFIED));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATECREATE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEADDED));

			add(DBLiteSchema(DBLiteSchema::Text, DB_COMMENT));
			add(DBLiteSchema(DBLiteSchema::Text, DB_AUTHOR));
			add(DBLiteSchema(DBLiteSchema::Text, DB_DESCRIPTION));
			add(DBLiteSchema(DBLiteSchema::Date, DB_EDITOR));
		}
	};


	class ExifObjectSchema : public DBLiteTableSchema {
	public:
		ExifObjectSchema() : DBLiteTableSchema(ROW_EXTERNAL_EXIF) {

			add(DBLiteSchema(DBLiteSchema::Text, DB_ORGINALNAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LABEL));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_RATING));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MEDIATYPE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATECREATE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_DESCRIPTION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_WIDTH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_HEIGHT));
			add(DBLiteSchema(DBLiteSchema::Text, DB_RESOLUTION));
			//			add(DBLiteSchema(DBLiteSchema::Text, DB_DEPTH));
			add(DBLiteSchema(DBLiteSchema::Text, DB_VIEWROTATION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SAMPLECOLOR));
			//			add(DBLiteSchema(DBLiteSchema::Text, DB_PAGE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COLORSPACE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COMPRESSION));
			//			add(DBLiteSchema(DBLiteSchema::Text, DB_PRIMARYENCODING));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MAKER));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MODEL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SOFTWARE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SOURCEURL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXIFVERSION));
			add(DBLiteSchema(DBLiteSchema::Date, DB_CAPTUREDATE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXPOSUREPROGRAM));
			add(DBLiteSchema(DBLiteSchema::Text, DB_ISOSPEEDRATING));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXPOSUREBIAS));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXPOSURETIME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_APERTURE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_METERINGMODE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LIGHTSOURCE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FLASH));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FOCALLENGTH));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SENSINGMETHOD));
			add(DBLiteSchema(DBLiteSchema::Text, DB_DIGITALZOOM));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LATITUDE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LONGITUDE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_GPSTIMESTAMP));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COPYRIGHT));
			add(DBLiteSchema(DBLiteSchema::Text, DB_USAGERIGHTS));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COPYRIGHTURL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MEDIATYPE));
		}
	};

	class BasicMetadataSchema : public DBLiteTableSchema {
	public:
		BasicMetadataSchema() : DBLiteTableSchema(ROW_BASIC_EXIF) {



			add(DBLiteSchema(DBLiteSchema::Text, DB_FILENAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_ORGINALNAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FILEPATH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_CRC));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MD5));
			add(DBLiteSchema(DBLiteSchema::Text, DB_UUID));

			add(DBLiteSchema(DBLiteSchema::Text, DB_MEDIATYPE));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_FILESIZE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEMODIFIED));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATECREATE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEADDED));

			add(DBLiteSchema(DBLiteSchema::Text, DB_LABEL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_TITLE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SUBJECT));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_RATING));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_RATINGPERCENT));
			add(DBLiteSchema(DBLiteSchema::Text, DB_DESCRIPTION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_KEYWORDS));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_VERSION));

			add(DBLiteSchema(DBLiteSchema::Text, DB_COMMENT));
			add(DBLiteSchema(DBLiteSchema::Text, DB_AUTHOR));
			add(DBLiteSchema(DBLiteSchema::Date, DB_CAPTUREDATE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_SUBSETIMEORIGINAL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COPYRIGHT));

			add(DBLiteSchema(DBLiteSchema::Integer, DB_WIDTH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_HEIGHT));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_VIEWROTATION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_XRESOLUTION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_YRESOLUTION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_RESOLUTIONUNIT));

			add(DBLiteSchema(DBLiteSchema::Text, DB_MAKER));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MODEL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXPOSURETIME));
			add(DBLiteSchema(DBLiteSchema::Float, DB_APERTURE));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_ISOSPEEDRATING));
			add(DBLiteSchema(DBLiteSchema::Float, DB_EXPOSUREBIAS));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXPOSUREPROGRAM));
			add(DBLiteSchema(DBLiteSchema::Float, DB_SUBJECTDISTANCE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_FOCALLENGTH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_FOCALLENGTHIN35MM));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FLASH));
			add(DBLiteSchema(DBLiteSchema::Text, DB_METERINGMODE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_DIGITALZOOM));

			add(DBLiteSchema(DBLiteSchema::Text, DB_LIGHTSOURCE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COLORSPACE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_WHITESBALANCE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SCENECAPTURETYPE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_CONTRAST));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SATURATION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SHARPNESS));
			add(DBLiteSchema(DBLiteSchema::Float, DB_BRIGHTNESSVALUE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SUBJECTDISTANCERANGE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_MAXAPERTURE));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_COMPRESSION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_BITSPERSAMPLE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_COMPRESSEDBITSPERPIXEL));
			add(DBLiteSchema(DBLiteSchema::Float, DB_FLASHENERGY));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXIFVERSION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SOFTWARE));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_PHOTOMETRICINTERPRETATION));

			add(DBLiteSchema(DBLiteSchema::Text, DB_LATITUDE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LONGITUDE));
		}
	};

	class MetadataSchema : public DBLiteTableSchema {
	public:
		MetadataSchema() : DBLiteTableSchema("Metadata") {

			// Assest 
			// file
			add(DBLiteSchema(DBLiteSchema::Integer, DB_SEQUENCEID));	// *
			add(DBLiteSchema(DBLiteSchema::Text, DB_FILENAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_ORGINALNAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FILEPATH));
			add(DBLiteSchema(DBLiteSchema::Text, DB_UUID));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MEDIATYPE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MD5));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_CRC));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_FILESIZE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATECREATE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEMODIFIED));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEADDED));
			// Description
			add(DBLiteSchema(DBLiteSchema::Text, DB_DESCRIPTION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LABEL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_TITLE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SUBJECT));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_RATING));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_RATINGPERCENT));
			add(DBLiteSchema(DBLiteSchema::Text, DB_KEYWORDS));// template
			add(DBLiteSchema(DBLiteSchema::Integer, DB_VERSION));// template
			// Origin
			add(DBLiteSchema(DBLiteSchema::Text, DB_COMMENT));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_AUTHOR));
			add(DBLiteSchema(DBLiteSchema::Date, DB_CAPTUREDATE));


			// Cammara
			// Image
			add(DBLiteSchema(DBLiteSchema::Date, DB_SUBSETIMEORIGINAL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COPYRIGHT));

			add(DBLiteSchema(DBLiteSchema::Integer, DB_WIDTH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_HEIGHT));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_VIEWROTATION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_BITSPERSAMPLE));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_XRESOLUTION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_YRESOLUTION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_RESOLUTIONUNIT));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_COMPRESSION));
			// Camera
			add(DBLiteSchema(DBLiteSchema::Text, DB_MAKER));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MODEL));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXPOSURETIME));
			add(DBLiteSchema(DBLiteSchema::Float, DB_APERTURE));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_ISOSPEEDRATING));
			add(DBLiteSchema(DBLiteSchema::Float, DB_EXPOSUREBIAS));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXPOSUREPROGRAM));
			add(DBLiteSchema(DBLiteSchema::Float, DB_SUBJECTDISTANCE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_FOCALLENGTH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_FOCALLENGTHIN35MM));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FLASH));
			add(DBLiteSchema(DBLiteSchema::Text, DB_METERINGMODE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_DIGITALZOOM));
			// Advanced photo
			add(DBLiteSchema(DBLiteSchema::Text, DB_LIGHTSOURCE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_COLORSPACE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_WHITEBALANCE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SCENECAPTURETYPE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_CONTRAST));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SATURATION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SHARPNESS));
			add(DBLiteSchema(DBLiteSchema::Float, DB_BRIGHTNESSVALUE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SUBJECTDISTANCERANGE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_MAXAPERTURE));
			add(DBLiteSchema(DBLiteSchema::Float, DB_COMPRESSEDBITSPERPIXEL));
			add(DBLiteSchema(DBLiteSchema::Float, DB_FLASHENERGY));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EXIFVERSION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SOFTWARE));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_PHOTOMETRICINTERPRETATION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_GPSTIMESTAMP));


			//	add(DBLiteSchema(DBLiteSchema::Text, DB_SENSINGMETHOD));

			// Location
			add(DBLiteSchema(DBLiteSchema::Text, DB_LATITUDE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LONGITUDE));
			add(DBLiteSchema(DBLiteSchema::Text, DB_LOCATION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_SCENE));// template

			// IPTC
			add(DBLiteSchema(DBLiteSchema::Text, DB_SOURCEURL));			// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_USAGERIGHTS));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_COPYRIGHTURL));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_HEADLINE));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_CATEGORY));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_SOURCE));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_INSTRUCTIONS));// template

			add(DBLiteSchema(DBLiteSchema::Text, DB_CREATOR));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_JOBTITLE));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_ADDRESS));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_CITY));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_STATE));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_POSTALCODE));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_COUNTRY));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_PHONE));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_EMAIL));// template
			add(DBLiteSchema(DBLiteSchema::Text, DB_WEBSITE));// template
			// new 

		}
	};

	class VersionMetadataSchema : public DBLiteTableSchema {
	public:
		VersionMetadataSchema() : DBLiteTableSchema("VersionMetadata") {
			add(DBLiteSchema(DBLiteSchema::Integer, DB_SEQUENCEID));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_VERSION));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_DATABASEID));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FILENAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_ORGINALNAME));
			add(DBLiteSchema(DBLiteSchema::Text, DB_FILEPATH));
			add(DBLiteSchema(DBLiteSchema::Integer, DB_CRC));
			add(DBLiteSchema(DBLiteSchema::Text, DB_MD5));
			add(DBLiteSchema(DBLiteSchema::Text, DB_UUID));


			add(DBLiteSchema(DBLiteSchema::Integer, DB_FILESIZE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEMODIFIED));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATECREATE));
			add(DBLiteSchema(DBLiteSchema::Date, DB_DATEADDED));

			add(DBLiteSchema(DBLiteSchema::Text, DB_COMMENT));
			add(DBLiteSchema(DBLiteSchema::Text, DB_AUTHOR));
			add(DBLiteSchema(DBLiteSchema::Text, DB_DESCRIPTION));
			add(DBLiteSchema(DBLiteSchema::Text, DB_EDITOR));
		}
	};
} // namespace SIADBLite