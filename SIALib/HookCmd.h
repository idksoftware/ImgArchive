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

#ifndef HOOKCMD_H_
#define HOOKCMD_H_

#include "ExternalProcess.h"
#include "ImagePath.h"

namespace simplearchive {
//class ImagePath;

/**
 * @brief Defines the base of all hook commands
 *
 */ 
class HookCmd {
public:
	typedef enum {
		HC_OnFile,
		HC_OnFileCopy,
		HC_OnFolder,
		HC_OnContainer,
		HC_PostArchive,
		HC_PreArchive,
		HC_PreProcess,
		HC_Unknown
	} HookType;

private:
	HookType m_HookType;

	bool readScriptsNames();
	std::string getScriptNames();
	static std::string m_hookPath;
	std::string m_name;

protected:
	void init(ImagePath &imagePath);
public:
	HookCmd(HookType type);
	virtual ~HookCmd();
	bool process();
	bool process(const char *file,const char *ext);
	static void setHookPath(const char *path);
};

/** @beief This is thw post archive hook carried out after
 *         the images are placed in the archive. 
 */
class PostArchiveCmd : public HookCmd {
public:
	PostArchiveCmd();
	PostArchiveCmd(ImagePath &imagePath);
};

/** @beief This is thw pre-archive hook carried out before
 *         the images are placed in the archive. 
 */
class PreArchiveCmd : public HookCmd {
public:
	PreArchiveCmd();
	PreArchiveCmd(ImagePath &imagePath);
};

/** @beief This is thw pre-archive hook carried out before
 *         the images are processed for inputing into the
 *         archive in the archive. 
 */
class PreProcessCmd : public HookCmd {
public:
	PreProcessCmd();
	PreProcessCmd(ImagePath &imagePath);
};

class OnFileCmd : public HookCmd {
	std::string m_path;
	std::string m_file;
	std::string m_ext;
public:
	OnFileCmd(const char *file);
	bool process();

};

class OnFileCopyCmd : public HookCmd {
	std::string m_path;
	std::string m_image;
public:
	OnFileCopyCmd(const char *path, const char *image);
	bool process();

};

class OnFolderCmd : public HookCmd {
	std::string m_folder;
public:
	OnFolderCmd(const char *folder);

};
/*
class OnContainerCmd : public HookCmd {
public:
	OnContainerCmd();
	OnContainerCmd(ImagePath &imagePath);
};
*/
} /* namespace simplearchive */
#endif /* HOOKCMD_H_ */
