#pragma once

#include <string>
#include <vector>
#include <memory>

enum class HPSource {

};

enum class HomePathType {
	Default,	// Default value
	LocalEnv,	// Local Environment set
	SystemEnv,	// System Environment set
	UserOnly,	// user only archive
	AllUsers,	// all users archive
	Unknown
};

enum class HPError {
	Ok,					// OK
	initaliseError,
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

	bool baseSysInit(std::string& path, const char* relativePath, const char* enviromentPath);
	bool baseUserInit(std::string& path, const char* relativePath, const char* enviromentPath);
	void baseEnableInit(bool& option, bool defaultOption, const char* enviromentOption);
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
	
	std::string errorStr();

	virtual bool init() = 0;
	virtual HomePathID getID() const = 0;
	virtual bool setPath(const char* p) = 0;
	
	static bool loadEnv();
};

class ImgArchiveHome {
	static std::vector<HomePathsBase*> m_list;

	bool m_found{ false };	// string found
	bool m_valid{ false };	// in file system
	HomePathType m_type{ HomePathType::Unknown };
	HPError m_error{ HPError::Unknown };
	static std::string m_imgArchiveHome;

	void initHomePaths();
	
	bool setArchiveHome();
	
protected:

	ImgArchiveHome() noexcept {
		if (setArchiveHome() == false) {
			return; // m_error set
		}
		HomePathsBase::loadEnv();
		if (m_valid == true) {
			initHomePaths();
			init();
		}
	}

public:
	
	~ImgArchiveHome() = default;

	bool init();

	HPError error() noexcept {
		return m_error;
	}

	std::string errorStr();

	HomePathType type() noexcept {
		return m_type;
	}
	bool isFound() noexcept {	// string found
		return m_found;
	}
	bool isValid() noexcept {	// in file system
		return m_valid;
	}
	
	static const std::string& getImgArchiveHome();

	static ImgArchiveHome& getObject() {
		static ImgArchiveHome imgArchiveHome;
		return imgArchiveHome;
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
	
	static const std::string& get();
	static MasterPath& getObject() {
		static MasterPath masterPath;
		return masterPath;
	}
};

class MasterBackupOnePath : public HomePathsBase {

	static std::string m_path;
	static bool m_enabled;
protected:

public:
	MasterBackupOnePath() = default;
	virtual ~MasterBackupOnePath() {};

	virtual HomePathID getID() const {
		return HomePathID::MasterPath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	void enabled(bool b) { m_enabled = b; };

	static bool enabled() { return m_enabled; };
	static const std::string& get();
	static MasterBackupOnePath& getObject() {
		static MasterBackupOnePath masterBackupOnePath;
		return masterBackupOnePath;
	}
};

class MasterBackupTwoPath : public HomePathsBase {

	static std::string m_path;
	static bool m_enabled;
protected:

public:
	MasterBackupTwoPath() = default;
	virtual ~MasterBackupTwoPath() {};

	virtual HomePathID getID() const {
		return HomePathID::MasterPath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	void enabled(bool b) { m_enabled = b; };

	static bool enabled() { return m_enabled; };
	static const std::string& get();
	static MasterBackupTwoPath& getObject() {
		static MasterBackupTwoPath masterBackupTwoPath;
		return masterBackupTwoPath;
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

	static const std::string& get();
	static DerivativePath& getObject() {
		static DerivativePath derivativePath;
		return derivativePath;
	}
};

class DerivativeBackupOnePath : public HomePathsBase {

	static std::string m_path;
	static bool m_enabled;
protected:

public:
	DerivativeBackupOnePath() = default;
	virtual ~DerivativeBackupOnePath() {};

	virtual HomePathID getID() const {
		return HomePathID::MasterPath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	void enabled(bool b) { m_enabled = b; };

	static bool enabled() { return m_enabled; };
	static const std::string& get();
	static DerivativeBackupOnePath& getObject() {
		static DerivativeBackupOnePath derivativeBackupOnePath;
		return derivativeBackupOnePath;
	}
};

class DerivativeBackupTwoPath : public HomePathsBase {

	static std::string m_path;
	static bool m_enabled;
protected:

public:
	DerivativeBackupTwoPath() = default;
	virtual ~DerivativeBackupTwoPath() {};

	virtual HomePathID getID() const {
		return HomePathID::MasterPath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	void enabled(bool b) { m_enabled = b; };

	static bool enabled() { return m_enabled; };
	static const std::string& get();
	static DerivativeBackupTwoPath& getObject() {
		static DerivativeBackupTwoPath derivativeBackupTwoPath;
		return derivativeBackupTwoPath;
	}
};

class WorkspacePath : public HomePathsBase {

	static std::string m_path;
	static bool m_autoView;
	static bool m_autoCheckout;
protected:
	
public:
	WorkspacePath() = default;
	virtual ~WorkspacePath() {};
	
	HomePathID getID() const override {
		return HomePathID::WorkspacePath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	void setAutoCheckout(bool b) { m_autoCheckout; };
	void setAutoViewOn(bool b) { m_autoView; };

	static bool autoCheckout();
	static bool autoViewOn();
	static const std::string& get();

	static WorkspacePath& getObject() {
		static WorkspacePath workspacePath;
		return workspacePath;
	}
};

class PicturePath : public HomePathsBase {

	static std::string m_path;
	static bool m_autoView;
public:
	PicturePath() = default;
	virtual ~PicturePath() {};
	
	HomePathID getID() const override {
		return HomePathID::PicturePath;
	};

	bool init() override;
	bool setPath(const char* p) override;
	void setAutoViewOn(bool b) { m_autoView; };

	static bool autoViewOn();
	static const std::string& get();

	static PicturePath& getObject() {
		static PicturePath picturePath;
		return picturePath;
	}
};

class WWWImagePath : public HomePathsBase {

	static std::string m_path;
	static bool m_autoView;
public:
	WWWImagePath() = default;
	virtual ~WWWImagePath() {};

	HomePathID getID() const override {
		return HomePathID::WWWImagePath;
	};

	bool init();
	bool setPath(const char* p);
	void setAutoViewOn(bool b) { m_autoView; };

	static bool autoViewOn();
	static const std::string& get();

	static WWWImagePath& getObject() {
		static WWWImagePath wwwImagePath;
		return wwwImagePath;
	}
};