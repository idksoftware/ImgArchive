/*
 * AppOptions.h
 *
 *  Created on: Oct 27, 2014
 *      Author: wzw7yn
 */

#ifndef APPOPTIONS_H_
#define APPOPTIONS_H_
#include <string>

namespace simplearchive {

class AppOptions {
public:
	typedef enum {
		CM_Import,
		CM_Export,
		CM_Checkout,
		CM_Checkin,
		CM_Version,
		CM_View,
		CM_Mirror,
		CM_Uncheckin,
		CM_Archive,
		CM_Unknown
	} CommandMode;
private:
	static AppOptions *m_this;
	static CommandMode m_commandMode;
	static std::string m_name;
	bool m_error;
	void setCommandMode(const AppOptions::CommandMode mode);
	bool setCommandMode(const char *modeString);
	void setName(const char *name);
	AppOptions();
public:
	
	static AppOptions &get();

	virtual ~AppOptions();

	bool initalise(int argc, char **argv);
	AppOptions::CommandMode getCommandMode();
	const char *getName();
	
};

} /* namespace simplearchive */
#endif /* APPOPTIONS_H_ */
