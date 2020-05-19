/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "MetaType.h"
#include "AddressScope.h"
#include "HistoryEvent.h"

namespace simplearchive {


	/// This class is used by the DirectoryVisitor. The DirectoryVisitor class will
	/// call the make function to make FolderVisitor objects when required.
	/// This must be used as a base class for new classes that the DirectoryVisitor uses to
	/// process folders the DirectoryVisitor class visits.
	///
	class CheckoutRow;
	class CheckoutPartition;
	class CheckoutStatusLog;
	//class AddressScope;
	/*
	class ReporterItem {
	public:
		enum class Status {
			Completed,
			Warning,
			Error,
			Fatal,
			Unkown
		};
		const char *statusString();
		std::string m_message;
		Status m_status;
		ReporterItem(ReporterItem::Status status, std::string &message);
	};
	class IndexActionReporter : public std::vector<ReporterItem> {
	public:
		void add(ReporterItem::Status status, const char *format, ...);
	};
	*/
	class IndexAction {
		friend class IndexVisitor;
		
	protected:
		
		std::shared_ptr<CheckoutRow> m_currentRow;
		std::shared_ptr<CheckoutPartition> m_currentPartition;
		
		virtual bool onMetadata(const char* path, const char* name) { return true; };
		/// This function is a factory function used to create new FolderVisitor objects.
		AddressScope* m_addressScope { nullptr };

	public:
		/// On the start of each directory found, this function is run.
		virtual bool onStart() { return true; };
		/// At the end of each directory found, this function is run.
		virtual bool onEnd() { return true; };
		/// On finding a file, this function is run.
		virtual bool onYearFolder(const char *name) { return true; };
		/// On finding a file, this function is run.
		virtual bool onYearEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onDayFolder(const char *name) { return true; };
		/// On finding a directory, this function is run.
		virtual bool onDayEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onImage(const char* name) { return true; };

		//virtual bool action(std::unique_ptr<AddressScope> scope, std::shared_ptr<MTRow> row) { return true; };

		
	public:
		/// Constructor
		IndexAction() = default;
		/// Distructor
		virtual ~IndexAction() = default;
		void setAddressScope(AddressScope& addressScope) {
			m_addressScope = &addressScope;
		}
	};

	
	
	class IndexVisitor {
	protected:
		std::shared_ptr<IndexAction> m_indexAction;
		AddressScope m_addressScope;
		std::string m_indexPath;
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		IndexVisitor(std::shared_ptr<IndexAction> indexAction);
		// Destructor
		virtual ~IndexVisitor();

		void setPath(const char* indexPath);
		
		bool setScope(const char *scope);
		
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		virtual bool process(const char* rootFolder);
		virtual bool process();
	};

	/*

	class CheckoutTableIndex : public IndexVisitor {
	protected:
		static std::string m_primaryIndex;
		AddressScope m_addressScope;
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		CheckoutTableIndex(std::shared_ptr<IndexAction> indexAction) : IndexVisitor(indexAction) {};
		// Destructor
		virtual ~CheckoutTableIndex() = default;

		static bool Init(const char* primaryIndex);
		
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		bool process(const char* rootFolder);
		bool process();
	};

	*/
};