#include "PageMaker.h"
#include <exception>
//#include <stdio.h>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <string>
#include "ArchiveVisitor.h"
#include "pathcontroller.h"
#include "MetadataObject.h"
#include "CSVArgs.h"
#include "HtmlPages.h"
#include "HookCmd.h"

namespace simplearchive {

	// Utility class for finding the image size in a sting

	// pic1_thumb.png PNG 480x270 480x270+0+0 8-bit sRGB 255793B 0.000u 0:00.000

	class ImageSize {
		bool lookforSize(std::string &str, int &idx);
		int lookforNum(std::string &str, int &idx);
	public:
		int w;
		int h;
		ImageSize() {};
		bool parse(const char *str);
	};

	bool ImageSize::lookforSize(std::string &str, int &idx) {
		w = 0;
		h = 0;
		w = lookforNum(str, idx);
		if (str[idx] != 'x' && str[idx] != 'X') {
			return false;
		}
		idx++;
		if (!isdigit(str[idx])) {
			return false;
		}
		h = lookforNum(str, idx);
		return true;
	}

	int ImageSize::lookforNum(std::string &str, int &idx) {
		std::string sstr = str;

		int sizeOfString = sstr.length();

		int num = 0;
		while (idx < sizeOfString)
		{
			if (!isdigit(sstr[idx]))
			{
				break;
			}
			num = (num * 10) + (sstr[idx] - '0');
			idx++;
		}
		return num;
	}

	bool ImageSize::parse(const char *str)
	{

		std::string string = str;
		int sizeOfString = string.length();
		int iteration = 0;

		while (iteration < sizeOfString)
		{
			if (isdigit(string[iteration]))
			{
				if (lookforSize(string, iteration)) {
					return true; // size found
				}

			}

			iteration++;

		}

		return false;
	}


	/* ======================================

	PageMaker
	
	========================================*/

	class HtmlTemplate : public std::vector<std::string> {

	};
	class PageNode;
	class MakePageBlocks {
		std::shared_ptr<PageNode> m_imageTree;
		std::shared_ptr<PageNode> m_imageNodes;
		std::string  m_htmlRoot;
		std::string  m_rootPath;
		std::string m_archiveRoot;
		
		bool makePageBlock(std::shared_ptr<PageNode> pn);
		bool creatImageHtmlfile(std::shared_ptr<PageNode> dayNode, std::shared_ptr<PageNode> imageNode);
		
		bool creatDayHtmlfile(std::shared_ptr<PageNode> dayNode, DayFileItem &fileItem);
		bool creatYearHtmlfile(std::shared_ptr<PageNode> archive);
		bool creatDaysHtmlfile(std::shared_ptr<PageNode> year);
	public:
		MakePageBlocks(const char *root, std::shared_ptr<PageNode> imageTree, std::shared_ptr<PageNode> imageNodes, const char *htmlRoot, const char *archiveRoot);
		bool makeImageBlocks();
		bool makeContentsHtml();
	};

	class TemplateReader {
	public:
		typedef enum {
			Error = -1,
			Text,
			YearNav,
			DayNav,
			Images
		} Token;
		bool m_logging;
		int m_currentLineNumber;
	protected:
		/// @brief parses a line of text
		TemplateReader::Token parse(const char *text);
		//std::string includePath(int pos, std::string line);
		int m_includeCnt;
		std::string m_path;
		/// @brief deliminator for the key / value pair.
		char m_delimChar;
		std::string m_blockName;
	public:
		/// @brief Constructor for class
		TemplateReader() {};


		/// @brief Destructor for the class.
		virtual ~TemplateReader() {};

		/// @brief This function attempts to read a configuration file.
		/// @param    datafile	File name to read.
		/// @param    config		Config class to be filled out.
		/// @return	returns true if read correctly.
		bool read(const char *datafile, HtmlTemplate &config);
		/// @brief This function attempts to read a configuration file.
		/// @param    str	string to read.
		/// @param    config		Config class to be filled out.
		/// @return	returns true if read correctly.
		bool read(const std::string &str, HtmlTemplate &config);

		virtual bool process() {
			return true;
		};

		void setNoLogging() {
			m_logging = false;
		}

		int getCurrentLineNumber() {
			return m_currentLineNumber;
		}
	};

	PageIndexMaker::PageIndexMaker()
	{

	}
	PageIndexMaker::~PageIndexMaker() {

	}

	bool isMainImage(const char *name) {
		std::string dayName = SAUtils::getFilenameNoExt(name);
		if (dayName[dayName.size() - 2] == '_') {
			char sizeChar = dayName[dayName.size() - 1];
			if (sizeChar == '1' || sizeChar == '2' || sizeChar == '3' || sizeChar == 't') {
				return false;
			}
		}
		return true;
	}
	
