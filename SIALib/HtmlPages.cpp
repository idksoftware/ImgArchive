#include "HtmlPages.h"
#include "SAUtils.h"

class ImageSummary {

};


HtmlPage::HtmlPage()
{
}


HtmlPage::~HtmlPage()
{
}

//void HtmlPage::content(std::ofstream & htmlFile)
//{
//}

void HtmlPage::addLink(const char *link)
{
	m_link.push_back(link);
}

void HtmlPage::insertLinks(std::ofstream & htmlFile)
{
	for (auto ii = m_link.begin(); ii != m_link.end(); ii++) {
		std::string& link = *ii;
		htmlFile << '\t' << link.c_str() << '\n';
	}
}


bool HtmlPage::make(const char *path) {
	std::ofstream htmlFile(path);
	if (htmlFile.is_open() == false) {
		return false;
	}
	htmlFile << "<!DOCTYPE html>\n";
	htmlFile << "<html>\n";
	htmlFile << "<head>\n";
	htmlFile << "	<meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\">\n";
	insertLinks(htmlFile);
	head(htmlFile);
	
	htmlFile << "</head>\n";
	htmlFile << "<body>\n";
	header(htmlFile);
	content(htmlFile);
	htmlFile << "</body>\n";
	htmlFile << "</html>\n";
	htmlFile.close();
	return true;
}



BaseHtmlPage::BaseHtmlPage()
{
}

BaseHtmlPage::~BaseHtmlPage()
{
}

void BaseHtmlPage::head(std::ofstream & htmlFile)
{
}

/*
YearHtmlPage::YearHtmlPage(const char *year) {
	m_year = year;
}
YearHtmlPage::~YearHtmlPage() {

}

void YearHtmlPage::header(std::ofstream &htmlFile) {

}

void YearHtmlPage::content(std::ofstream &htmlFile) {
	//htmlFile << "<!DOCTYPE html>\n";
	//htmlFile << "<html>\n";
	//htmlFile << "<body>\n";
	htmlFile << "<h1>" << m_year << "</h1>\n";

	for (auto ii = m_items.begin(); ii != m_items.end(); ii++) {
		//std::shared_ptr<DayNode> dayNode = *ii;
		DayNode dayNode = *ii;

		// F:\Archive\SIA Pictures\images\2017\2017-01-16\html
		//htmlFile << "<img src=\"../images/" << yearNode->getName() << "\" alt=\"" << yearNode->getName() << "\" width=\"460\" height=\"345\">\n";
		htmlFile << "<a href = \"../" << dayNode.m_name << "/" << dayNode.m_name << "/html/" << dayNode.m_name << ".html\">" << dayNode.m_name << "</a>\n";

	}


	//htmlFile << "</body>\n";
	//htmlFile << "</html>\n";

}
void YearHtmlPage::load(std::vector<DayNode> &fileItems) {
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_items.push_back(*i);
	}
}
*/

DaysHtmlPage::DaysHtmlPage(const char *year) {
	m_year = year;
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../../style/history.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../../style/gallery.css\">");
}
DaysHtmlPage::~DaysHtmlPage() {

}

void DaysHtmlPage::header(std::ofstream &htmlFile) {
	
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"../../../../html/homenav.html\">Home</a></li>\n"\
		"	<li><a  class = \"active\" href = \"../../../../html/archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"../../../../html/historynav.html\">History</a></li>\n"\
		"	<li><a href = \"../../../../html/journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"../../../../html/aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"../../../../html/menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
	

}

void DaysHtmlPage::head(std::ofstream &htmlFile) {
	htmlFile << "	<style>\n";
	htmlFile << "		figure {\n";	
	htmlFile << "	       display: inline-block; \n";
	htmlFile << "		   vertical-align: top; \n";
	htmlFile << "	       height: 280px; \n";
	htmlFile << "	       width: 260px; \n";
	htmlFile << "	       margin: 5px 5px 5px 5px; \n";
	htmlFile << "	    }\n";
	htmlFile << "		figure img { \n";
		/*border: 1px solid #666;*/
	htmlFile << "		   background-color: #FFF; \n";
	htmlFile << "          padding: 2px; \n";
	htmlFile << "	       align-content: center; \n";
	htmlFile << "	    }\n";
	htmlFile << "		figcaption { \n";
	htmlFile << "	       font: 1.1em/normal Arial, Helvetica, sans-serif; \n";
	htmlFile << "		   text-align: center; \n";
	htmlFile << "	       margin: 5px 0 0 0; \n";
	htmlFile << "	    }\n";
	htmlFile << "		</style>\n";
}

void DaysHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>" << m_year << "</h2>\n"\
		"	</div>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Log</th>\n";
	
	htmlFile << "<th>Number of items</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";

	std::unique_ptr<DayFileItem> targetItem;
	for (auto i = m_items.begin(); i != m_items.end(); i++) {
		DayFileItem item = *i;
		htmlFile << "<tr>\n";
		
		if (m_targetDate.compare(item.m_day) == 0) {
			htmlFile << "<td>" << item.m_day << "</td>\n";
			targetItem = std::make_unique<DayFileItem>(item);
		}
		else {
			htmlFile << "<td><a href=\"" << "../../" << item.m_day << "/html/" << item.m_day << ".html\">" << item.m_day << "</td>\n";
		}
		htmlFile << "<td>" << item.m_numberOfImages << "</td>\n";
		htmlFile << "</tr>\n";
	}
	htmlFile << "</tr> \n </table>\n";
	makeImageHtmlBlock(htmlFile, targetItem->m_items);
}
void DaysHtmlPage::load(std::vector<DayFileItem> &fileItems) {
	m_items.clear();
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_items.push_back(*i);
	}
}

void DaysHtmlPage::makeImageHtmlBlock(std::ofstream &htmlFile, std::vector<ImageFileItem> &items) {
	
	htmlFile << "<div class = \"content\">\n";
	htmlFile << "	<p class = \"intro\">2018/01/19 Last updated 2018/01/21</p>\n";
	htmlFile << "	<div class = \"gallery\">\n";
	for (auto ii = items.begin(); ii != items.end(); ii++) {

		ImageFileItem image = *ii;

		htmlFile << "	<figure>\n";
		htmlFile << "	<a target = \"_blank\" href = \"./" << image.m_image << ".html\">\n";
		if (image.imagewidth != 0) {
			htmlFile << "		<img src = \"../images/" << image.m_image << "\" alt = \"" << image.m_image << "\" width = \"" << image.imagewidth << "\" height = \"" << image.imageheight << "\">\n";
		}
		else {
			htmlFile << "		<img src = \"../images/" << image.m_image << "\" alt = \"" << image.m_image << "\" width = \"300\" height = \"200\">\n";
		}
		htmlFile << "	</a>\n";
		htmlFile << "	<figcaption>" << image.m_image << "</figcaption>\n";
		htmlFile << "	</figure>\n";
	}
	htmlFile << "	</div>\n";
	htmlFile << "</div>\n";
	/*
	htmlFile << "<div>\n";
	for (auto ii = items.begin(); ii != items.end(); ii++) {
		
		ImageFileItem image = *ii;
		
		htmlFile << "<div class = \"responsive\">\n";
		htmlFile << "	<div class = \"gallery\">\n";
		htmlFile << "	<a target = \"_blank\" href = \"./" << image.m_image << ".html\">\n";

		if (image.imagewidth != 0) {
			htmlFile << "		<img src = \"../images/" << image.m_image << "\" alt = \"" << image.m_image << "\" width = \"" << image.imagewidth << "\" height = \"" << image.imageheight << "\">\n";
		}
		else {
			htmlFile << "		<img src = \"../images/" << image.m_image << "\" alt = \"" << image.m_image << "\" width = \"300\" height = \"200\">\n";
		}

		//htmlFile << "		<img src = \"../images/" << image.m_image << "\" alt = \"" << image.m_image << "\" width = \"300\" height = \"200\">\n";
		htmlFile << "	</a>\n";
		htmlFile << "		<div class = \"desc\">Add a description of the image here</div>\n";
		htmlFile << "		</div>\n";
		htmlFile << "		</div>\n";
	}
	htmlFile << "</div>\n";
	*/

}


/*
<div class="responsive">
<div class="gallery">
<a target="_blank" href="./DSC01541.JPG.html">
<img src="../images/DSC01541.JPG" alt="DSC01541.JPG" width="300" height="200">
</a>
<div class="desc">Add a description of the image here</div>
</div>
</div>
*/
ImageDetailsPage::ImageDetailsPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../../style/history.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../../style/details.css\">");
}
ImageDetailsPage::~ImageDetailsPage() {

}

