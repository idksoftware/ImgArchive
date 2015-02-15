/*
 * ViewManager.h
 *
 *  Created on: Jan 23, 2015
 *      Author: wzw7yn
 */

#ifndef VIEWMANAGER_H_
#define VIEWMANAGER_H_
#include <string>

#define VIEW_CONFIG_FILE	"/view.conf"

namespace simplearchive {
class ViewItemContainer;
class ViewManager {
	static ViewManager *m_this;
	static ViewItemContainer *m_pContainer;
	static std::string m_confpath;
	static std::string m_archiveRoot;
	static std::string m_masterViewPath;
	bool readConf();
public:
	ViewManager();
	virtual ~ViewManager();

	static bool initalise(const char *archiveRoot, const char *confpath);
	bool initaliseMaster(const char *archiveRoot, const char *masterViewPath) {
		m_archiveRoot = archiveRoot;
		m_masterViewPath = masterViewPath;
		return true;
	}
	//bool processMaster();
	bool process();
	bool add(int id);
	//bool add2Master(const char *relPath, const char *name);
};

} /* namespace simplearchive */
#endif /* VIEWMANAGER_H_ */
