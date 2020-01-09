#pragma once

#include <string>
enum class HomePathType {
	Env,
	UserOnly,
	AllUsers,
	Unknown
};

class HomePath
{
	static std::string m_homePath;
	static bool m_found;	// string found
	static bool m_valid;	// in file system
	
public:
	HomePath() = default;
	~HomePath() = default;
	static std::string get();
	static HomePathType type();
	static bool isfound() {	// string found
		return m_found;
	}
	static bool isValid() {	// in file system
		return m_valid;
	}
};

