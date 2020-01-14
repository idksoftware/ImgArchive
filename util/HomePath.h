#pragma once

#include <string>

enum class HPSource {

};

enum class HomePathType {
	LocalEnv,	// Local Environment set
	SystemEnv,	// System Environment set
	UserOnly,	// user only archive
	AllUsers,	// all users archive
	Unknown
};

enum class HPError {
	Ok,					// OK
	CannotLocatePath,	// Cannot locate path at default or HOME if set
	NotFound,			// Path not at default and HOME not set 
	Unknown				// Unknown state
};

class HomePath
{
	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;
	
	static HomePathType m_type;
	static HPError m_error;
public:
	HomePath() = default;
	~HomePath() = default;
	static bool init();
	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
};





class WorkspacePath
{
	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;

	static HomePathType m_type;
	static HPError m_error;
public:
	WorkspacePath() = default;
	~WorkspacePath() = default;
	static bool init();
	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
};

class PicturePath
{
	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;

	static HomePathType m_type;
	static HPError m_error;
public:
	PicturePath() = default;
	~PicturePath() = default;
	static bool init();
	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
};

class WWWImagePath
{
	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;

	static HomePathType m_type;
	static HPError m_error;
public:
	WWWImagePath() = default;
	~WWWImagePath() = default;
	static bool init();
	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
};