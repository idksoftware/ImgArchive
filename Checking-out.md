#Checking out of images (--checkout)
Checking out an image informs the archive that an image is to be edited. The image in the archive by default is read only. On checking out an image the image will be changed to read/write. By informing the archive that it is to be edited i.e. checked out, the archive can list all the checked out images and whether any edits have been made. If an image has changed then those changes can be checked back in (checked-in) to the archive and the changes reserved.
If an image has not been checked out before editing but the image was edited then the archive may over write the changes when carrying out an integrity check on the archive. The archive will find that the image has changed but not authorised. The archive may replace the image with the image held in the shadow archive thus protecting the archive from accidental corruption.
The command line options are as follows:
--archive-folder	The archive this command will used to added the images.
--export-folder	The folder the images will be taken from to be placed in the archive
--current-path 	Use the current path to take the images
-–quiet	No output to the terminal
-–verbose	All information to the terminal
-–logging-level	Sets the logging level of detail that will be places in the log file.
---dry-run	Run the command but do not change the archive. 

