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
	bool m_found;	// string found
	bool m_valid;	// in file system

	HomePathType m_type;
	HPError m_error;
public:
	HomePathsBase() = default;
	virtual ~HomePathsBase() {};

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
};


class HomePath : public HomePathsBase {

	static std::string m_homePath;
	
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

	static std::string get();
	static HomePathsBase* getObject() {
		static HomePath homePath;
		return &homePath;
	}
};

class MasterPath : public HomePathsBase {

	static std::string m_homePath;
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

	std::string get();
	HomePathsBase* getObject() {
		static MasterPath masterPath;
		return &masterPath;
	}
};


class DerivativePath : public HomePathsBase {

	static std::string m_homePath;
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
	bool setLocalUserDefaultHome();

	static std::string get();
	static HomePathsBase* getObject() {
		static DerivativePath derivativePath;
		return &derivativePath;
	}
};

class WorkspacePath : public HomePathsBase {

	static std::string m_homePath;
	
protected:
	
public:
	WorkspacePath() = default;
	virtual ~WorkspacePath() {};

	
	HomePathID getID() const override {
		return HomePathID::WorkspacePath;
	};

	static std::string get();

	bool init() override;
	bool setPath(const char* p) override;
	bool setAllUserDefaultHome() override;
	bool setLocalUserDefaultHome() override;

	static HomePathsBase* getObject() {
		static DerivativePath derivativePath;
		return &derivativePath;
	}
};

class PicturePath : public HomePathsBase {

	static std::string m_homePath;
	
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

	static std::string get();
	static HomePathsBase* getObject() {
		static PicturePath picturePath;
		return &picturePath;
	}
};

class WWWImagePath : public HomePathsBase {

	static std::string m_homePath;
	
public:
	WWWImagePath() = default;
	virtual ~WWWImagePath() {};

	HomePathID getID() const override {
		return HomePathID::WWWImagePath;
	};

	bool init();
	static bool setPath(const char* p);
	static bool setAllUserDefaultHome();
	static bool setLocalUserDefaultHome();

	static std::string get();
	static HomePathsBase* getObject() {
		static PicturePath wwwImagePath;
		return &wwwImagePath;
	}
};