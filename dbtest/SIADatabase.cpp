#include "SIADatabase.h"
#include "Schema.h"

namespace SIADBLite {

	SIADatabase SIADatabase::instance;

	SIADatabase::SIADatabase()
	{
		addTable(new DerivativeMetadataSchema);
		addTable(new ExifObjectSchema);
		addTable(new BasicMetadataSchema);
		addTable(new MetadataSchema);
		addTable(new VersionMetadataSchema);
	}


	SIADatabase::~SIADatabase()
	{
	}

	SIADatabase &SIADatabase::get() {
		return instance;
	}

} // namespace SIADBLite