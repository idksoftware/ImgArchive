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
		
		std::string m_Master;
		std::string m_workspace;
		std::string m_primaryIndex;
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
		virtual bool onImage() { return true; };

		virtual bool onMetadata(const char *path, const char *name) { return true; };
		/// This function is a factory function used to create new FolderVisitor objects.
		
	public:
		/// Constructor
		IndexAction() = default;
		
		void init(const char *master, const char *workspace, const char *primaryIndex) {
			m_Master = master;
			m_workspace = workspace;
			m_primaryIndex = primaryIndex;
		};
		/// Distructor
		virtual ~IndexAction() = default;
		
	};

	class StatusAction : public IndexAction {
		std::shared_ptr<CheckoutStatusLog> log;
	protected:
		
		/// On the start of each directory found, this function is run.
		virtual bool onStart();
		/// At the end of each directory found, this function is run.
		virtual bool onEnd();
		/// On finding a file, this function is run.
		virtual bool onYearFolder(const char *name) { return true; };
		/// On finding a file, this function is run.
		virtual bool onYearEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onDayFolder(const char *name) { return true; };
		/// On finding a directory, this function is run.
		virtual bool onDayEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onImage();

		virtual bool onMetadata(const char *path, const char *name) { return true; };
		/// This function is a factory function used to create new FolderVisitor objects.
		
	public:
		/// Constructor
		StatusAction() {};
		/// Distructor
		virtual ~StatusAction() {};

	};

	
	class IndexVisitor {
		static std::string m_master;
		static std::string m_workspace;
		static std::string m_primaryIndex;
		std::shared_ptr<IndexAction> m_indexAction;
		std::unique_ptr<AddressScope> m_addressScope;
		bool process(const char *rootFolder);
	public:
		/// Constructor
		/// @parm folderVisitor - pointer to FolderVisitor
		IndexVisitor(std::shared_ptr<IndexAction> indexAction);
		static bool Init(const char *Master, const char *workspace, const char *primaryIndex);
		bool setScope(const char *scope);
		// Destructor
		virtual ~IndexVisitor();
		/// This Function processes the files under the root using the
		/// FolderVisitor class passed in the constructor
		bool process();
		
		
	};

	
};