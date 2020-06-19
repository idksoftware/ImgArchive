#include "UpdateTemplateFile.h"
#include "ConfigReader.h"

namespace simplearchive {

	bool UpdateTemplateFile::update(const char* path, const char* file, const char* option, const char* value)
	{
		m_rootPath = path;
		m_file = file;

		std::unique_ptr<ConfigBlock> m_templateFile(new ConfigBlock);

		ConfigReader configReader;
		if (configReader.read(path, file, *m_templateFile) == false) {
			return false;
		}


		ConfigWriter configWriter(*m_templateFile);
		configWriter.update(option, value);
		
		std::string datafile = path;
		datafile += '/';
		datafile += file;

		if (configWriter.write(datafile.c_str()) == false) {
			return false;
		}

		/*
		std::ofstream configFile;
		configFile.open(m_configfile.c_str());
		//configFile.open("C:\\temp\\t.dat");
		if (configFile.bad()) {
			return false;
		}
		configFile << imgaConfig;
		std::cout << imgaConfig;
		configFile.close();
		*/
		return true;
	}

};