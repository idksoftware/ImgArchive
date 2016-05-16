Checking in of images (--checkin)
Checking in of images allows the archive to create a new version of the checked in image. This version will become the latest version and the image that was the latest will become the highest version in the version list. By default the new checked in version will be made read only and become the viewable version of the image and the version any new change will based on. In some cases the edited image may need to be checked out for more changes, i.e. if the next batch of changes may not go to plan and need to be reversed? By saving the change to a version will allow the reversal of changes back to this place holder version. This can be done by using the uncheck out command.    
The command line options are as follows:
--archive-folder	The archive this command will used to added the images.
--export-folder	The folder the images will be taken from to be placed in the archive
--current-path 	Use the current path to take the images
--recheck-out	Check the image back out after check it in. 
-–quiet	No output to the terminal
-–verbose	All information to the terminal
-–logging-level	Sets the logging level of detail that will be places in the log file.
---dry-run	Run the command but do not change the archive. 


