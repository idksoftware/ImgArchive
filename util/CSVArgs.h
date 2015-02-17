/*
 * CSVArgs.h
 *
 *  Created on: Feb 17, 2015
 *      Author: wzw7yn
 */

#ifndef CSVARGS_H_
#define CSVARGS_H_
#include <string>
#include <vector>

namespace simplearchive {

class CSVArgs : public std::vector<std::string> {

public:
	CSVArgs();
	virtual ~CSVArgs();
	bool process(const char *dataString);
};

} /* namespace simplearchive */
#endif /* CSVARGS_H_ */
