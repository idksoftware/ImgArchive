#pragma once

#include <string>
#include <vector>

namespace simplearchive {

	class VersionsListItem {
		int m_version;
		std::string m_versionMetadataPath;
	public:
		VersionsListItem(int version, const char *versionMetadataPath) :
			m_version(version), m_versionMetadataPath(versionMetadataPath) {}
		int getVersion() { return m_version; }
		const char *getVersionMetadataPath() { return m_versionMetadataPath.c_str(); }
	};

	class VersionsList : public std::vector<VersionsListItem> {
	public:
		VersionsList() = default;
		~VersionsList() = default;
		void add(int version, const char *versionMetadataPath) {
			push_back(VersionsListItem(version, versionMetadataPath));
		}
	};

}
