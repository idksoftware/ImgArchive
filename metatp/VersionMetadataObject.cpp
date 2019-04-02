#include "VersionMetadataObject.h"

namespace simplearchive {
	
VersionMetadataSchema VersionMetadataObject::m_tableSchema;
//VersionMetadataObject::VersionMetadataObject(MTTableSchema &ts) : MTRow(ts)
//{
//}


VersionMetadataObject::~VersionMetadataObject()
{
}


VersionTable::VersionTable() : MTTable(new VersionMetadataSchema)
{

}
VersionTable::~VersionTable() {

}

}; // namespace simplearchive 