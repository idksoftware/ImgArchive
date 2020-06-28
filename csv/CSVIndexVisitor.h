#pragma once

#include <string>
#include <vector>
#include <memory>

#include "MetaType.h"
#include "AddressScope.h"
#include "HistoryEvent.h"

namespace simplearchive {

	/*
	class CVSActionTrigger {
	public:
		CVSActionTrigger() = default;
		~CVSActionTrigger() = default;

		bool OnRow(MTRow row) { return true; };

	};
	*/

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

	/*
	class CSVIndexVisitorWithTriggerBase : public CSVIndexVisitorBase {
	protected:
		std::shared_ptr<CVSActionTrigger> m_triggerAction;
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CSVIndexVisitorWithTriggerBase(std::shared_ptr<CVSActionTrigger> triggerAction, std::shared_ptr<CSVIndexAction> indexAction)
				: m_triggerAction(triggerAction),
				CSVIndexVisitorBase(indexAction)
		{};
		// Destructor
		virtual ~CSVIndexVisitorWithTriggerBase() {};

		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder) = 0;

		
	};
	*/

	class CSVIndexSystemHistory : public CSVIndexVisitorBase {
		
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CSVIndexSystemHistory(std::shared_ptr<CSVIndexAction> indexAction) : CSVIndexVisitorBase(indexAction) {};
		// Destructor
		virtual ~CSVIndexSystemHistory() {};
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder);
	};

	class CSVIndexImageHistory : public CSVIndexVisitorBase {
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CSVIndexImageHistory(std::shared_ptr<CSVIndexAction> indexAction) : CSVIndexVisitorBase(indexAction) {};
		// Destructor
		virtual ~CSVIndexImageHistory() {};
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder);
	};

	class CSVIndexCheckoutStatus : public CSVIndexVisitorBase {
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CSVIndexCheckoutStatus(std::shared_ptr<CSVIndexAction> indexAction) : CSVIndexVisitorBase(indexAction) {};
		// Destructor
		virtual ~CSVIndexCheckoutStatus() {};
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder);
	};
};