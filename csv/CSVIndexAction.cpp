#include "CSVIndexAction.h"
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
//#include <dirent.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
//#include <unistd.h>
#ifdef WINDOWS
#include <sysstat.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdarg>
#include "CIDKFileFind.h"
#include "IndexVisitor.h"
#include "SAUtils.h"

#include "pathcontroller.h"
#include "DBDefines.h"
#include "MetaType.h"
#include "AddressScope.h"
#include "CLogger.h"
#include "ResultsList.h"

namespace simplearchive {

	bool CSVIndexAction::onEnd()
	{
		//if (!resultsList->write(ResultsList::FormatType::Human)) {
		//	ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
		//	return false;
		//}
		return true;
	};

	bool CSVIndexAction::onImage(const char* name)
	{
		std::shared_ptr<MTTable> currentPartition = std::make_shared<MTTable>(m_mtTableSchema);

		if (currentPartition->read(name) == false) {
			return false;
		}

		for (auto i = currentPartition->begin(); i != currentPartition->end(); i++) {
			std::shared_ptr<MTRow> row = *i;
			const MTTableSchema& ts = row->getSchema();
			m_resultsList->push_back(row);

		}
		return true;
	}

	bool CSVIndexAction::onStart()
	{
		m_resultsList = std::make_shared<ResultsList>(m_mtTableSchema);
		return true;
	};

	bool CSVIndexAction::onDayFolder(const char* name)
	{
		return true;
	}

};