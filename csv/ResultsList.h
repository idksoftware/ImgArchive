#pragma once

#include <string>
#include <list>
#include <memory>
#include "MetaType.h"
#include "ResultsList.h"

namespace simplearchive {
	/**
	 *
	 */

	class MTTableSchema;



	class ResultsList : public std::vector<SharedMTRow> {

	
		std::shared_ptr<MTTableSchema> m_tableSchema;
		
	public:
		ResultsList(std::shared_ptr<MTTableSchema> tableSchema);
		virtual ~ResultsList();
		MTTableSchema& getTableSchema() {
			return *m_tableSchema;
		}
	};

} /* namespace simplearchive */


