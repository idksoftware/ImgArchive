#pragma once
#include <string>
#include <vector>
#include <memory>

#include "MetaType.h"
#include "AddressScope.h"

namespace simplearchive {

	class ResultsList;

	class CSVIndexAction {
		friend class IndexVisitor;

	protected:

		/// This function is a factory function used to create new FolderVisitor objects.
		AddressScope* m_addressScope{ nullptr };
		std::shared_ptr<ResultsList> m_resultsList;
		
		std::shared_ptr<MTTableSchema> m_mtTableSchema;
	public:
		/// On the start of each directory found, this function is run.
		virtual bool onStart();
		/// At the end of each directory found, this function is run.
		virtual bool onEnd();
		/// On finding a file, this function is run.
		virtual bool onYearFolder(const char* name) { return true; };
		/// On finding a file, this function is run.
		virtual bool onYearEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onDayFolder(const char* name);
		/// On finding a directory, this function is run.
		virtual bool onDayEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onImage(const char* name);

		//virtual bool action(std::unique_ptr<AddressScope> scope, std::shared_ptr<MTRow> row) { return true; };


	public:
		/// Constructor
		CSVIndexAction(std::shared_ptr<MTTableSchema> mtTableSchema) : m_mtTableSchema(mtTableSchema) {};
		/// Distructor
		virtual ~CSVIndexAction() = default;
		void setAddressScope(AddressScope& addressScope) {
			m_addressScope = &addressScope;
		}

		std::shared_ptr<ResultsList> getResultsList() {
			return m_resultsList;
		}
	};

};
