#pragma once

/*
	This class contain the Primary mirror that all other mirrors derive there data.
*/
#include <string>
#include <cstdint>

namespace simplearchive {

	class PrimaryMirror
	{
		uint64_t m_primaryIndex;

		std::string m_masterRepositoryPath;
		std::string m_derivativeRepositoryPath;
		std::string m_primaryMirrorPath;
		std::string m_primaryIndexPath;

	public:
		PrimaryMirror();
		~PrimaryMirror() = default;
		static PrimaryMirror get();

		void setMasterRepositoryPath(const char* path);
		void setDerivativeRepositoryPath(const char* path);
		void setPrimaryIndexPath(const char* path);
		void setPrimaryMirrorPath(const char* path);

		bool process();

	};
};
