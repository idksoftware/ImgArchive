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

#ifndef VIEWMANAGER_H_
#define VIEWMANAGER_H_
#include <string>
#include <mutex>
#include <memory>

#define VIEW_CONFIG_FILE	"/view.dat"



namespace simplearchive {
class ViewItemContainer;
/**
 * This class manages the view functionality.
 *
 * This view manager uses image hold in the Master archive only.
 * The View manager need to be one time initialised.
 */
class ViewManager {
public:
	typedef enum {
		VS_Unknown,
		VS_IncludeSet,
		VS_ExcludeSet,
		VS_InclusiveSearch,
		VS_ExclusiveSearch,
		VS_ALL
	} EViewSet;
private:
	
	
	
	ViewManager();
	


	static std::shared_ptr<ViewItemContainer> m_pContainer;
	static std::string m_confpath;
	static std::string m_archiveRoot;
	static std::string m_masterViewPath;
	bool readConf();
	static std::string makeOptionString(const char *locationPath, bool mode, bool type, bool access, EViewSet viewSet, bool metadata, const char *setpath);
public:
	ViewManager(const ViewManager& src) = delete;
	//ViewManager(ViewManager const&) = delete;
	void operator=(ViewManager const&) = delete;
	ViewManager(ViewManager&&) = delete;                  // Move construct
	ViewManager& operator=(ViewManager &&) = delete;      // Move assign

	virtual ~ViewManager();
	static ViewManager& GetInstance();
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
	static bool create(const char *name, const char *locationPath, bool mode, bool type, bool access, EViewSet viewSet, bool metadata, const char *path);
	static bool remove(const char *name);
	static bool edit(const char *name, const char *locationPath, bool mode, bool type, bool access, EViewSet viewSet, bool metadata, const char *path);
};

} /* namespace simplearchive */
#endif /* VIEWMANAGER_H_ */