	bool PageIndexMaker::process() {

		HomeHtmlPage homeHtmlPage;
		std::string fileOut = m_htmlPath;
		fileOut += "/homenav.html";
		homeHtmlPage.make(fileOut.c_str());
		ArchiveHtmlPage archiveHtmlPage;
		fileOut = m_htmlPath;
		//fileOut += "/archivenav.html";
		archiveHtmlPage.make(fileOut.c_str());
		HistoryHtmlPage historyHtmlPage;
		fileOut = m_htmlPath;
		fileOut += "/historynav.html";
		//historyHtmlPage.make(fileOut.c_str());
		JournalHtmlPage journalHtmlPage;
		fileOut = m_htmlPath;
		fileOut += "/journalnav.html";
		//journalHtmlPage.make(fileOut.c_str());
		MenuHtmlPage menuHtmlPage;
		fileOut = m_htmlPath;
		fileOut += "/menunav.html";
		menuHtmlPage.make(fileOut.c_str());
		AboutHtmlPage aboutHtmlPage;
		fileOut = m_htmlPath;
		fileOut += "/aboutnav.html";
		aboutHtmlPage.make(fileOut.c_str());
		return true;
	}

	PageImageMaker::PageImageMaker()
	{
	}

	PageImageMaker::~PageImageMaker()
	{
	}

	class PageNode : public std::vector<std::shared_ptr<PageNode>> {
	public:
		enum NodeType {
			Root,
			Image,
			Year,
			Day
		};
		PageNode(NodeType type, std::string name) {
			m_type = type;
			m_name = name;
		}

		~PageNode() {};

		const char *getName() {
			return m_name.c_str();
		}

		NodeType &getType() {
			return m_type;
		}
		void print() {
			cout << "Name: " << m_name << " Type: " << m_type << '\n';

		}
	private:
		NodeType m_type;
		std::string m_name;

	};

	
	class PageMakerVisitor : public AVAction {

		std::shared_ptr<PageNode> m_imageTree;
		std::shared_ptr<PageNode> m_imageNodes;
		std::shared_ptr<PageNode> m_curYearPageNode;
		std::shared_ptr<PageNode> m_curDayPageNode;

	protected:
		/// On the start of each directory found, this function is run.
		virtual void onStart() {
			//printf("onStart\n");
			std::string root = "Home";
			m_imageTree = std::make_shared<PageNode>(PageNode::Root, root);
			m_imageNodes = std::make_shared<PageNode>(PageNode::Root, root);
		};
		/// At the end of each directory found, this function is run.
		virtual void onEnd() {
			//printf("onEnd\n");
		};
		/// On finding a file, this function is run.
		virtual void onYearFolder(const char *name) {
			
			//printf("onYearFolder %s: \n", name);
			m_curYearPageNode = std::make_shared<PageNode>(PageNode::Year, name);
			m_imageTree->push_back(m_curYearPageNode);
		};
		/// On finding a file, this function is run.
		virtual void onYearEnd() {
			//printf("onYearEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onDayFolder(const char *name) {
			
			
			m_curDayPageNode = std::make_shared<PageNode>(PageNode::Day, name);
			m_curYearPageNode->push_back(m_curDayPageNode);
			m_imageNodes->push_back(m_curDayPageNode);
		};
		/// On finding a directory, this function is run.
		virtual void onDayEnd() {
			//printf("onDayEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onImage(const char *path, const char *name) {
			
			if (!isMainImage(name)) {
				return;
			}
			std::shared_ptr<PageNode> imagePageNode = std::make_shared<PageNode>(PageNode::Image, name);
			m_curDayPageNode->push_back(std::move(imagePageNode));
		};

		/// This function is a factory function used to create new FolderVisitor objects.
		virtual PageMakerVisitor *make() { return new PageMakerVisitor; }
	public:
		/// Constructor
		PageMakerVisitor() {};
		/// Distructor
		virtual ~PageMakerVisitor() {};

		std::shared_ptr<PageNode> getImageTree() {
			return m_imageTree;
		}
		std::shared_ptr<PageNode> getimageNodes() {
			return m_imageNodes;
		}
	
	};

