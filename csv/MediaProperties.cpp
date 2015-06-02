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

#include "MetadataObject.h"
#include "MediaProperties.h"

namespace simplearchive {

class MediaPropertiesString : public CSVString {
public:
	MediaPropertiesString() {};
	virtual ~MediaPropertiesString() {};
	virtual const char *toString(CSVRowItem &item);

};

const char *MediaPropertiesString::toString(CSVRowItem &item) {
	MediaPropertiesItem &tmp = (MediaPropertiesItem &)item;
	return tmp.toString();
}


MediaPropertiesItem::MediaPropertiesItem(int id) : CSVRowItem(id) {
}

MediaPropertiesItem::~MediaPropertiesItem() {
}

const char *MediaPropertiesItem::toString() {
	start();
	add(m_width);
	add(m_height);
	add(m_resolution);
	add(m_depth);
	add(m_viewRotation);
	add(m_sampleColor);
	add(m_page);
	add(m_colorSpace);
	add(m_compression);
	add(m_prinaryEncoding);
	end();
	return row().c_str();
}




bool MediaPropertiesItem::fromString(int pos, std::string &str) {
	m_width = token(&pos, str);
	pos++;
	m_height = token(&pos, str);
	pos++;
	m_resolution = token(&pos, str);
	pos++;
	m_depth = token(&pos, str);
	pos++;
	m_viewRotation = token(&pos, str);
	pos++;
	m_sampleColor = token(&pos, str);
	pos++;
	m_page = token(&pos, str);
	pos++;
	m_colorSpace = token(&pos, str);
	pos++;
	m_compression = token(&pos, str);
	pos++;
	m_prinaryEncoding = token(&pos, str);
	return true;
}

MediaProperties::MediaProperties() {

}

MediaProperties::~MediaProperties() {

}
// Adds the information of the side car file from a MetadataObject.
void MediaProperties::add(MetadataObject &metadataObject) {
	const char *idStr = metadataObject.getSequenceId().c_str();
	int seqId = atol(idStr);
	MediaPropertiesItem *item = new MediaPropertiesItem(seqId);

	item->setWidth(metadataObject.getWidth());
	item->setHeight(metadataObject.getHeight());
	item->setResolution(metadataObject.getResolution());
	item->setDepth(metadataObject.getDepth());
	item->setViewRotation(metadataObject.getViewRotation());
	item->setSampleColor(metadataObject.getSampleColor());
	item->setPage(metadataObject.getPage());
	item->setColorSpace(metadataObject.getColorSpace());
	item->setCompression(metadataObject.getCompression());
	item->setPrinaryEncoding(metadataObject.getPrinaryEncoding());
	push_back(item);
	return;
}

/// reads a csv side car file
bool MediaProperties::read(const char *datafile) {
	std::string path(datafile);
	path += "/MediaProperties.csv";
	if (CSVRow::read(path.c_str()) == false) {
		return false;
	}
	return true;
}

/// reads a csv side car file
bool MediaProperties::write(const char *path) {
	std::string filename("MediaProperties.csv");
	MediaPropertiesString cov;
	CSVRow::write(path, filename.c_str(), cov);

	return true;
}

bool MediaProperties::add(const char *row) {
	std::string tmp = row;
	int pos = 0;
	int seqId = readId(tmp, &pos);
	MediaPropertiesItem *item = new MediaPropertiesItem(seqId);
	item->fromString(pos+1, tmp);
	push_back(item);
	return true;
}

unsigned int MediaProperties::findImage(const char *text, int col) {
	unsigned int count = 0;
	for (std::vector<CSVRowItem *>::iterator i = this->begin(); i != this->end(); i++, count++) {
		MediaPropertiesItem *item = (MediaPropertiesItem *)*i;
		std::string field;
		switch (col) {
		case 0: field = item->getWidth(); break;
		case 1: field = item->getHeight(); break;
		case 2: field = item->getResolution(); break;
		case 3: field = item->getDepth(); break;
		case 4: field = item->getViewRotation(); break;
		case 5: field = item->getSampleColor(); break;
		case 6: field = item->getPage(); break;
		case 7: field = item->getColorSpace(); break;
		case 8: field = item->getCompression(); break;
		case 9: field = item->getPrinaryEncoding(); break;
        return std::string::npos;
		}
		if (field.compare(text) == 0) {
			return count;
		}
	}
	return std::string::npos;
}
bool MediaProperties::load(unsigned int row, MetadataObject &mo) {
	if (this->size() < row ) {
		return false;
	}
	MediaPropertiesItem *item = (MediaPropertiesItem *)at(row);
	mo.setWidth(item->getWidth());
    mo.setHeight(item->getHeight());
    mo.setResolution(item->getResolution());
    mo.setDepth(item->getDepth());
    mo.setViewRotation(item->getViewRotation());
    mo.setSampleColor(item->getSampleColor());
    mo.setPage(item->getPage());
    mo.setColorSpace(item->getColorSpace());
    mo.setCompression(item->getCompression());
    mo.setPrimaryEncoding(item->getPrinaryEncoding());

    return true;
}
bool MediaProperties::save(unsigned int row, MetadataObject &mo) {
	if (this->size() < row ) {
		return false;
	}
	MediaPropertiesItem *item = (MediaPropertiesItem *)at(row);
	item->setWidth(mo.getWidth());
	item->setHeight(mo.getHeight());
	item->setResolution(mo.getResolution());
	item->setDepth(mo.getDepth());
	item->setViewRotation(mo.getViewRotation());
	item->setSampleColor(mo.getSampleColor());
	item->setPage(mo.getPage());
	item->setColorSpace(mo.getColorSpace());
	item->setCompression(mo.getCompression());
	item->setPrinaryEncoding(mo.getPrinaryEncoding());
    return true;
}

const std::string& MediaPropertiesItem::getColorSpace() const {
	return m_colorSpace;
}

void MediaPropertiesItem::setColorSpace(const std::string& colorSpace) {
	m_colorSpace = colorSpace;
}

const std::string& MediaPropertiesItem::getCompression() const {
	return m_compression;
}

void MediaPropertiesItem::setCompression(const std::string& compression) {
	m_compression = compression;
}

const std::string& MediaPropertiesItem::getDepth() const {
	return m_depth;
}

void MediaPropertiesItem::setDepth(const std::string& depth) {
	m_depth = depth;
}

const std::string& MediaPropertiesItem::getHeight() const {
	return m_height;
}

void MediaPropertiesItem::setHeight(const std::string& height) {
	m_height = height;
}

const std::string& MediaPropertiesItem::getPage() const {
	return m_page;
}

void MediaPropertiesItem::setPage(const std::string& page) {
	m_page = page;
}

const std::string& MediaPropertiesItem::getPrinaryEncoding() const {
	return m_prinaryEncoding;
}

void MediaPropertiesItem::setPrinaryEncoding(
		const std::string& prinaryEncoding) {
	m_prinaryEncoding = prinaryEncoding;
}

const std::string& MediaPropertiesItem::getResolution() const {
	return m_resolution;
}

void MediaPropertiesItem::setResolution(const std::string& resolution) {
	m_resolution = resolution;
}

const std::string& MediaPropertiesItem::getSampleColor() const {
	return m_sampleColor;
}

void MediaPropertiesItem::setSampleColor(const std::string& sampleColor) {
	m_sampleColor = sampleColor;
}

const std::string& MediaPropertiesItem::getSequenceId() const {
	return m_sequenceId;
}

void MediaPropertiesItem::setSequenceId(const std::string& sequenceId) {
	m_sequenceId = sequenceId;
}

const std::string& MediaPropertiesItem::getViewRotation() const {
	return m_viewRotation;
}

void MediaPropertiesItem::setViewRotation(const std::string& viewRotation) {
	m_viewRotation = viewRotation;
}

const std::string& MediaPropertiesItem::getWidth() const {
	return m_width;
}

void MediaPropertiesItem::setWidth(const std::string& width) {
	m_width = width;
}

} /* namespace simplearchive */
