/*
 * ArchiveRepository.h
 *
 *  Created on: May 9, 2014
 *      Author: wzw7yn
 */

#ifndef ARCHIVEREPOSITORY_H_
#define ARCHIVEREPOSITORY_H_
#include "ImageGroup.h"
#include "HistoryEvent.h"

namespace simplearchive {

class HistoryEvent;
/// @brief The Main Archive class object.
class ArchiveRepository {
	static ArchiveRepository *m_This;
	ArchiveRepository();
	bool Import(ImageContainer *imageContainer);
	std::string m_pathToArchive;
	std::string m_pathToSourceRoot;
	std::string m_pathToActiveRoot;
	bool processHistory(const char *filepath, const char *comment, const HistoryEvent &evt, int ver);
public:

	virtual ~ArchiveRepository();
	static ArchiveRepository &get();

	bool Import(ImageGroup &imageGroup);
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
#endif /* ARCHIVEREPOSITORY_H_ */
