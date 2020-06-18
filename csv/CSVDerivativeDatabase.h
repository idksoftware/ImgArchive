#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include <memory>
#include "DBDefines.h"
#include "MetadataObject.h"
#include "DerivativeMetadata.h"
#include "ArchivePath.h"


namespace simplearchive {

	class MirrorDB;
	//class CSVDatabase : MTDatabase {
	class CSVDerivativeDatabase {
		CSVDerivativeDatabase();
		static std::shared_ptr<CSVDerivativeDatabase> m_this;
		static std::string m_dbpath;
		//std::unique_ptr<MirrorDB> m_mirrorDB;

	public:
		/// Set the database path for the archive. This will normally under Master/.root
		static void setDBPath(const char *path);
		/// gets the database instance.
		static CSVDerivativeDatabase &get();
		/// destructor
		virtual ~CSVDerivativeDatabase();

		void add(DerivativeMetadataRow &metadataObject);
		void update(DerivativeMetadataRow &metadataObject, unsigned int idx, const char *path);
		const DerivativeMetadataRow *get(unsigned int idx, const char *path);
		std::shared_ptr<DerivativeMetadataRow> makeRow();
		std::shared_ptr<DerivativeMetadataRow> get(const char *name, const char *path);
		bool put(const char *name, const char *path, DerivativeMetadataRow &mo);

		bool copy(const char *path);
		bool validate(const char *path);

	};

} // namespace simplearchive {