	bool PageImageMaker::process(DaysProcessed &daysProcessed) {
		
		if (SAUtils::DirExists(m_htmlPath.c_str()) == false) {
			return false;
		}
		std::shared_ptr<PageMakerVisitor> pageMakerVisitor = std::make_shared<PageMakerVisitor>();
		
		ArchiveVisitor archiveVisitor(pageMakerVisitor.get());
		archiveVisitor.setPostfix("/images");
		if (archiveVisitor.process(m_htmlPath.c_str()) == false) {
			return false;
		}
		std::shared_ptr<PageNode> imageTree = pageMakerVisitor->getImageTree();
		std::shared_ptr<PageNode> imageNodes = pageMakerVisitor->getimageNodes();
		MakePageBlocks makePageBlocks(m_rootPath.c_str(), imageTree, imageNodes, m_htmlPath.c_str(), m_archiveRootPath.c_str());
		makePageBlocks.makeContentsHtml();
		makePageBlocks.makeImageBlocks();
		return true;
	}

	bool PageImageMaker::process(const char *fileIn, const char *fileOut) {
		std::string pathIn = m_templatePath;
		pathIn += '/';
		pathIn += fileIn;

		std::string pathOut = m_templatePath;
		pathOut += '/';
		pathOut += fileOut;

		if (SAUtils::FileExists(pathIn.c_str()) == false) {
			return false;
		}
		
		TemplateReader templateReader;
		HtmlTemplate htmlTemplate;
		if (templateReader.read(pathIn.c_str(), htmlTemplate) == false) {
			return false;
		}
		/*
		//m_templatePath
		std::ifstream input("filename.ext", std::ifstream::in);
		std::ofstream output(fileOut, std::ofstream::out);

		for (std::string line; std::getline(input, line);)
		{
			output << "<Data" << "</Data>";
		}
		*/
		return true;

	}

	MakePageBlocks::MakePageBlocks(const char *rootPath, std::shared_ptr<PageNode> imageTree, std::shared_ptr<PageNode> imageNodes, const char *htmlRoot, const char *archiveRoot) {
		m_imageTree = imageTree;
		m_imageNodes = imageNodes;
		m_htmlRoot = htmlRoot;
		m_rootPath = rootPath;
		m_archiveRoot = archiveRoot;
	}

	bool MakePageBlocks::makeImageBlocks() {
		for (auto ii = m_imageTree->begin(); ii != m_imageTree->end(); ii++) {
			std::shared_ptr<PageNode> year = *ii;
			
			if (creatDaysHtmlfile(year) == false) {
				return false;
			}
		}
		if (creatYearHtmlfile(m_imageTree) == false) {
			return false;
		}
		
		for (auto ii = m_imageNodes->begin(); ii != m_imageNodes->end(); ii++) {
			std::shared_ptr<PageNode> pn = *ii;
			if (makePageBlock(pn) == false) {
				return false;
			}
			//if (creatDayHtmlfile(pn) == false) {
			//	return false;
			//}
		}
		return true;
	}

	bool MakePageBlocks::makePageBlock(std::shared_ptr<PageNode> pn) {
		pn->print();
		for (auto ii = pn->begin(); ii != pn->end(); ii++) {
			std::shared_ptr<PageNode> imageNode = *ii;
			imageNode->print();
			if (creatImageHtmlfile(pn, imageNode) == false) {
				return false;
			}	
		}
		
		return true;
	}

	bool MakePageBlocks::creatDaysHtmlfile(std::shared_ptr<PageNode> year) {
		std::string _year = year->getName();
		

		std::vector<DayFileItem> fileItems;
		for (auto ii = year->begin(); ii != year->end(); ii++) {
			std::shared_ptr<PageNode> dayNode = *ii;
			DayFileItem fileItem;
			fileItem.m_day = dayNode->getName();
			fileItem.m_numberOfImages = dayNode->size() / 5;
			fileItems.push_back(fileItem);
			creatDayHtmlfile(dayNode, fileItems.back());
		}

		
		DaysHtmlPage daysHtmlPage(year->getName());
		std::string fileOut = m_htmlRoot;
		fileOut += "/";
		fileOut += _year;
		fileOut += '/';

		for (auto ii = fileItems.begin(); ii != fileItems.end(); ii++) {
			DayFileItem dayNode = *ii;
			daysHtmlPage.setTargetDate(dayNode.m_day.c_str());
			daysHtmlPage.load(fileItems);
			std::string thisFileOut = fileOut;
			thisFileOut += dayNode.m_day; thisFileOut += "/html/";
			thisFileOut += dayNode.m_day; thisFileOut += ".html";
			daysHtmlPage.make(thisFileOut.c_str());
		}
		
		return true;
	}
	// This function creates the thumbnail images infomation
	bool MakePageBlocks::creatDayHtmlfile(std::shared_ptr<PageNode> dayNode, DayFileItem &fileItem) {
		std::string dayName = dayNode->getName();
		std::string cwd = m_htmlRoot;
		std::string htmlPath = m_htmlRoot;
		htmlPath += "/";
		htmlPath += dayName.substr(0, 4);
		htmlPath += '/';
		htmlPath += dayName;
		std::string imageFilePath = htmlPath;
		imageFilePath += '/';
		imageFilePath += "images";
		imageFilePath += '/';

		for (auto ii = dayNode->begin(); ii != dayNode->end(); ii++) {
			std::shared_ptr<PageNode> imageNode = *ii;
			imageNode->print();
			ImageFileItem imageItem;
			imageItem.m_image = imageNode->getName();
			std::string imageFile = imageFilePath;
			imageFile += SAUtils::getFilenameNoExt(imageItem.m_image);
			imageFile += "_t.";
			imageFile += SAUtils::getExtention(imageItem.m_image);

			OnIndentSizeCmd onIndentSizeCmd(imageFile.c_str());
			onIndentSizeCmd.process();

			ImageSize imageSize;
			if (imageSize.parse(onIndentSizeCmd.getOutput())) {
				imageItem.imagewidth = imageSize.w;
				imageItem.imageheight = imageSize.h;
			}

			fileItem.m_items.push_back(imageItem);			
		}
		return true;
	}

