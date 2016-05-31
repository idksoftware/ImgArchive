#Features

Simple Image Archive (SIA) is a small set of executables that proves the following:

1. Simple in design and use.

2.Supports Bulk import and export.

3.Supports image versioning.

4.Can be extended using third party tools.

5.Uses only the base OS’s file system

6.Integrates into the Cloud infrastructure.
  
7.Can be used with almost any graphical photo editing tool.

8.Provides support for RAW and JPG image pairs and image groups.

9.Full activity reporting.

10.Provides both Image and change histories.

11.Integrated integrity and recovery systems

12.Is robust by providing a shadow archive which is not directly accessed.
 
13.Provides both on and off site mirroring of the archive.

14.Provides an XML cataloguing.

Simple in design and use

The Information Structure is a simple file based one that follows an open format giving the ability of other tools to be integrated into the archive system.

 

Supports Bulk import and export

Provides Bulk metadata entry using cascading metadata templates, these templates allow you to build information into sets of image without having to repeat common information.

Support image versioning

Prove versioning of images, so when editing images previous versions of an image can be recovered.  

Can be extended using third party tools

SIA provides the ability to execute hook scripts where an event is about to happen (pre-action) or just after the event (post-action). These scripts allow external applications to inter act with SIA as it operates.   

Uses only the base OS’s file system

                By only using the file system any damage to the archive can be easily corrected. 

Integrates into the Cloud infrastructure

Being based on the file system only, SIA can be integrated into the Cloud infrastructure. A simple means of integration would be to use the in-built mirroring system to replicate the archive in the cloud.   

Can be used with almost any graphical photo editing tool

SIA uses a check-out/check-in system for editing images. Before editing the image can be checked-out of the archive, when the editing is complete then the image can be checked back in to the archive. A version of the image before the image was edited will be saved as the previous version. Each time this check-out/check-in process takes place a new version of the image will be created.    

Provides support for RAW and JPG image pairs and image groups

                Most modern digital cameras can produce JPG, RAW and RAW/JPG paired images   

Full activity reporting

Both Image and change histories

Integrated integrity and recovery systems

Maintain data integrity of the archive by providing integrated integrity systems to guard against accidently corrupting the archive.  

 

Provides an XML cataloguing

Provides an XML cataloguing that can be extended into providing an on-line image catalogue.

 

Built-in SQLite to be used as a backend database

Built-in SQLite to be used as a backend database, However this is not required for the operation of the archive but provides support for operations such as searching the archive for which a database is ideal.  

 

Maintain data integrity of the archive by providing integrated integrity systems to guard against accidently corrupting the archive.  

Follows an open design approach in that the storage format is open giving the ability of other tools to be integrated into the archive system.

 Image information is organised in a flat file system that is directly viewable by a text editor using XML and CSV file formats

Optionally a SQLite database can also be used as an Information management system which can augment the flat file structure.

The archive is designed to be simple in design and to use. It consists of archiving core that provides the basic archiving functions but in addition, takes input and provides output from optional external components to provide a tailored archiving solution that can be extended into a complete achieving system.

The Archive takes a simple design approach that many source code repositories take, in that it uses the host operating system file system as the primary storage for the images in the repository. The archive is organised in folders that reflect the images capture date or the date added to the archive. The archive repository consists of a simple file hierarchy based on year, month, and day. These are created has required depending on date of the image.

So a typical archive will have a root or main folder and sub-folders for each year. Under the year will be folders with the date simply encoded into the folders name. By default the image file name will be the standard format the camera manufacture uses. This however can be configured to be renamed if required. 

Image information is also stored in a flat file system that is directly viewable by a text editor using XML and CSV file formats. Optionally an SQLite database can also be used as an Information management 

15.Built-in SQLite to be used as a backend database.
