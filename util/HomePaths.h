#pragma once

#include <string>
#include <vector>
#include <memory>

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

enum class HomePathID {
	HomePath,
	MasterPath,
	DerivativePath,
	WorkspacePath,
	PicturePath,
	WWWImagePath,
	InvalidPath
};

class HomePathsBase {
	static std::vector<HomePathsBase*> m_list;
protected:
	
public:
	HomePathsBase() = default;
	virtual ~HomePathsBase() = default;

	virtual HomePathID getID() const = 0;
};


class HomePath : public HomePathsBase {

	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	
	static HomePathType m_type;
	static HPError m_error;

public:
	HomePath() = default;
	~HomePath() = default;
	bool init();

	virtual HomePathID getID() const { return HomePathID::HomePath; };
	
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
	static bool setPath(const char* p);
	static bool setAllUserDefaultHome();
	static bool setLocalUserDefaultHome();

	static std::string get();
	
	static HomePathsBase* getObject() {
		static HomePath homePath;
		return &homePath;
	}
};

class MasterPath : public HomePathsBase {

	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;
	
	static HomePathType m_type;
	static HPError m_error;
protected:
	
public:
	MasterPath() = default;
	~MasterPath() = default;


	static bool init();
	virtual HomePathID getID() const { return HomePathID::MasterPath; };

	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
	static bool setPath(const char* p);
	static bool setAllUserDefaultHome();
	static bool setLocalUserDefaultHome();

	static HomePathsBase* getThis() {
		static MasterPath masterPath;
		return &masterPath;
	}
};


class DerivativePath : public HomePathsBase {

	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;

	static HomePathType m_type;
	static HPError m_error;
protected:
	
public:
	DerivativePath() = default;
	~DerivativePath() = default;
	
	bool init();
	virtual HomePathID getID() const { return HomePathID::DerivativePath; };

	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
	static bool setPath(const char* p);
	static bool setAllUserDefaultHome();
	static bool setLocalUserDefaultHome();

	static HomePathsBase* getThis() {
		static DerivativePath derivativePath;
		return &derivativePath;
	}
};

class WorkspacePath : public HomePathsBase {

	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;

	static HomePathType m_type;
	static HPError m_error;
protected:
	
public:
	WorkspacePath() = default;
	~WorkspacePath() = default;

	bool init();
	virtual HomePathID getID() const { return HomePathID::WorkspacePath; };

	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
	static bool setPath(const char* p);
	static bool setAllUserDefaultHome();
	static bool setLocalUserDefaultHome();

	static HomePathsBase* getThis() {
		static DerivativePath derivativePath;
		return &derivativePath;
	}
};

class PicturePath : public HomePathsBase {

	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	//static bool m_configured;

	static HomePathType m_type;
	static HPError m_error;
public:
	PicturePath() = default;
	~PicturePath() = default;
	
	bool init();
	virtual HomePathID getID() const { return HomePathID::PicturePath; };

	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
	static bool setPath(const char* p);
	static bool setAllUserDefaultHome();
	static bool setLocalUserDefaultHome();
};

class WWWImagePath : public HomePathsBase {

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
	virtual HomePathID getID() const { return HomePathID::WWWImagePath; };

	static std::string get();
	static HomePathType type();
	static bool isFound() noexcept {	// string found
		return m_found;
	}
	static bool isValid() noexcept {	// in file system
		return m_valid;
	}
	static bool setPath(const char* p);
	static bool setAllUserDefaultHome();
	static bool setLocalUserDefaultHome();
};