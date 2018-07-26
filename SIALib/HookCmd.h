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
		MC_MVPreview1,
		MC_MVPreview2,
		MC_MVPreview3,
		MC_MVThumb,
		MC_MVRAW,
		MC_MVImageSize,
		HC_Unknown
	} HookType;

private:
	HookType m_HookType;
	std::string m_output;
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
	bool process(const char *arg1);
	bool process(const char *arg1,const char *arg2);
	bool process(const char *arg1, const char *arg2, const char *arg3);
	static void setHookPath(const char *path);
	const char *getOutput() noexcept {
		return m_output.c_str();
	}
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

class OnIndentSizeCmd : public HookCmd {
	std::string m_imageName;
public:
	OnIndentSizeCmd(const char *m_imageName);
	bool process();
};

class OnViewThumbnailCmd : public HookCmd {
	std::string m_sourceFilePath;
	std::string m_distFilePath;
public:
	OnViewThumbnailCmd(const char *sourceFilePath, const char *distFilePath);
	bool process();
};
class OnViewPreview1Cmd : public HookCmd {
	std::string m_sourceFilePath;
	std::string m_distFilePath;
public:
	OnViewPreview1Cmd(const char *sourceFilePath, const char *distFilePath);
	bool process();
};
class OnViewPreview2Cmd : public HookCmd {
	std::string m_sourceFilePath;
	std::string m_distFilePath;
public:
	OnViewPreview2Cmd(const char *sourceFilePath, const char *distFilePath);
	bool process();
};
class OnViewPreview3Cmd : public HookCmd {
	std::string m_sourceFilePath;
	std::string m_distFilePath;
public:
	OnViewPreview3Cmd(const char *sourceFilePath, const char *distFilePath);
	bool process();
};
class OnViewRAWCmd : public HookCmd {
	std::string m_sourceFilePath;
	std::string m_distFilePath;
public:
	OnViewRAWCmd(const char *sourceFilePath, const char *distFilePath);
	bool process();
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
