#ifndef ExternalProcess_H_
#define ExternalProcess_H_

#include <string>

namespace simplearchive {
	/**
	* This enables
	*/
	class ExifObject;
	class ExternalProcess {

		std::string replaceToken(std::string &str, const char *tok, const char *repstr);
		std::string m_cmdLine;
	public:
		/// @brief Constructor for class.
		ExternalProcess();

		/// @brief Distructor for class.
		virtual ~ExternalProcess();
		bool init();
		virtual bool process(const char *imageFile);
		std::string getDate();
		std::string replace(std::string &commandLine, std::string &in, std::string &out);
	};

} /* namespace simplearchive */
#endif /* ExternalProcess_H_ */