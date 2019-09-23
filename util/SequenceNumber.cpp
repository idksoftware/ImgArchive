#include "SequenceNumber.h"
#include <string>
#include <vector>
#include <iostream> 
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


static char base32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

static int LookupDigits[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,           //gap: spc,!"#$%'()*
	-1,                   // +
	-1, -1, -1,             // gap ,-.
	-1,                   // /
	-1, -1, 26, 27, 28, 29, 30, 31, -1, -1, // 0-9
	-1, -1, -1,             // gap: :;<
	-1,                   //  = (end padding)
	-1, -1, -1,             // gap: >?@
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,
	17,18,19,20,21,22,23,24,25, // A-Z
	-1, -1, -1, -1, -1, -1,    // gap: [\]^_`
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1, // a-z    
	-1, -1, -1, -1,          // gap: {|}~ (and the rest...)
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};



const char *Base32::decimal2Base32(long dec, int padding) {
	std::vector<char> list;
	m_str.clear();
	int rem = 0;
	if (dec > 0) {
		while (dec > 0) {
			rem = dec % 32;
			list.push_back(base32[rem]);
			dec = dec / 32;
		}
	}
	else {
		list.push_back(base32[0]);
	}
	std::reverse(list.begin(), list.end());
	for (auto i = list.begin(); i != list.end(); i++) {
		const char c = *i;
		m_str += c;
	}
	if (padding != 0) {
		padToBase32(m_str, padding);
	}
	return m_str.c_str();
}

int Base32::Base32ToDecimal(std::string encoded)
{
	int ret = 0;
	unsigned char c = 0;
	unsigned int num = 0;
	const int len = encoded.length();
	for (unsigned int index = 0; index < encoded.length(); index++) {
		c = encoded[index];
		if (c >= sizeof(LookupDigits)) {
			return -1;
		}
		num = LookupDigits[c];
		if (num == -1) {
			return -1;
		}
		ret = (32 * ret) + num;
	}
	return ret;
}



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
		m_str = base32.decimal2Base32(dec, 3);

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
	int high_num = base32.Base32ToDecimal(high_string);
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
	int pos = filenane.find_last_of('.');
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
		throw  std::exception("Invalid year");
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