	bool MakePageBlocks::creatYearHtmlfile(std::shared_ptr<PageNode> archive) {
		
		
		std::vector<YearFileItem> fileItems;
		for (auto ii = archive->begin(); ii != archive->end(); ii++) {
			std::shared_ptr<PageNode> yearNode = *ii;
			YearFileItem fileItem;
			fileItem.m_year = yearNode->getName();
			fileItem.m_numberOfDays = yearNode->size();
			
			std::shared_ptr<PageNode> first = yearNode->front();
			fileItem.m_firstDay = first->getName();

			for (auto ii = yearNode->begin(); ii != yearNode->end(); ii++) {
				
				std::shared_ptr<PageNode> day = *ii;
				fileItem.m_numberOfImages =+ day->size() / 5;
				
			}
			fileItems.push_back(fileItem);
		}

		ArchiveHtmlPage archiveHtmlPage;
		std::string fileOut = m_rootPath;
		fileOut += "/html/archivenav.html";

		archiveHtmlPage.load(fileItems);
		archiveHtmlPage.make(fileOut.c_str());

		return true;
	}

	bool loadMetadata(MetadataObject &metadataObject, ImageDatails &details) {
		// file
		details.sequenceID = metadataObject.getSequenceIdString();
		details.filename = metadataObject.getFilenameString();
		details.orginalName = metadataObject.getOrginalNameString();
		details.filePath = metadataObject.getFilepathString();
		details.uUID = metadataObject.getUniqueIdString();
		details.mediaType = metadataObject.getMediaTypeString();
		//tmp = metadataObject.getMd5String().c_str();
		details.md5 = metadataObject.getMd5String();
		details.crc = metadataObject.getCrcString();
		details.fileSize = metadataObject.getFileSizeString();
		details.dateCreate = metadataObject.getDateCreateString();
		details.dateModified = metadataObject.getDateModifiedString();
		details.dateAdded = metadataObject.getAddedDateString();
		// Description
		details.description = metadataObject.getDescriptionString();
		details.label = metadataObject.getLabelString();
		details.title = metadataObject.getTitleString();
		details.subject = metadataObject.getRatingString();
		details.rating = metadataObject.getRatingString();
		details.ratingPercent = metadataObject.getRatingPercentString();
		//details->tags = metadataObject.getT
		details.keywords = metadataObject.getKeywordsString();
		details.version = metadataObject.getVersionString();
		// Origin
		details.comment = metadataObject.getCommentString();
		details.author = metadataObject.getAuthorString();
		details.captureDate = metadataObject.getCaptureDateString();
		details.subSetimeOriginal = metadataObject.getSubsetimeOriginaLString();
		details.copyright = metadataObject.getCopyrightString();
		// Image
		details.width = metadataObject.getWidthString();
		details.height = metadataObject.getHeightString();
		details.viewRotation = metadataObject.getViewRotationString();
		details.bitsPersample = metadataObject.getBitsPerSampleString();
		details.xResolution = metadataObject.getXResolutionString();
		details.yResolution = metadataObject.getYResolutionString();
		details.resolutionUnit = metadataObject.getResolutionUnitString();
		details.compression = metadataObject.getCompressionString();
		// Camera
		details.maker = metadataObject.getMakerString();
		details.model = metadataObject.getModelString();
		details.exposureTime = metadataObject.getExposureTimeString();
		details.aperture = metadataObject.getApertureString();
		details.isoSpeedRating = metadataObject.getIsoSpeedRatingString();
		details.exposureBias = metadataObject.getExposureBiasString();
		details.exposureProgram = metadataObject.getExposureProgramString();
		details.subjectDistance = metadataObject.getSubjectDistanceString();
		details.focalLength = metadataObject.getFocalLengthString();
		details.focalLengthIn35mm = metadataObject.getFocalLengthIn35mmString();
		details.flash = metadataObject.getFlashString();
		details.meteringMode = metadataObject.getMeteringModeString();
		details.digitalZoom = metadataObject.getDigitalZoomString();
		// Advanced photo
		details.lightSource = metadataObject.getLightSourceString();
		details.colorSpace = metadataObject.getColorSpaceString();
		details.whiteBalance = metadataObject.getWhiteBalanceString();
		//details->sceneCaptureType = metadataObject.getC
		details.contrast = metadataObject.getContrastString();
		details.saturation = metadataObject.getSaturationString();
		details.sharpness = metadataObject.getSharpnessString();
		details.brightnessValue = metadataObject.getBrightnessValueString();
		details.subjectDistanceRange = metadataObject.getSubjectDIstanceRangeString();
		details.maxAperture = metadataObject.getMaxApertureString();
		details.compressedBitsPerPixel = metadataObject.getCompressedBitsPerPixelString();
		details.flashEnergy = metadataObject.getFlashEnergyString();
		details.exifVersion = metadataObject.getExifVersionString();
		details.software = metadataObject.getSoftwareString();
		details.photometricInterpretation = metadataObject.getPhotometricInterpretationString();
		details.gpsTimeStamp = metadataObject.getGPSTimeStampString();
		//	add(MTSchema(MTSchema::Text, DB_SENSINGMETHOD));
		// Location
		details.latitude = metadataObject.getLatitudeString();
		details.longitude = metadataObject.getLongitudeString();
		details.location = metadataObject.getLoctionString();
		details.scene = metadataObject.getSceneString();
		// IPTC
		details.sourceUrl = metadataObject.getSourceUrlString();
		details.usageRights = metadataObject.getUsageRightsString();
		details.copyrightUrl = metadataObject.getCopyrightUrlString();
		details.headline = metadataObject.getHeadlineString();
		details.category = metadataObject.getCategoryString();
		details.source = metadataObject.getSourceString();
		details.instructions = metadataObject.getInstructionsString();
		details.creator = metadataObject.getCreatorString();
		details.jobTitle = metadataObject.getJobTitleString();
		details.address = metadataObject.getAddressString();
		details.city = metadataObject.getCityString();
		details.state = metadataObject.getStateString();
		details.postalCode = metadataObject.getPostalCodeString();
		details.country = metadataObject.getCountryString();
		details.phone = metadataObject.getPhoneString();
		details.email = metadataObject.getEmailString();
		details.website = metadataObject.getWebsiteString();
		return true;
	}

	

