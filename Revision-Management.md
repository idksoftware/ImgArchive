Revision Management (--revision)
This command manages the revisions of an image created under the check-out/check-in process. This command allows you to carry out the following:
1.	Display the total number of versions of a particular image in the archive.
2.	Retrieve a particular version of a particular image into a specified folder.
3.	Retrieve all versions of a particular image into a specified folder.
These sub-commands are detailed in the following section.
Display the total number of versions of a particular image in the archive.
Given the address of the image in the form <year>-<moth>-<day>/<image file name> this sub-command prints the number of versions for this image in the archive.
sia –revision  --number-of --address =<year>-<moth>-<day>/<image file name> 
Example:
sia –revision  2013-11-03/DSC0023.jpg
Output from sub-command:
Versions: 11
In this example, eleven versions of this image was found.
Retrieve a particular version of a particular image into a specified folder.
Given the address of the image in the form <year>-<moth>-<day>/<image file name> this sub-command retrieves a version of an image identified by the address passed.
sia –revision   --number=1 –address=<year>-<moth>-<day>/<image file name> 
Example:
sia –revision  --number=1 –address=2013-11-03/DSC0023.jpg
Output from sub-command:
Versions: 11
In this example, eleven versions of this image was found.

Retrieve all versions of a particular image into a specified folder.
Given the address of the image in the form <year>-<moth>-<day>/<image file name> this sub-command retrieves a version of an image identified by the address passed.
sia –revision   --number=1 –address=<year>-<moth>-<day>/<image file name> 
Example:
sia –revision  --all –address=2013-11-03/DSC0023.jpg
Output from sub-command:
Versions: 11
In this example, eleven versions of this image was found.



int revisions(const char *filepath);

	bool getImageVersion(const char *imagePath,const char *targetPath, int idx);

	bool getAllImageVersions(const char *imagePath,const char *targetPath);
      /// Called to found if an error accured.

	std::string getHistory(const char *filepath);
