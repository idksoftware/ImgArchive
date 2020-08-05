#include "ParseProperties.h"

#include "DBDefines.h"

enum class Type {
	Text,
	Integer,
	Float,
	Date,
	Unknown
};

struct {
	const char* n_name;
	Type m_type;
} PropertiesList[] = {
	{ DB_DESCRIPTION, Type::Text },
	{ DB_LABEL, Type::Text },
	{ DB_TITLE, Type::Text },
	{ DB_SUBJECT, Type::Text },
	{ DB_RATING, Type::Integer },
	{ DB_RATINGPERCENT, Type::Integer },
	{ DB_TAGS, Type::Text },
	{ DB_KEYWORDS, Type::Text },
	{ DB_VERSION, Type::Integer },
	{ DB_COMMENT, Type::Text },
	{ DB_AUTHOR, Type::Text },
	{ DB_SUBSETIMEORIGINAL, Type::Date },
	{ DB_COPYRIGHT, Type::Text },
	{ DB_WIDTH, Type::Integer },
	{ DB_HEIGHT, Type::Integer },
	{ DB_VIEWROTATION, Type::Integer },
	{ DB_BITSPERSAMPLE, Type::Integer },
	{ DB_XRESOLUTION, Type::Integer },
	{ DB_YRESOLUTION, Type::Integer },
	{ DB_RESOLUTIONUNIT, Type::Integer },
	{ DB_COMPRESSION, Type::Integer },
	{ DB_MAKER, Type::Text },
	{ DB_MODEL, Type::Text },
	{ DB_EXPOSURETIME, Type::Text },
	{ DB_APERTURE, Type::Float },
	{ DB_ISOSPEEDRATING, Type::Integer },
	{ DB_EXPOSUREBIAS, Type::Float },
	{ DB_EXPOSUREPROGRAM, Type::Text },
	{ DB_SUBJECTDISTANCE, Type::Float },
	{ DB_FOCALLENGTH, Type::Float },
	{ DB_FOCALLENGTHIN35MM, Type::Integer },
	{ DB_FLASH, Type::Text },
	{ DB_METERINGMODE, Type::Text },
	{ DB_DIGITALZOOM, Type::Float },
	{ DB_LIGHTSOURCE, Type::Text },
	{ DB_COLORSPACE, Type::Text },
	{ DB_WHITEBALANCE, Type::Text },
	{ DB_SCENECAPTURETYPE, Type::Text },
	{ DB_CONTRAST, Type::Text },
	{ DB_SATURATION, Type::Text },
	{ DB_SHARPNESS, Type::Text },
	{ DB_BRIGHTNESSVALUE, Type::Text },
	{ DB_SUBJECTDISTANCERANGE, Type::Text },
	{ DB_MAXAPERTURE, Type::Float },
	{ DB_COMPRESSEDBITSPERPIXEL, Type::Float },
	{ DB_FLASHENERGY, Type::Float },
	{ DB_EXIFVERSION, Type::Text },
	{ DB_SOFTWARE, Type::Text },
	{ DB_PHOTOMETRICINTERPRETATION, Type::Integer },
	{ DB_GPSTIMESTAMP, Type::Text },
	{ DB_SENSINGMETHOD, Type::Text },
	{ DB_LATITUDE, Type::Text },
	{ DB_LONGITUDE, Type::Text },
	{ DB_LOCATION, Type::Text },
	{ DB_SCENE, Type::Text },
	{ DB_SOURCEURL, Type::Text },
	{ DB_USAGERIGHTS, Type::Text },
	{ DB_COPYRIGHTURL, Type::Text },
	{ DB_HEADLINE, Type::Text },
	{ DB_CATEGORY, Type::Text },
	{ DB_SOURCE, Type::Text },
	{ DB_INSTRUCTIONS, Type::Text },
	{ DB_CREATOR, Type::Text },
	{ DB_JOBTITLE, Type::Text },
	{ DB_ADDRESS, Type::Text },
	{ DB_CITY, Type::Text },
	{ DB_STATE, Type::Text },
	{ DB_POSTALCODE, Type::Text },
	{ DB_COUNTRY, Type::Text },
	{ DB_PHONE, Type::Text },
	{ DB_EMAIL, Type::Text },
	{ DB_WEBSITE, Type::Text }
};

Option ParseProperties::process(std::string& optionString)
{
	return Option();
}

bool ParseProperties::parse(const char* optionString)
{
	bool ret = processArgs(optionString);
	if (!ret) {
		return false;
	}

	int n = sizeof(PropertiesList) / sizeof(PropertiesList[0]);

	for (int i = 0; i < n; i++) {
		if (iequals(m_option, PropertiesList[i].n_name)) {
			switch (PropertiesList[i].m_type) {
			case Type::Text:
				return true;
			case Type::Integer:
				return isNumber(m_value);
			case Type::Float:
				return isFloatNumber(m_value);
			case Type::Date:
				return true;
			case Type::Unknown:
				break;
			default:
				break;
			}
		}
	}
	return false;
}