void ImageDetailsPage::head(std::ofstream &htmlFile) {

	htmlFile << "	<style>\n";
	htmlFile << "	body {\n";
	htmlFile << "		background: gray;\n";
	htmlFile << "	}\n";
	htmlFile << "	.detailimage {\n";
	htmlFile << "		display: block;\n";
	htmlFile << "		margin-left: auto;\n";
	htmlFile << "		margin-right: auto;\n";
	htmlFile << "		max-width:500px;\n";
	htmlFile << "	}\n";

	htmlFile << "	</style>\n";
}

void ImageDetailsPage::header(std::ofstream &htmlFile) {
	/*
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"../../../../html/homenav.html\">Home</a></li>\n"\
		"	<li><a class = \"active\" href = \"../../../../html/archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"../../../../html/historynav.html\">History</a></li>\n"\
		"	<li><a href = \"../../../../html/journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"../../../../html/aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"../../../../html/menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
	*/
}


void ImageDetailsPage::content(std::ofstream &htmlFile) {

	
	htmlFile << "<div class = \"gallery\">\n";
	htmlFile << "	<a target = \"blank\" href = \"../images/" << m_details->image << "\">\n";
	if (m_details->imagewidth != 0) {
		htmlFile << "		<img class=\"detailimage\" src = \"../images/" << m_details->image << "\" alt = \"Fjords\" width = \"" << m_details->imagewidth << "\" height = \"" << m_details->imageheight << "\">\n";
	}
	else {
		htmlFile << "		<img class=\"detailimage\" src = \"../images/" << m_details->image << "\" alt = \"Fjords\" width = \"300\" height = \"200\">\n";
	}	
	htmlFile << "	</a>\n";
	htmlFile << "	<div class = \"desc\">Canon EOS 1D X | Canon EF 600mm f / 4.0L IS II USM @840mm | 1 / 500 | f / 5.6 | ISO 1000</div>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<section class = \"tabcontainer\">\n";
	htmlFile << "\n";
	htmlFile << "	<div class = \"tab\">\n";
	htmlFile << "		<button class = \"tablinks\" onclick = \"openCity(event, \'FileInformation\')\" id = \"defaultOpen\">File</button>\n";
	htmlFile << "		<button class = \"tablinks\" onclick = \"openCity(event, \'Description\')\">Description</button>\n";
	htmlFile << "			<button class = \"tablinks\" onclick = \"openCity(event, \'Origin\')\">Origin</button>\n";
	htmlFile << "			<button class = \"tablinks\" onclick = \"openCity(event, \'Camera\')\">Camera</button>\n";
	htmlFile << "			<button class = \"tablinks\" onclick = \"openCity(event, \'Image\')\">Image</button>\n";
	htmlFile << "			<button class = \"tablinks\" onclick = \"openCity(event, \'AdvancedPhoto\')\">Advanced</button>\n";
	htmlFile << "			<button class = \"tablinks\" onclick = \"openCity(event, \'Location\')\">Location</button>\n";
	htmlFile << "			<button class = \"tablinks\" onclick = \"openCity(event, \'IPTC\')\">IPTC</button>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<div id = \"Location\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>Location</Caption>\n";
	htmlFile << "	<tr> <td>Location</td>" << m_details->location << "<td></td></tr>\n";
	htmlFile << "	<tr> <td>Scene</td> <td>" << m_details->scene << "</td></tr>\n";
	htmlFile << "	<tr> <td>Location< / td>" << m_details->location << "<td>< / td>< / tr>\n";
	htmlFile << "	<tr> <td>Scene</td> <td>" << m_details->scene << "</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<div id = \"FileInformation\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>File Information</Caption>\n";
	htmlFile << "	<tr><td>SequenceId</td><td>" << m_details->sequenceID << "</td></tr>\n";
	htmlFile << "	<tr><td>Filename</td><td>" << m_details->filename << "</td></tr>\n";
	htmlFile << "	<tr><td>OrginalName</td><td>" << m_details->orginalName << "</td></tr>\n";
	htmlFile << "	<tr><td>Filepath</td><td>" << m_details->filePath << "</td></tr>\n";
	htmlFile << "	<tr><td>Uuid</td><td>" << m_details->uUID << "</td></tr>\n";
	htmlFile << "	<tr><td>MediaType</td><td>" << m_details->mediaType << "</td></tr>\n";
	htmlFile << "	<tr><td>Md5</td><td>" << m_details->md5 << "</td></tr>\n";
	htmlFile << "	<tr><td>Crc</td><td>" << m_details->crc << "</td></tr>\n";
	htmlFile << "	<tr><td>FileSize</td><td>" << m_details->fileSize << "</td></tr>\n";
	htmlFile << "	<tr><td>DateCreate</td><td>" << m_details->dateCreate << "</td></tr>\n";
	htmlFile << "	<tr><td>DateModified</td><td>" << m_details->dateModified << "</td></tr>\n";
	htmlFile << "	<tr><td>DateAdded</td><td>" << m_details->dateAdded << "</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "	<div id = \"Description\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>Description</Caption>\n";
	htmlFile << "	<tr><td>" << m_details->description << "</td><td>                              </td></tr>\n";
	htmlFile << "	<tr><td>Rating</td><td>" << m_details->rating << "</td></tr>\n";
	htmlFile << "	<tr><td>RatingPercent</td><td>" << m_details->ratingPercent << "</td></tr>\n";
	htmlFile << "	<tr><td>Version</td><td>" << m_details->version << "</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<div id = \"Camera\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>Camera</Caption>\n";
	htmlFile << "	<tr> <td>Maker</td> <td>" << m_details->maker << "</td></tr>\n";
	htmlFile << "	<tr> <td>Model</td> <td>" << m_details->model << "</td></tr>\n";
	htmlFile << "	<tr> <td>ExposureTime</td> <td>" << m_details->exposureTime << "</td></tr>\n";
	htmlFile << "	<tr> <td>Aperture</td> <td>" << m_details->aperture << "</td></tr>\n";
	htmlFile << "	<tr> <td>IsoSpeedRating</td> <td>" << m_details->isoSpeedRating << "</td></tr>\n";
	htmlFile << "	<tr> <td>ExposureBias</td> <td>" << m_details->exposureBias << "</td></tr>\n";
	htmlFile << "	<tr> <td>ExposureProgram</td> <td>" << m_details->exposureProgram << "</td></tr>\n";
	htmlFile << "	<tr> <td>SubjectDistance</td> <td>" << m_details->subjectDistance << "</td></tr>\n";
	htmlFile << "	<tr> <td>FocalLength</td> <td>" << m_details->focalLength << "</td></tr>\n";
	htmlFile << "	<tr> <td>FocalLenghtIn35mm</td> <td>" << m_details->focalLengthIn35mm << "</td></tr>\n";
	htmlFile << "	<tr> <td>Flash</td> <td>" << m_details->flash << "</td></tr>\n";
	htmlFile << "	<tr> <td>MeteringMode</td> <td>" << m_details->meteringMode << "</td></tr>\n";
	htmlFile << "	<tr> <td>DigitalZoom</td> <td>" << m_details->digitalZoom << "</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<div id = \"Image\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>Image</Caption>\n";
	htmlFile << "	<tr> <td>Width</td> <td>" << m_details->width << "</td></tr>\n";
	htmlFile << "	<tr> <td>Height</td> <td>" << m_details->height << "</td></tr>\n";
	htmlFile << "	<tr> <td>ViewRotation</td> <td>" << m_details->viewRotation << "</td></tr>\n";
	htmlFile << "	<tr> <td>BitsPerSample</td> <td>" << m_details->bitsPersample << "</td></tr>\n";
	htmlFile << "	<tr> <td>XResolution</td> <td>" << m_details->xResolution << "</td></tr>\n";
	htmlFile << "	<tr> <td>YResolution</td> <td>" << m_details->yResolution << "</td></tr>\n";
	htmlFile << "	<tr> <td>ResolutionUnit</td> <td>" << m_details->resolutionUnit << "</td></tr>\n";
	htmlFile << "	<tr> <td>Compression</td> <td>" << m_details->compression << "</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<div id = \"AdvancedPhoto\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>Advanced photo</Caption>\n";
	htmlFile << "	<tr> <td>LightSource</td> <td>" << m_details->lightSource << "</td></tr>\n";
	htmlFile << "	<tr> <td>ColorSpace</td> <td> " << m_details->colorSpace << "</td></tr>\n";
	htmlFile << "	<tr> <td>SceneCaptureType</td> <td>" << m_details->sceneCaptureType << "</td></tr>\n";
	htmlFile << "	<tr> <td>Contrast</td> <td>" << m_details->contrast << "</td></tr>\n";
	htmlFile << "	<tr> <td>Saturation</td> <td>" << m_details->saturation << "</td></tr>\n";
	htmlFile << "	<tr> <td>Sharpness</td> <td>" << m_details->sharpness << "</td></tr>\n";
	htmlFile << "	<tr> <td>BrightnessValue</td> <td>" << m_details->brightnessValue << "</td></tr>\n";
	htmlFile << "	<tr> <td>MaxAperture</td> <td>" << m_details->maxAperture << "</td></tr>\n";
	htmlFile << "	<tr> <td>CompressedBitsPerPixel</td> <td>" << m_details->compressedBitsPerPixel << "</td></tr>\n";
	htmlFile << "	<tr> <td>FlashEnergy</td> <td>" << m_details->flashEnergy << "</td></tr>\n";
	htmlFile << "	<tr> <td>ExifVersion</td> <td>" << m_details->exifVersion << "</td></tr>\n";
	htmlFile << "	<tr> <td>Software</td> <td>" << m_details->software << "</td></tr>\n";
	htmlFile << "	<tr> <td>PhotometricInterpretation</td> <td>" << m_details->photometricInterpretation << "</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<div id = \"IPTC\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>IPTC</Caption>\n";
	htmlFile << "	<tr> <td>SourceUrl</td> <td>" << m_details->sourceUrl << "</td></tr>\n";
	htmlFile << "	<tr> <td>UsageRights</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Headline</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Category</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Source</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Instructions</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Instructions</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Creator</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>JobTitle</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Address</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>City</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>State</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>PostalCode</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Country</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Phone</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Email</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	<tr> <td>Website</td> <td>" << m_details->description << "</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "\n";
	htmlFile << "	<div id = \"Origin\" class = \"tabcontent\">\n";
	htmlFile << "	<table class = \"image_attr\">\n";
	htmlFile << "	<Caption>Origin</Caption>\n";
	htmlFile << "	<tr> <td>CaptureDate</td> <td>2018.01.19.12.19.45</td></tr>\n";
	htmlFile << "	</table>\n";
	htmlFile << "	</div>\n";
	htmlFile << "	</section>\n";
	htmlFile << "\n";
	htmlFile << "\n";

	htmlFile << "	<script>\n";


	htmlFile << "	function openCity(evt, cityName) {\n";
	htmlFile << "		var i, tabcontent, tablinks;\n";
	htmlFile << "		tabcontent = document.getElementsByClassName(\"tabcontent\");\n";
	htmlFile << "		for (i = 0; i < tabcontent.length; i++) {\n";
	htmlFile << "			tabcontent[i].style.display = \"none\";\n";
	htmlFile << "		}\n";
	htmlFile << "		tablinks = document.getElementsByClassName(\"tablinks\");\n";
	htmlFile << "		for (i = 0; i < tablinks.length; i++) {\n";
	htmlFile << "			tablinks[i].className = tablinks[i].className.replace(\" active\", \"\");\n";
	htmlFile << "		}\n";
	htmlFile << "		document.getElementById(cityName).style.display = \"block\";\n";
	htmlFile << "		evt.currentTarget.className += \" active\";\n";
	htmlFile << "	}\n";

	htmlFile << "	document.getElementById(\"defaultOpen\").click();\n";
	htmlFile << "	</script>\n";

}

