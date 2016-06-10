#Add Images (--add)
This is how images are placed in the archive repository. The DAM term is Ingestion, or just simply getting the images into the archive.
In order to add images to the archive two important pieces of information needs to be known, the first being the source folder of the imported images. This may be a camera connected by USB or a folder with images contained within it. The second is the archive the importing images are to be imported in too. This information is obligatory. However these values can be found automaticaly using environment variables or placed in the main configuration file.

In addition, these are defaulted, meaning, If source folder cannot be found in the configuration file the archive will use the current directory, If archive folder cannot be found in the configuration file will use the platform default. for example, on a system running Windows the archive will be placed in the default Folder:

$ProgramData/IDK Software/Simple Image Archive. See platform defaults.  

The command line options are as follows:

--archive-folder	The archive this command will used to added the images.

--source-folder	The folder the images will be taken from to be placed in the archive

--current-path 	Use the current path to take the images

-–quiet	No output to the terminal

-–verbose	All information to the terminal

-–logging-level	Sets the logging level of detail that will be places in the log file.

---dry-run	Run the command but do not change the archive.
 
--refresh	Add the images to the archive. if found to be missing or changed replace the images in the archive with these ones.
 
--keep-dups=<duplicates folder>	On finding a duplicate image place the new image in the duplicates folder.

--use-today
	This option forces the archive date to be todays date. All other dates are ignored.

--use-filedate
	Use file date forces the archive data to be the image file date. In the case of a RAW/JPG pair then the RAW image 
file date will be used.

--use-date=<year.month.day>	This option forces the archive date to be the date specified in the date argument. This can be used in the case were the capture date in the camera may be incorrect and the correct date to be explicitly specified.

For example:
--use-date=2014.07.12
	