	bool MakePageBlocks::creatImageHtmlfile(std::shared_ptr<PageNode> dayNode, std::shared_ptr<PageNode> imageNode) {
		if (!isMainImage(imageNode->getName())) {
			return true;
		}
		std::string dayName = dayNode->getName();

		std::string htmlPath = m_htmlRoot;
		htmlPath += "/";
		htmlPath += dayName.substr(0, 4);
		htmlPath += '/';
		htmlPath += dayName;
		std::string htmlFilePath = htmlPath;
		htmlFilePath += '/';
		htmlFilePath += "html";
		if (SAUtils::DirExists(htmlFilePath.c_str()) == false) {
			if (SAUtils::mkDir(htmlFilePath.c_str()) == false) {
				return false;
			}
		}

		std::string imageFilePath = htmlPath;
		imageFilePath += '/';
		imageFilePath += "images";
		if (SAUtils::DirExists(imageFilePath.c_str()) == false) {
			if (SAUtils::mkDir(imageFilePath.c_str()) == false) {
				return false;
			}
		}

		ImageDatails details;
		
		std::string htmlFile = htmlFilePath;
		htmlFile += '/';
		htmlFile += imageNode->getName();
		htmlFile += ".html";
		
		std::string name = imageNode->getName();
		ImageDetailsPage imageDetailsPage;
		std::string metadataPath = m_archiveRoot;
		metadataPath += "/metadata/";
		metadataPath += dayName.substr(0, 4);
		metadataPath += '/';
		metadataPath += dayName;
		metadataPath += '/';
		metadataPath += name;
		metadataPath += ".xml";

		MetadataObject metadataObject;
		metadataObject.fromXML(metadataPath.c_str());
		//metadataObject.print();
		loadMetadata(metadataObject, details);
		// Just in case the metadate is blank
	/*
		details.sequenceID = metadataObject.getSequenceIdString();
		details.filename = metadataObject.getFilenameString();
		details.orginalName = metadataObject.getOrginalNameString();
		details.filePath = metadataObject.getFilepathString();
		details.uUID = metadataObject.getUniqueIdString();
		details.mediaType = metadataObject.getMediaTypeString();
		details.md5 = metadataObject.getMd5String();
		details.crc = metadataObject.getCrcString();
		details.fileSize = metadataObject.getFileSizeString();
		details.dateCreate = metadataObject.getDateCreateString();
		details.dateModified = metadataObject.getDateModifiedString();
		details.dateAdded = metadataObject.getAddedDateString();
	*/
		printf("%s", details.filename.c_str());

		details.image = SAUtils::getFilenameNoExt(name);
		details.image += "_2.";
		details.image += SAUtils::getExtention(name);

		std::string imagePath = imageFilePath;
		imagePath += "/";
		imagePath += details.image;

		OnIndentSizeCmd onIndentSizeCmd(imagePath.c_str());
		onIndentSizeCmd.process();

		ImageSize imageSize;
		if (imageSize.parse(onIndentSizeCmd.getOutput())) {
			details.imagewidth = imageSize.w;
			details.imageheight = imageSize.h;
		}
		metadataObject.getUniqueIdString();


		imageDetailsPage.load(&details);
		imageDetailsPage.make(htmlFile.c_str());

		return true;
	}

