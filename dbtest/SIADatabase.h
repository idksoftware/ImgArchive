#pragma once

#include "DBLite.h"
#include <mutex>

namespace SIADBLite {

	class SIADatabase : public DBLite::DBLiteDatabase
	{
		static SIADatabase instance;
		SIADatabase();
	public:
		
		~SIADatabase();

		SIADatabase(SIADatabase const&) = delete;
		void operator=(SIADatabase const&) = delete;

		static SIADatabase &get();
	};

} // SIADBLite