void ImageDetailsPage::load(ImageDatails *details) {
	m_details = details;
}


AboutHtmlPage::AboutHtmlPage()
{
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
}

AboutHtmlPage::~AboutHtmlPage()
{
}

void AboutHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a class = \"active\" href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void AboutHtmlPage::content(std::ofstream &htmlFile) {
	/*
	htmlFile << "<!DOCTYPE html>\n"\
		"	<html>\n"\
		"	<head>\n"\
		"	<meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\">\n"\
		"	<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">\n"\
		"	</head>\n"\
		"	<body>\n"\
		"	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li class = \"right\"><a class = \"active\" href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n"\
		"	<div style = \"padding:0 16px;\">\n"\
		"	<h2>About page</h2>\n"\
		"</div>\n"\
		"</body>\n"\
		"</html>\n";
*/
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>About page</h2>\n"\
		"</div>\n";
}

HomeHtmlPage::HomeHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
}

HomeHtmlPage::~HomeHtmlPage() {
}

void HomeHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Home page</h2>\n"\
		"	</div>\n";
}

void HomeHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a  class = \"active\" href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}




ArchiveHtmlPage::ArchiveHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/history.css\">");
}

ArchiveHtmlPage::~ArchiveHtmlPage() {
}

void ArchiveHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a class = \"active\" href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void ArchiveHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Archive page</h2>\n"\
		"	</div>\n";

	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Year</th>\n";
	
	htmlFile << "<th>Number of days</th>\n";
	htmlFile << "<th>Number of images</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	for (auto i = m_fileItems.begin(); i != m_fileItems.end(); i++) {
		YearFileItem item = *i;
		htmlFile << "<tr>\n";
		htmlFile << "<td><a href=\"" << "../images/" << item.m_year << '/' << item.m_firstDay << "/html/" << item.m_firstDay << ".html\">" << item.m_year << "</td>\n";
		htmlFile << "<td>" << item.m_numberOfDays << "</td>\n";
		htmlFile << "<td>" << item.m_numberOfImages << "</td>\n";
		htmlFile << "</tr>\n";
	}
	htmlFile << "</tr> \n </table>\n";
}

