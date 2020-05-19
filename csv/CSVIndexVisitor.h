#pragma once

#include <string>
#include <vector>
#include <memory>

#include "MetaType.h"
#include "AddressScope.h"
#include "HistoryEvent.h"

namespace simplearchive {

	class CSVIndexAction;

	class CSVIndexVisitor {
	protected:
		std::shared_ptr<CSVIndexAction> m_indexAction;
		AddressScope m_addressScope;
		std::string m_indexPath;
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CSVIndexVisitor(std::shared_ptr<CSVIndexAction> indexAction, const char* indexPath);
		// Destructor
		virtual ~CSVIndexVisitor();

		

		bool setScope(const char* scope);

		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder);
		virtual bool process();
		CSVIndexAction &getIndexAction() {
			return *m_indexAction;
		}
	};

};