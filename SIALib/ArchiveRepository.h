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

#pragma once

#include "ImageContainer.h"


namespace simplearchive {


/// @brief The Main Archive class object.
class ArchiveRepository {
	
	ArchiveRepository() noexcept ;
	bool Import(ImageContainer *imageContainer);
	std::string m_pathToArchive;
	std::string m_pathToSourceRoot;
	std::string m_pathToActiveRoot;
	
public:
	ArchiveRepository(ArchiveRepository const&) = delete;
	void operator=(ArchiveRepository const&) = delete;
	ArchiveRepository(ArchiveRepository&&) = delete;                  // Move construct
	ArchiveRepository& operator=(ArchiveRepository &&) = delete;      // Move assign


	virtual ~ArchiveRepository();
	static ArchiveRepository &get();

	//bool Import(ImageGroup &imageGroup);
	bool checkout(const char *filepath, const char *comment);
	bool checkin(const char *filepath, const char *comment);
	bool uncheckout(const char *filepath, const char *comment);

	const std::string& getPathToArchive() const {
		return m_pathToArchive;
	}

	void setPathToArchive(std::string& pathToArchive) {
		m_pathToArchive = pathToArchive;
	}

	const std::string& getPathToSourceRoot() const {
		return m_pathToSourceRoot;
	}

	void setPathToSourceRoot(std::string& pathToSourceRoot) {
		m_pathToSourceRoot = pathToSourceRoot;
	}

	void setPathToActiveRoot(const std::string& pathToActiveRoot) {
		m_pathToActiveRoot = pathToActiveRoot;
	}
};

} /* namespace simplearchive */