void ArchiveHtmlPage::load(std::vector<YearFileItem>& fileItems)
{
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_fileItems.push_back(*i);
	}
}



JournalHtmlPage::JournalHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/history.css\">");
}

JournalHtmlPage::~JournalHtmlPage() {
}

void JournalHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a class = \"active\" href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void JournalHtmlPage::load(std::vector<JournalFileItem> &fileItems) {
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_fileItems.push_back(*i);
	}
}
/*
std::string m_date;
std::string m_name;
bool m_complete;
int m_total;
int m_incomplete;
int m_imported;
int m_duplicate;
int m_error;
*/
void JournalHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Import journal page</h2>\n"\
		"	</div>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Date</th>\n";
	htmlFile << "<th>Name/ID</th>\n";
	htmlFile << "<th>Complete</th>\n";
	htmlFile << "<th>Total</th>\n";
	htmlFile << "<th>Incomplete</th>\n";
	htmlFile << "<th>Imported</th>\n";
	htmlFile << "<th>Duplicate</th>\n";
	htmlFile << "<th>Error</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	for (auto i = m_fileItems.begin(); i != m_fileItems.end(); i++) {
		JournalFileItem item = *i;
		htmlFile << "<tr>\n";
		htmlFile << "<td><a href=\"" << "../system/journal/import/" << item.m_name << ".html\">" << SAUtils::getFilenameNoExt(item.m_name) << "</td>\n";
		htmlFile << "<td>" << item.m_date << "</td>\n";
		htmlFile << "<td>" << item.m_name << "</td>\n";
		htmlFile << "<td>" << item.m_complete << "</td>\n";
		htmlFile << "<td>" << item.m_total << "</td>\n";
		htmlFile << "<td>" << item.m_incomplete << "</td>\n";
		htmlFile << "<td>" << item.m_imported << "</td>\n";
		htmlFile << "<td>" << item.m_duplicate << "</td>\n";
		htmlFile << "<td>" << item.m_error << "</td>\n";
		htmlFile << "</tr>\n";
	}
	htmlFile << "</tr> \n </table>\n";
}

