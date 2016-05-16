Un-checking out of images (--uncheckout)
This command allows you to reverse the check-out process. Any image that was checked out can be un-checked out leaving it in the same state before checking out, thus undoing any edits made to the checked out version. There is a keep option that will keep the changes made before losssing them in the uncheck out process in another image file, this will be the same name as the original image with the name “_keep” at the end if the image name.    
The command line options are as follows:
--archive-folder	The archive this command will used to added the images.
--export-folder	The folder the images will be taken from to be placed in the archive
--current-path 	Use the current path to take the images
-–quiet	No output to the terminal
-–verbose	All information to the terminal
-–logging-level	Sets the logging level of detail that will be places in the log file.
---dry-run	Run the command but do not change the archive. 
