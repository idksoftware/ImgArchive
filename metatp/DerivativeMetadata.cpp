#include "DerivativeMetadata.h"

#include <memory>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "MetaType.h"
#include "SAUtils.h"
#include "pathcontroller.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	DerivativeMetadataSchema DerivativeMetadataRow::m_schema;
	DerivativeMetadataRow::DerivativeMetadataRow() : MTRow(m_schema)
	{
	}

	DerivativeMetadataRow::~DerivativeMetadataRow()
	{
	}


} // namespace simplearchive {