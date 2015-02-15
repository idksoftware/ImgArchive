/*
 * CSVJournalFile.h
 *
 *  Created on: Jan 13, 2015
 *      Author: wzw7yn
 */

#ifndef CSVJOURNALFILE_H_
#define CSVJOURNALFILE_H_

#include <memory>
#include <vector>
#include "ExifDateTime.h"
namespace simplearchive {


class JournalData;
class CSVJournalFile : private std::vector<JournalData> {


public:
	CSVJournalFile();
	virtual ~CSVJournalFile();
	bool writeCSV(const char *path);
	bool writeXML(const char *path);
	bool add(int id, const char *name, unsigned int size, unsigned long crc,
							const char *md5, ExifDateTime &created, ExifDateTime &modified);
	void clearUp();
};

} /* namespace simplearchive */
#endif /* CSVJOURNALFILE_H_ */
