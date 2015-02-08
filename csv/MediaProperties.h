/*
 * MediaProperties.h
 *
 *  Created on: Jul 9, 2014
 *      Author: wzw7yn
 */

#ifndef MEDIAPROPERTIES_H_
#define MEDIAPROPERTIES_H_
#include <string>
#include <vector>
#include <cstdlib>
#include "CSVRow.h"

namespace simplearchive {

class MediaPropertiesItem : public CSVRowItem {
	std::string m_sequenceId;
	std::string m_width;
	std::string m_height;
	std::string m_resolution;
	std::string m_depth;
	std::string m_viewRotation;
	std::string m_sampleColor;
	std::string m_page;
	std::string m_colorSpace;
	std::string m_compression;
	std::string m_prinaryEncoding;
public:
	MediaPropertiesItem(int id);
	virtual ~MediaPropertiesItem();
	const std::string& getColorSpace() const;
	void setColorSpace(const std::string& colorSpace);
	const std::string& getCompression() const;
	void setCompression(const std::string& compression);
	const std::string& getDepth() const;
	void setDepth(const std::string& depth);
	const std::string& getHeight() const;
	void setHeight(const std::string& height);
	const std::string& getPage() const;
	void setPage(const std::string& page);
	const std::string& getPrinaryEncoding() const;
	void setPrinaryEncoding(const std::string& prinaryEncoding);
	const std::string& getResolution() const;
	void setResolution(const std::string& resolution);
	const std::string& getSampleColor() const;
	void setSampleColor(const std::string& sampleColor);
	const std::string& getSequenceId() const;
	void setSequenceId(const std::string& sequenceId);
	const std::string& getViewRotation() const;
	void setViewRotation(const std::string& viewRotation);
	const std::string& getWidth() const;
	void setWidth(const std::string& width);

	virtual const char *toString();
	virtual bool fromString(int pos, std::string &str);
};

class MetadataObject;
class MediaProperties : public CSVRow {
public:
	MediaProperties();
	virtual ~MediaProperties();

	/// Adds the information of the GPS properties from a MetadataObject.
	void add(MetadataObject &metadataObject);

	/// Adds a GPSPropertiesItem object.
	void add(MediaPropertiesItem &item);

	/// reads a csv GPSProperties file
	bool read(const char *datafile);

	/// writes a csv GPSProperties file
	bool write(const char *path);

	virtual bool add(const char *row);
};

} /* namespace simplearchive */
#endif /* MEDIAPROPERTIES_H_ */
