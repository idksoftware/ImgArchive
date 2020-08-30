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

class InitailiseHomePaths;

class HomePathsBase {
	
	
protected:

	bool m_found{ false };	// string found
	bool m_valid{ false };	// in file system
	HomePathType m_type{ HomePathType::Unknown };
	HPError m_error{ HPError::Unknown };

	static bool m_first;
	static std::string m_allUsersHomeEnvironmentPath;
	static std::string m_myselfHomeEnvironmentPath;
	static std::string m_allusersHomeDefaultPath;
	static std::string m_myselfHomeDefaultPath;
	static std::string m_homeDrive;
	static std::string m_homePathEnv;

public:
	HomePathsBase() = default;
	virtual ~HomePathsBase() {};

	HPError error() noexcept {
		return m_error;
	}
	HomePathType type() noexcept {
		return m_type;
	}
	bool isFound() noexcept {	// string found
		return m_found;
	}
	bool isValid() noexcept {	// in file system
		return m_valid;
	}
	
	virtual bool init() = 0;
	virtual HomePathID getID() const = 0;
	virtual bool setPath(const char* p) = 0;
	virtual bool setAllUserDefaultHome() = 0;
	virtual bool setLocalUserDefaultHome() = 0;
	virtual bool setEnvironmentPath() = 0;
	static bool loadEnv();
};

class InitialiseHomePaths {
	static std::vector<HomePathsBase*> m_list;
	bool m_error{ false };

	void initHomePaths();
	bool setHomeType();
	bool setAllUserDefaultHome();
	bool setLocalUserDefaultHome();
public:
	InitialiseHomePaths() noexcept {
		HomePathsBase::loadEnv();
		initHomePaths();
	}
	~InitialiseHomePaths() = default;

	bool init();

	bool error() noexcept {
		return m_error;
	}

	static InitialiseHomePaths& getObject() {
		static InitialiseHomePaths initialiseHomePaths;
		return initialiseHomePaths;
	}


};

class HomePath : public HomePathsBase {

	static std::string m_path;
	
public:

	HomePath() = default;
	virtual ~HomePath() {};

	HomePathID getID() const override {
		return HomePathID::HomePath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	bool setAllUserDefaultHome() override;
	bool setLocalUserDefaultHome() override;
	bool setEnvironmentPath() override;

	static std::string get();

	static HomePath& getObject() {
		static HomePath homePath;
		return homePath;
	}
};

class MasterPath : public HomePathsBase {

	static std::string m_path;
protected:
	
public:
	MasterPath() = default;
	virtual ~MasterPath() {};

	virtual HomePathID getID() const {
		return HomePathID::MasterPath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	bool setAllUserDefaultHome() override;
	bool setLocalUserDefaultHome() override;
	bool setEnvironmentPath() override;

	static std::string& get();
	static MasterPath& getObject() {
		static MasterPath masterPath;
		return masterPath;
	}
};


class DerivativePath : public HomePathsBase {

	static std::string m_path;
protected:
	
public:
	DerivativePath() = default;
	virtual ~DerivativePath() {};
	
	
	HomePathID getID() const override {
		return HomePathID::DerivativePath;
	};

	
	bool init() override;
	bool setPath(const char* p) override;
	bool setAllUserDefaultHome() override;
	bool setLocalUserDefaultHome() override;
	bool setEnvironmentPath() override;

	static std::string& get();
	static DerivativePath& getObject() {
		static DerivativePath derivativePath;
		return derivativePath;
	}
};

class WorkspacePath : public HomePathsBase {

	static std::string m_path;
	
protected:
	
public:
	WorkspacePath() = default;
	virtual ~WorkspacePath() {};

	
	HomePathID getID() const override {
		return HomePathID::WorkspacePath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	bool setAllUserDefaultHome() override;
	bool setLocalUserDefaultHome() override;
	bool setEnvironmentPath() override;

	static std::string& get();

	static WorkspacePath& getObject() {
		static WorkspacePath workspacePath;
		return workspacePath;
	}
};

class PicturePath : public HomePathsBase {

	static std::string m_path;
	
public:
	PicturePath() = default;
	virtual ~PicturePath() {};
	
	HomePathID getID() const override {
		return HomePathID::PicturePath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	bool setAllUserDefaultHome() override;
	bool setLocalUserDefaultHome() override;
	bool setEnvironmentPath() override;

	static std::string& get();

	static HomePathsBase& getObject() {
		static PicturePath picturePath;
		return picturePath;
	}
};

class WWWImagePath : public HomePathsBase {

	static std::string m_path;
	
public:
	WWWImagePath() = default;
	virtual ~WWWImagePath() {};

	HomePathID getID() const override {
		return HomePathID::WWWImagePath;
	};

	bool init();
	bool setPath(const char* p);
	bool setAllUserDefaultHome();
	bool setLocalUserDefaultHome();
	bool setEnvironmentPath() override;

	static std::string& get();

	static HomePathsBase& getObject() {
		static PicturePath wwwImagePath;
		return wwwImagePath;
	}
};