	bool MakePageBlocks::makeContentsHtml() {
		for (auto ii = m_imageTree->begin(); ii != m_imageTree->end(); ii++) {
			std::shared_ptr<PageNode> year = *ii;

			cout << "Year: ";  year->print();
			for (auto ii = year->begin(); ii != year->end(); ii++) {
				std::shared_ptr<PageNode> day = *ii;

				cout << "Day: "; day->print();
			}
		}
		return true;
	}

	bool TemplateReader::read(const char *datafile, HtmlTemplate &config) {


		std::string text;
		m_path = datafile;
		std::ifstream file(datafile);
		if (file.is_open() == false) {
			return false;
		}

		while (std::getline(file, text)) {
			//m_dataContainer->push_back(*(new std::string(text)));
			switch (parse(text.c_str())) {
			case Text:
				continue;
			case YearNav:
				continue;
			case DayNav:
				continue;
			case Images:
				continue;
			case Error:
				return false;
			default:
				if (m_logging) {
					//CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read token in config file \"%s\"", m_path.c_str());
				}
				return false;
			}
		}
		//std::cout << config.size() << std::endl;
		file.close();

		return true;
	}


	bool TemplateReader::read(const std::string &str, HtmlTemplate &htmlTemplate) {

		std::stringstream ss(str);
		std::string text;


		//while (ss.getline(text, 2 * 1024)) {
		while (std::getline(ss, text, '\n')){

			//m_dataContainer->push_back(*(new std::string(text)));
			switch (parse(text.c_str())) {
				
				case Text:
					continue;
				case YearNav:
					continue;
				case DayNav:
					continue;
				case Images:
					continue;
				case Error:
					return false;
				default:
					if (m_logging) {
						//CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read token in config file \"%s\"", m_path.c_str());
					}
					return false;
			}
		}


		return true;
	}

	

	TemplateReader::Token TemplateReader::parse(const char *text) {
		std::string line = text;
		if (line[0] != '<') {
			return Text;
		}
		std::size_t last = line.find(1, '>');
		std::string tag = line.substr(1, line.length() - 1);
		if (tag.compare("sia-year-nav") == 0) {
			return YearNav;
		}
		if (tag.compare("sia-day-nav") == 0) {
			return DayNav;
		}
		if (tag.compare("sia-images") == 0) {
			return Images;
		}
		return Error;
	}

	/* ======================================

	SystemHistoryMaker

	========================================*/

	SystemHistoryMaker::SystemHistoryMaker() {}
	SystemHistoryMaker::~SystemHistoryMaker() {}
	
	void contentsHeader(std::ofstream &htmlFile) {
		htmlFile << "<!DOCTYPE html>\n";
		htmlFile << "<html>\n";
		htmlFile << "<head>\n";
		htmlFile << "\t<link rel = \"stylesheet\" type = \"text/css\" href = \"../../style/history.css\">\n";
		htmlFile << "</head>\n";
		htmlFile << "<body>\n";
		htmlFile << "<h1>History Log</h1>\n";
		htmlFile << "<table> \n <tr>\n";
		htmlFile << "<th>Log</th>\n";
		htmlFile << "<th>Start</th>\n";
		htmlFile << "<th>End</th>\n";
		htmlFile << "<th>Number of items</th>\n";
		htmlFile << "</tr>\n";
		htmlFile << "<tr>\n";
	}