HistoryHtmlPage::HistoryHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/history.css\">");
}

HistoryHtmlPage::~HistoryHtmlPage() {
}

void HistoryHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a class = \"active\" href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void HistoryHtmlPage::load(std::vector<SystemHistoryFileItem> &fileItems) {
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_fileItems.push_back(*i);
	}
}

void HistoryHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>History page</h2>\n"\
		"	</div>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Log</th>\n";
	htmlFile << "<th>Start</th>\n";
	htmlFile << "<th>End</th>\n";
	htmlFile << "<th>Number of items</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	for (auto i = m_fileItems.begin(); i != m_fileItems.end(); i++) {
		SystemHistoryFileItem item = *i;
		htmlFile << "<tr>\n";
		htmlFile << "<td><a href=\"" << "../system/history/" << item.m_name << ".html\">" << SAUtils::getFilenameNoExt(item.m_name) << "</td>\n";
		htmlFile << "<td>" << item.m_start << "</td>\n";
		htmlFile << "<td>" << item.m_end << "</td>\n";
		htmlFile << "<td>" << item.m_numberOfItems << "</td>\n";
		htmlFile << "</tr>\n";
	}
	htmlFile << "</tr> \n </table>\n";
}



MenuHtmlPage::MenuHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
}

MenuHtmlPage::~MenuHtmlPage() {
}

void MenuHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a class = \"active\" href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void MenuHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Menu page</h2>\n"\
		"	</div>\n";
}

