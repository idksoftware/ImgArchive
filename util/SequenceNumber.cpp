#include "SequenceNumber.h"
#include <string>
#include <vector>
#include <iostream> 
#include <algorithm>
#include "SAUtils.h"
//#include "CIDKFileFind.h"

/*
using FileList = std::vector<std::string>;
using FileList_Ptr = std::unique_ptr<FileList>;


FileList_Ptr getFiles(const char *dirpath) {

	FileList_Ptr fileList = std::make_unique<FileList>();
	
	
	std::string dirpathstr(dirpath);
	dirpathstr = dirpathstr + "/*.*";
	CIDKFileFind fileFind(dirpathstr);
	fileFind.Open();
	if (fileFind.GotFile() == false) {
		return fileList;
	}
	do {
		std::string tmp(fileFind.GetFileName());
		fileList->push_back(tmp);
	} while (fileFind.GetNext());

	return fileList;

}
*/




const char * NumberSequence::toSequence()
{
	std::vector<char> higher;
	std::string lower_string;
	int dec = m_int;
	
	m_str.clear();
	int rem;
	if (dec > 0) {
		
		rem = dec % 100000;
		lower_string = std::to_string(rem);
		dec = dec / 100000;
		padDec(lower_string, 3);

	
		Base32 base32;
		m_str = base32.toBase32(dec, 3);

	}
	
	m_str = m_str + '-';
	padDec(lower_string, 5);
	m_str += lower_string;
	return m_str.c_str();
}

int NumberSequence::toDecimal()
{
	
	std::string high_string = m_str.substr(0, 3);
	if (m_str[3] != '-') {
		return -1;
	}
	Base32 base32;
	int high_num = base32.toDecimal(high_string);
	std::string lower_string = m_str.substr(4, 9);
	if (isNumber(lower_string) == false) {
		return -1;
	}


	return 0;
}

DateSequence::DateSequence(int y, int m, int d, long dec, const char *path)
{
	m_year = y;
	m_month = m;
	m_day = d;
	m_int = dec;
	m_path = path;
}

DateSequence::DateSequence(std::string& encoded)
{
}

std::string DateSequence::toString() {
	std::stringstream s;
	s << std::setw(2) << std::setfill('0') << m_year;
	s << std::setw(2) << std::setfill('0') << m_month;
	s << std::setw(2) << std::setfill('0') << m_day;
	return std::string(s.str());

}


int DateSequence::getNumber(const std::string& filenane) {
	size_t pos = filenane.find_last_of('.');
	std::string numStr = filenane.substr(pos - 5, 5);
	if (isNumber(numStr) == false) {
		return -1;
	}
	int num = atoi(numStr.c_str());
	return num;
}
const char * DateSequence::toSequence()
{
	std::vector<int> seqList;
	if (m_year > 2999 || m_year < 1900) {
		throw  std::exception();
	}
	if (m_year >= 2000) {
		m_year = m_year - 2000;
	}
	else {
		m_year = m_year - 1900;
	}
	std::string lower_string;
	int seqTotal = -1;
	if (m_path.empty() == false) {
		
		FileList_Ptr imagelist = SAUtils::getFiles_(m_path.c_str());
		for (auto iii = imagelist->begin(); iii != imagelist->end(); iii++) {
			std::string imageFile = *iii;
			char c = (imageFile)[0];
			if (c == '.') {
				continue;
			}
			printf("\tImage %s \n", imageFile.c_str());
			int seq = DateSequence::getNumber(imageFile);
			if (seqTotal < seq) {
				seqTotal = seq;
			}
		}
		seqTotal++;
		lower_string = std::to_string(seqTotal);
		padDec(lower_string, 5);
	}
	else {

		int dec = m_int;
		m_str.clear();
		int rem;
		if (dec > 0) {

			rem = dec % 100000;
			lower_string = std::to_string(rem);
			dec = dec / 100000;
			padDec(lower_string, 5);
		}

	}
	std::string str = toString();
	str += '-';
	str += lower_string;
	m_str = str;
	return m_str.c_str();
}

int DateSequence::toDecimal()
{
	return 0;
}

int DateSequence::toDate()
{
	return 0;
}

const char *NoRenameSequence::toSequence() {

	std::string lower_string;
	bool found = false;
	bool complete = false;
	int seqTotal = 1;
	std::string targetFilename = m_name;
	if (m_path.empty() == false) {

		FileList_Ptr imagelist = SAUtils::getFiles_(m_path.c_str());
		while (!complete) {
			for (auto iii = imagelist->begin(); iii != imagelist->end(); iii++) {
				std::string imageFile = *iii;
				char c = (imageFile)[0];
				if (c == '.') {
					continue;
				}
				std::string name = SAUtils::getFilenameNoExt(imageFile);
				if (name.compare(targetFilename) == 0) {
					found = true;
					break;
				}
			}
			if (found) {
				found = false;
				targetFilename = m_name;
				std::string seqext = "_";
				seqext += std::to_string(seqTotal++);
				targetFilename += seqext;
			}
			else {
				complete = true;
			}
		}
		
	}
	m_name = targetFilename;
	return m_name.c_str();
}