	void contentsFooter(std::ofstream &htmlFile) {
		htmlFile << "</tr> \n </table>\n";
		htmlFile << "</body>\n";
		htmlFile << "</html>\n";
	}

	bool SystemHistoryMaker::process() {
		FileList_Ptr filelist = SAUtils::getFiles_(m_historyPath.c_str());
		if (filelist == nullptr || filelist->empty() == false) {

			for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string filenameItem = *i;
				if (SAUtils::getExtention(filenameItem).compare("hst") == 0) {

				
					m_filelist.push_back(filenameItem);
					std::string fileIn = m_historyPath;
					fileIn += '/';
					fileIn += filenameItem;
					
					std::string fileOut = m_htmlPath;
					fileOut += "/system/history/";
					fileOut += filenameItem;
					fileOut += ".html";
					SystemHistoryFileItem item;
					
					if (process(fileIn.c_str(), fileOut.c_str(), item) == false) {
						return false;
					}
					m_fileItems.push_back(item);
				}
			}
		}

		HistoryHtmlPage historyHtmlPage;
		std::string fileOut = m_htmlPath;
		fileOut += "/html/historynav.html";
		
		
		historyHtmlPage.load(m_fileItems);
		historyHtmlPage.make(fileOut.c_str());
		return true;
	}



	bool SystemHistoryMaker::process(const char *fileIn, const char *fileOut, SystemHistoryFileItem &item) {
		std::string text;
		
		std::ifstream file(fileIn);
		if (file.is_open() == false) {
			return false;
		}
		std::ofstream htmlFile(fileOut);
		if (htmlFile.is_open() == false) {
			return false;
		}
		item.m_name = SAUtils::getFilename(fileIn);

		htmlFile << "<!DOCTYPE html>\n";
		htmlFile << "<html>\n";
		htmlFile << "<head>\n";
		htmlFile << "\t<link rel = \"stylesheet\" type = \"text/css\" href = \"../../style/history.css\">\n";
		htmlFile << "</head>\n";
		htmlFile << "<body>\n";
		htmlFile << "<h1>" << SAUtils::getFilenameNoExt(fileIn) << "</h1>\n";
		htmlFile << "<table> \n <tr>\n";
		htmlFile << "<th>Time Date</th>\n";
		htmlFile << "<th>Image</th>\n";
		htmlFile << "<th>Version</th>\n";
		htmlFile << "<th>Arction</th>\n";
		htmlFile << "<th>Path</th>\n";
		htmlFile << "<th>Comment</th>\n";
		htmlFile << "</tr>\n";
		htmlFile << "<tr>\n";
		bool first = true;
		std::string timeDate;
		item.m_numberOfItems = 0;
		while (std::getline(file, text)) {
			//m_dataContainer->push_back(*(new std::string(text)));
			CSVArgs csvArgs(':');
			csvArgs.process(text);
			timeDate = csvArgs.at(0);
			if (first == true) {
				item.m_start = timeDate;
				first = false;
			}
			
			std::string path = csvArgs.at(1);
			std::string version = csvArgs.at(2);
			std::string comment = csvArgs.at(3);
			std::string action = csvArgs.at(4);

			std::string name = path.substr(16, path.length() - 16);
			path = path.substr(0, path.length() - (name.length() + 1));
			htmlFile << "<tr>\n";
			htmlFile << "<td>" << timeDate << "</td>\n";
			htmlFile << "<td>" << name << "</td>\n";
			htmlFile << "<td>" << version << "</td>\n";
			htmlFile << "<td>" << action << "</td>\n";
			htmlFile << "<td>" << path << "</td>\n";
			htmlFile << "<td>" << comment << "</td>\n";
			htmlFile << "</tr>\n";
			item.m_numberOfItems++;
		}
		item.m_end = timeDate;
		htmlFile << "</tr> \n </table>\n";

		htmlFile << "</body>\n";
		htmlFile << "</html>\n";
		htmlFile.close();
		//std::cout << config.size() << std::endl;
		file.close();
		return true;
	}
	
	ImportJournalMaker::ImportJournalMaker() {}
	ImportJournalMaker::~ImportJournalMaker() {}

	bool ImportJournalMaker::process(const char *fileIn, const char *fileOut, JournalFileItem &item) {
		std::string text;

		std::ifstream file(fileIn);
		if (file.is_open() == false) {
			return false;
		}
		std::ofstream htmlFile(fileOut);
		if (htmlFile.is_open() == false) {
			return false;
		}
		item.m_name = SAUtils::getFilename(fileIn);

		htmlFile << "<!DOCTYPE html>\n";
		htmlFile << "<html>\n";
		htmlFile << "<head>\n";
		htmlFile << "\t<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../style/history.css\">\n";
		htmlFile << "</head>\n";
		htmlFile << "<body>\n";
		htmlFile << "<h1>" << SAUtils::getFilenameNoExt(fileIn) << "</h1>\n";
		htmlFile << "<table> \n <tr>\n";
		htmlFile << "<th>Source</th>\n";
		htmlFile << "<th>Action</th>\n";
		htmlFile << "<th>Path</th>\n";
		
		htmlFile << "</tr>\n";
		htmlFile << "<tr>\n";
		bool first = true;
		
		item.m_complete = false;
		item.m_total = 0;
		item.m_incomplete = 0;
		item.m_imported = 0;
		item.m_duplicate = 0;
		item.m_error = 0;
		while (std::getline(file, text)) {
			//D:\tmp_raw/DSC01541.JPG,Imported,2017-01-18/DSC01541.JPG
			CSVArgs csvArgs(',');
			csvArgs.process(text);
			std::string source = csvArgs.at(0);
			std::string action = csvArgs.at(1);
			std::string path;
			if (csvArgs.size() > 2) {
				path = csvArgs.at(2);
			}
			else {
				path = "Not Importd";
			}

			
			htmlFile << "<tr>\n";
			htmlFile << "<td>" << source << "</td>\n";
			htmlFile << "<td>" << action << "</td>\n";
			htmlFile << "<td>" << path << "</td>\n";
			htmlFile << "</tr>\n";
			item.m_total++;
		}
		
		if (item.m_imported == item.m_total) {
			item.m_complete = true;
		}
		
		htmlFile << "</tr> \n </table>\n";

		htmlFile << "</body>\n";
		htmlFile << "</html>\n";
		htmlFile.close();
		//std::cout << config.size() << std::endl;
		file.close();
		return true;
	}

	bool ImportJournalMaker::process() {
		std::string importJournalPath = m_journalPath;
		importJournalPath += "/import";
		FileList_Ptr filelist = SAUtils::getFiles_(importJournalPath.c_str());
		if (filelist == nullptr || filelist->empty() == false) {

			for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string filenameItem = *i;
				if (SAUtils::getExtention(filenameItem).compare("log") == 0) {


					m_filelist.push_back(filenameItem);
					std::string fileIn = importJournalPath;
					fileIn += '/';
					fileIn += filenameItem;

					std::string fileOut = m_htmlPath;
					fileOut += "/system/journal/import/";
					fileOut += filenameItem;
					fileOut += ".html";
					JournalFileItem item;
					std::string filename = SAUtils::getFilenameNoExt(filenameItem);
					
					std::string year = filename.substr(2, 4);
					std::string mon = filename.substr(4, 2);
					std::string day = filename.substr(6, 2);
					std::string id = filename.substr(11, 4);

					stringstream s;
					s << day << ':' << mon << ':' << year << '-' << id;
					item.m_date = s.str();

					if (process(fileIn.c_str(), fileOut.c_str(), item) == false) {
						return false;
					}
					m_fileItems.push_back(item);
				}
			}
		}

		JournalHtmlPage journalHtmlPage;
		std::string fileOut = m_htmlPath;
		fileOut += "/html/journalnav.html";

		journalHtmlPage.load(m_fileItems);
		journalHtmlPage.make(fileOut.c_str());
		return true;
	}

} // namespace simplearchive

