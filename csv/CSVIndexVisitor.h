#pragma once

#include <string>
#include <vector>
#include <memory>

#include "MetaType.h"
#include "AddressScope.h"
#include "HistoryEvent.h"

namespace simplearchive {

	class CSVIndexAction;

	class CSVIndexVisitorBase {
	protected:
		std::shared_ptr<CSVIndexAction> m_indexAction;
		AddressScope m_addressScope;
		
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CSVIndexVisitorBase(std::shared_ptr<CSVIndexAction> indexAction);
		// Destructor
		virtual ~CSVIndexVisitorBase();

		bool setScope(const char* scope);

		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder) = 0;
		
		CSVIndexAction &getIndexAction() {
			return *m_indexAction;
		}
	};


	class CSVIndexVisitorHistory : public CSVIndexVisitorBase {
	

	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CSVIndexVisitorHistory(std::shared_ptr<CSVIndexAction> indexAction) : CSVIndexVisitorBase(indexAction) {};
		// Destructor
		virtual ~CSVIndexVisitorHistory() {};

		
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder);

		
	};
};