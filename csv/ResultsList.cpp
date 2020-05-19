#include "ResultsList.h"
#include <iostream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	ResultsList::ResultsList(std::shared_ptr<MTTableSchema> tableSchema) : m_tableSchema(tableSchema) {
		// TODO Auto-generated constructor stub

	}

	ResultsList::~ResultsList() {
		// TODO Auto-generated destructor stub
	}


} /* namespace simplearchive */