/*

<html>
<head>
<style>
div.gallery {
margin: 5px;
border: 1px solid #ccc;
float: left;
width: 180px;
}

div.gallery:hover {
border: 1px solid #777;
}

div.gallery img {
width: 100%;
height: auto;
}

div.desc {
padding: 15px;
text-align: center;
}
</style>
</head>
<body>

<div class="gallery">
<a target="_blank" href="./media/DSC01277.JPG">
<img src="./media/DSC01277.JPG" alt="Fjords" width="300" height="200">
</a>
<div class="desc">Add a description of the image here</div>
</div>

<div class="gallery">
<a target="_blank" href="./media/DSC01278.JPG">
<img src="./media/DSC01278.JPG" alt="Forest" width="300" height="200">
</a>
<div class="desc">Add a description of the image here</div>
</div>

<div class="gallery">
<a target="_blank" href="./media/DSC01279.JPG">
<img src="./media/DSC01279.JPG" alt="Northern Lights" width="300" height="200">
</a>
<div class="desc">Add a description of the image here</div>
</div>

<div class="gallery">
<a target="_blank" href="./media/DSC01280.JPG">
<img src="./media/DSC01280.JPG" alt="Mountains" width="300" height="200">
</a>
<div class="desc">Add a description of the image here</div>
</div>

</body>
</html>

*/
