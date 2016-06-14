#The Repository

All major computer systems have some form of mass storage, this is normally one or more hard drives. These drives can now have 5T bytes or more capacity, coupled with the fact that digital cameras can shot a large number of images in one photo shoot means having some formal method of storing images on the disk much more important. In addition have an automated method even more important as there is less chance of the images being stored in the wrong place and possibly losing them.

##Image Storage Structure
The storage strategy must be simple but robust independently of content. Using the image content is a popular way of organising an archive. However this in the majority of cases renders the folder management unwieldy. For example, using this approach do you organise the folders by keyword, client, project etc. how is handling images that belong to a number of groups which will translate to folders. The image will end up being duplicated in a lot of folders. This can also result in a lot of work managing which folders belong at the time the image are added to the archive. Addition, backing up will be needlessly complicated. Enter the partition structure.    

##The partition structure
Some also call this the bucket system, as each folder contains a bucket of images of unknown content. However data partitions are a common way of organising data within a database. This is done for manageability, performance and possibly availability reasons. This is used in this case for the same reasons, i.e. partitioning of data into manageable units, in this case images. The manageability, performance issue in this case is that locating an image file in large numbers other image files will be time consuming and having large numbers of image in one folder is more difficult to manage.

These partitions are organised by a primary index. SIA uses the image capture date as a primary index for storing images in the archive. The archive uses folders to partition the stored images into the manageable subsets. These subsets are partitioned into years. All images using a date that fall in a particular year will be stored under folder of that year. Within a year folder the year is partitioned further into days, as there are 365 maximum days in the year then this is a manageable number of partitions within a year’s main partition. So for any image with an archive date, can be found a year / day folder in which to place it. This also is a useful scheme in that most photographers will have a rough idea when an image was taken, so to then find images in the archive folders is not too difficult.
Flickr uses a similar system and calls it a camera roll.
 
The folders are named in a simple but consistent way. The year folder is simply the year, i.e. “2014”. Then the day folder is created using the year, month and day in the form <year>_<month>_<day>, for example: “2014_08_12” will be 12 August 2014. Having the sequence year, month and day allows the folders to be automatically sorted in date order when viewed using a file browser such as File Explorer on windows or finder on the MAC. 

By using the image capture date as the primary index to which to store image also means that the as the archive grows the date index will normally in most cases grow in an orderly sequence.
The location of images can be found quickly and unambiguously. 

##Sequence Identifiers
In addition to the partitioning of the images into the day captured, when the image is added to the archive a sequential index identifier is added to it. Each and every image has a sequential index identifier associated with it, and as it is given at the time it was added to the archive which will mean it will be is time order as to the time added to the archive, not the capture date.  As it is a sequence number it will not be associated with the time itself. For example an image added 2015/02/12 may have an index of 983423, two days later 2015/02/14 the next image is added with the index of 983424. It does however allow the management of backups much easier. See Backing up the Archive. 

Using this sequence number was an alternate to using the capture date as the primary index for the partitions. However this number is completely arbitrary. Using the capture date is much more intuitive to the human reader. In addition in most cases this data/time will be encoded into the EXIF data in the image, the sequence will not. 

##Workspace and shadow storage areas
The archive contains two copies of the image when the image is first added to the archive one copy will be in the workspace storage area and the other will be in the shadow area. Users will only normal be able to access the workspace storage area.
###Editing an image
The image in this area initially will be read only. To edit it, it will normally be checked out for editing. This informs the archive that it may be edited. If the edits need to be undone then it can be un-checked out. This results in the copy in the shadow area being copied over the one in the primary storage area thus undoing the changes. If on the other hand the edits need to be made permanent then the image needs to be checked-in in this case the original image in the shadow storage area will be renamed with a version number encoded into the filename and a copy of the edited image will be copied into the shadow storage area. It this way versions the original are built up giving the ability to return to previous images.
      
###The Archive Date
The SIA as explanted above the archive uses a date to place images. The most significant dates are:
1.	The date the image was captured in the camera
2.	The file date on the image file
3.	The archived date
####The date the image was captured in the camera
Out of the three dates the capture date is probably the most useful as it places the image in the archive at the time it was taken. This date may not be found as when the images file is taken from the camera a new create date in the image of the date the image was moved from the camera to the computer storing the image. However where is a way of recovering the capture date but this will only be present in some images that have EXIF data? EXIF data is used by the camera manufactures to store information about an image in the image. One of the values stored is the capture date. By reading this date from the image file then the capture date can be recovered. This relies on the EXIF data being present if not then the capture date is lost.   
####The file date on the image file
The file image creation date will always be present as it is a property of the image file that always must exist. This date may be a large period away from the capture date as this is the date you transfer your captured image from the camera. If you do this frequently then the then the date will not be too far from the capture date.
####The archived date
The date on which the image was place in the archive is the last significant date. Is date can be even further away in the future from the capture date? However if you add images into the archive at the same time as transferring from the camera then again you will not be too far out  from the capture time.
####Default archive date
By default SIA will try to find the earliest date or a date that is most likely to be the capture date. This is carried out by the following.
1.	Read EXIF information from all images in the set (in the case of RAW and JPG pairs).
2.	Find the create date from all image files.
3.	Use todays date.   
####Reading EXIF information
SIA will read EXIF information in two ways: Using an Internal EXIF reader and (if configured) using an external reader. SIA will try both methods; the internal reader will be tried first then the external reader.  All images in the set will be read for any EXIF and the results stored. This EXIF data will also be used to populate the metadata information for each image and set metadata. This is metadata that is shared across all images in the set and crucially the shared archive date.  If one of the images is missing data, by default the information will be filled in from other images if available. By default in an image set with both a RAW and JPG images the information in the RAW will be the primary information source. However the JPG will most likely contain EXIF data.  
####Finding the create date from an image pair
If the capture date cannot be found then the create date will need to be used. however in an RAW/JPG image pair, the RAW file create-time will be used if the capture date cannot be found in the EXIF data in the JPG image in the pair,
###Use todays date.   
As a fall back and no other date can be found todays date will be used. This is normally a last resort. 
##Date checks
SIA will do a sanity test on the dates. if the dates do not make sense  then the image will not be added to the archive and an error raised to flag there is a possible error in the dates. The Capture dates, the image file dates and todays date must be each be in the future i.e. the capture date must be the earliest date, the image file must be the same or later that the capture date and finally todays date must be the same or later than the image file date.
Anomalies in the dates can be due to the time and date being incorrectly set on the camera or archive computer. The default archive date can be overridden in these cases. One common problem is that the camera date may not be set from the manufacture’s default.      
###Specifying and controlling the archive date.
SIA has a number of options to specify and control the archive date. These can be used to override the default archive date selection. These are as follows:
Use date options:
These are used to override the archive date being selected. These are used if the default dates are somehow incorrect and a new date needs to be used. These options are as follows:
1.	Use todays date
2.	Use file date
3.	Use date  
Use todays date
This option forces the archive date to be todays date. All other dates are ignored.
--use_date_today

Use file date
Use file date forces the archive data to be the image file date. In the case of a RAW/JPG pair then the RAW image file date will be used.
--use_file_date
Use date  
This option forces the archive date to be the date specified in the date argument. This can be used in the case were the capture date in the camera may be incorrect and the correct date to be explicitly specified.
--use_date=<year.month.day>
For example:
--use_date=2014.07.12
The above will set the archive date to the 12 day of July 2014.  
Image Extensions
In the SIA configuration folder is the Image extension file. This file contains the file extensions that SIA uses to identify the media encoding type and determine which category type an image fits. There are three categories;
1.	Displayable images (Picture types). 
2.	Raw image (RAW Types).
3.	Images that may be displayable, but will not be raw images. These may have been generated by a camera or some third party image editor. Most image editors will save the edited image with a known extension. For example Photoshop uses the extension *.psd” (Image types).
 Some images may fit in more than one category. The most often images the a camera generates are “JPG” files these will normally have the extension “.jpg” the other most often generated files that a camera generates are RAW images these are more tricky. Each camera manufacturer uses their own defined file extension to denote RAW image, in some cases more than one extension to denote RAW images For example Nikon uses “*.NEF” to denote the file is a raw image file, Canon on the other hand use “*CRF” to denote a raw image file.
The image extensions file is split into three sections, one for each category type. An example file is shown below.
<!-- Raw image (RAW Types)-- > 
Dng=raw,Adobe Digital Negative
NEF=raw,Nikon RAW
CRF=raw,Canon RAW

<!-- Images that may be displayable, but will not be raw images. (Image types) -- > 
Bmp=img,Bitmap
Jpg=img,Joint Photograohic Experts Group

<!-- Displayable images (Picture types) -- > 
Gif=pic,Graphics Interchange Format
Jpg=img,Joint Photograohic Experts Group

As new extensions are created by camera manufactures and photo editor authors, then this file may be update to reflect the new changes. 
Managed and un-managed images
SIA provides managed and un-manage versions of each image in the archive. Images for each type are stored to different separate areas. The managed image is stored within the archive. the un-managed images are stored in an area that is easily accessible to the user.   
When a new image is added to the archive it becomes managed.  SIA will assign a unique number to the image and tracked within the archive and is normally not viewable by anyone. At the same time the image is added a viewable/editable version is also added in the un-managed area. The user of the archive is able to view and edit the un-managed images even delete them with no impact on the archive. This un-managed images can be refreshed from the managed images at any time. The managed version on the other hand should never be accessed by the user and must never be modified. If an un-managed image is edited and the changes need to be archived, then the images can be checked-in to the archive. When the image is checked-in a copy of the unmanaged image is placed in the managed area of the archive with a version number as part of the file name. This will prevent the original version of the image not to be over written and identify the new version. A unique number is then added and the new image is tracked within the archive.
Backing up and Mirroring
When a managed image is added to the archive all mirrors automatically updated with the new image. This ensures that more than one copy of the managed image exists in almost real-time.
When backing-up to off-line media such as Blu-Ray as each unique image is added to the archive   

Database support for metadata in SIA
Once the metadata for each image has been captured some means of storing and accessing that data needs to be found. The metadata lends itself to a number of methods for both accessing and the storage of the metadata information.
SIA supports three types of storage:
1.	XML files
2.	CSV files
3.	SQLite database
Each has advantages and disadvantages. Using the three methods can help to mitigate some of the disadvantages and support other storage methods and access methods. Each will be described in detail in the following sections.
XML files
XML stands for Extensible Mark-up Language (XML). This is a mark-up language that defines a set of rules for encoding documents in a format that is both human readable and machine readable i.e. readable by a computer. The standard for XML is defined in the XML 1.0 Specification produced by the World Wide Web Consortium (W3C). This format can be easily converted into HTML web pages. This conversion is common, so common that tools such as XSL parses have been developed to make the process easier.
There disadvantage is that they are slow to search as each file will need to be opened, read and closed. An archive will have a large number of files to read thus a time consuming process compared to accessing a database to carry out a similar search.  
The XML Database
This essentially is a collection of XML files placed in folders in a consistent way. This allows the XML files to be accessed effectively. Any software tool using the XML files will be able to do so by following the access rules.
These rules are as follows:
Each image has a XML file with its metadata. This will always contain the identification information for the image. The file name for the XML file is the full filename with extension plus “.xml”. For example: the image file “DSC_1234.jpg” will have a xml called “DSC_1234.jpg.xml”. The image set will also have an XML metadata file, the will a file name of the main image filename without the extension. For example: the image “DSC_1234” will have a xml file called “DSC_1234.xml”.
These xml file will be stored in the day folder in which the image resides. Each day folder will contain a folder called “.metadata” the dot at the start makes the folder hidden. Under this folder will be another folder called xml under this folder the xml files will be stored.

CSV files
CSV file are a comma separated value (CSV) files. Also the format may be called character separated values, because the separator character does not have to be a comma, this is the case of SIA. The files stored tabular date in plain text form. CSV file can be imported into both spread sheet applications such as Excel and databases such as Access. This includes importantly SQLite; CSV files can be used to provide a backup for SQLite.
The CSV Database
 The CSV file database, like the XML database is a collection of plain files. In this case CVS Formatted files. Unlike XML there will be one file per set of Metadata attributes for the day set of images. Each image attributes will be contained in one line in the CSV file. The sets of attributes will be connected by a sequence number which is the first field in each row. If a set of attributes are not available then they an entry with the sequence identification is added but the value can be blank. So in each day folder will contain a CSV folder under the metadata directory. This CSV folder will contain the following data sets:  
1.	File Properties
2.	Asset Properties
3.	Camera Information
4.	Copyright Properties
5.	GPS Properties
6.	Media Properties
Each data set will be contained in one CSV file. The data sets contained in the CSV files will reflect the same data held in the SQLite tables. 

SQLite database
SQLite is an open source database that is used in both large and small systems. Adobe uses it for Lightroom. Lightroom is Adobe’s archiving application. Another user of SQLite is the programing language Python. A SQLite database can be easily access using python. As this database is popular and completely free unlike a number of other databases it has a large following.  The main advantage of a database is that the data it holds can be searched and sorted much quicker than a flat file database. Data can be queried using SQL and applications can be made to use the data quickly. The disadvantages is that the database system are more complex to setup. Damage to a database affects virtually all the systems using it.
Sequence Identifiers
 Each image in the archive is uniquely identified by a sequence number. This is then used to cross reference images within the databases. The databases generate this number to two ways, the SQLite database will generate this number as a unique number primary number key in the Asset Properties table. All other tables in the database will then contain this number as their primary key. Each image in the database must be referenced in the Asset Properties table all other tables it can be optional. The Asset Properties table will contain the full path to the image in the archive and an index that performs the reverse in that given an image path it will return the sequence number.
 In the case of the SQLite database, the database can generate this unique sequence number and carry out the indexing into the other tables and maintain a link to the actual image in the archive. Flat File database such as the XML and CVS databases cannot do this directly. The reason being is  that that there is a set of CSV files per day and the sequence numbers are generated at the time the image is placed in the archive not the date the image was take. The sequence numbers are not guaranteed to be in any order.  To solve this problem SIA maintains a file based sequence number lookup. Given a sequence number the lookup will return the full archive path. To carry out the reverse, you will start with the archive so the folder the correct CSV is known the Asset Properties CSV file will be ion image file name order so finding the sequence file number is trivial.
Archive integrity
One main function of an image archive is to safe guard the images within it. The archive can be damaged either intentionally or unintentionally at any time. If damage is done to the archive, the first thing for the archive to do is to inform you, the user, as soon as possible that the damage has taken place. The next thing is to inform you what damage has been done, then lastly help you fix the damage. 
SIA has mechanisms to monitor the integrity of the archive by recording the times that images are modified. In addition maintains a file map of the archive with both a CRC and MD5 checksums of each file in the archive. If the file map of the archive does not match the contents of the archive then these differences can be listed. Sometime these differences are relatively harmless, such as an image being modified without being marked as checked-out; on the other hand whole years’ worth of images may be missing. The file map will highlight this change. From the users point of view missing a year may not be seen until images from that year are needed, along period time may have passed before the damage may be apparent.  Once the damage is identified a file list of damaged or missing files can be generated and the archive can be repaired from an archive mirror by copying the file back into the archive. A full integrity check can then be made of archive to verify that the repair was successful.  
Hook scripts
A hook script is a program triggered by an archive repository event , such as an image being about to be processed  to be put into the  archive. This is for example a point where if the image say a RAW  type then a picture type may be generated so both can be archived as a RAW/Picture pair.

Backups
Backups are an important part of the workflow in that they help to under pin the integrity of the archive. Depending on the value placed on the images in the archive, ideally at least three backups copies of the photos, so if one backup is lost, two backups remain. This comes from safety critical system where all critical system is in triplicate. If one system fails two are still operational. In the case of an aircraft, all critical system is in sets of three, if one system is lost two remain. The aircraft is not relying on one system. One system is in use and one system is a backup. It the aircraft flies without the backup if the system in use fails then this may lead to disaster. The same is true for the integrity of the archive. Another consideration is the location of the backups and time between backups.
Backup location
The location of the backups is a consideration when creating a backup system. The metaphor “having all ones eggs in one basket” is a good one.  If the entire backup sets and the computer containing the archive is lost say by a fire or stolen is in one location. Having another set of backups is a prudent strategy.  This normal case for backups and so are characterised into to two sets:
1.	On-site - Backups are normally in the same location of the computer containing the archive.
2.	Off-site - Backups are normally not in the same location of the computer containing the archive.
On-site Backups 
On-site backups (direct backups) are normally quickly available to use in the case of a failure in the integrity of the archive. These are the first line of backups. These backups are normally hard disks that mirror the main archive. These are not part of a RAID system but run alongside the RAID. A RAID system can run without any support from SIA and is part of the archive computers hard disk system. Using a RAID system is useful in insuring the integrity of the primary archive hard drive, however it cannot guard against accidental data corruption.
The direct mirror disk can be a second hard disk,  USB disk or a network disk, it is does need to be quite fast and available at all times the main archive is available.  
Off-site Backups
Off-site Backups (indirect backups) are normally not as quickly available.  If the entire backup and the computer containing the archive is lost. Then the second line of backups will remain to re-build the archive. This is easily achievable using a cloud backup system. This requires the internet to be available. The data it transmitted to a remote hard disk. As the internet is much slower than a local hard drive the data is sent at a slower rate than a direct mirror disk and the data is sent indirectly to the remote drive. 
Off-line backups
These are backups that are not available directly. The archive data is contained on Off-line such as Tapes, DVDs or Blu-ray. The data will need to be copied back on to the on-line system in order to use. This is a third line of backups and slow to recover the data but is used for archiving and the last stop disaster recovery. However this form of backing up the archive may be the most reliable in that the media can be the most robust. For example Blu-ray disks can have a data retention time of tens of years.
SIA backup support
SIA supports both direct and in-direct backups as part of the normal workflow of placing an image in the archive. As each image is added to the main archive it is also copied into the direct backup and indirect targets. Off-line media is handled separately
SIA Online backups
SIA provides the facilities for online backup thought the use of data mirrors. A data mirror provides a complete copy of a set of data. Then as the target set of data changes so the changes are reflected in the mirrored data in near real time. The data set changes are synchronised with the mirror.
The Mirror provided by SIA is not a true mirror as a true data mirror is the replication of logical disk volumes onto separate physical hard disks in real time to ensure continuous availability of the data using physical hardware. It is most commonly used in RAID 1 where two physical hard disks mirror each other. The user can be unaware that the data is being mirrored and in the case of a disk failure, the failed disk is replaced with a new one and the mirror is re-established by the good disk copying all the data to the new disk until all the data is back in sync.    
The type of mirror provided by SIA is known as file shadowing. This type of mirroring operates at the file level and needs no additional hardware. As the files in the target data set changes the changes are reflected in the file mirror. The target date in this case is the Image Archive and the data set is the entire archive. The archive can be then mirrored on another disk so if the main disk becomes faulty the archive is safely contained in the file mirror. Once the faulty disk is repaired or another location is found for the primary archive then the archive can be restored from the mirror by simply copying the files back into position relative to the root archive folder.
Recovery point object
Recovery point object or RPO is defined as the maximum tolerable period in which data might be lost from an IT service due to a major incident. In the context of the archive it is the time taken to re-sync the mirrors. 
Synchronous and Asynchronous Mirrors
This refers to how the data is written to the mirror. A Synchronous mirror is where as each file is copied into the Archive it is also written to the mirror. This reduces the RPO to the time taken to write the file copy to the mirror. The only better RPO time would be if the primary Archive is located on a mirrored disk where the RPO will be almost zero.  The down side is that the location of the mirror must have fast write times and always available. It the data cannot be written then the archive will stall i.e. the current archive operation will not complete until the mirror is able to write the file. This means a second hard drive is probably the best option for this type of mirror.
An Asynchronous mirror is updated periodically. The period may be within seconds; however the process that copies the files will not be the process controlling the archive. If the copy process stalls then the archive is unaffected. When the copy process is resumed then the process will needs to catch up the copying operation until the files are back in sync.
Direct and Indirect Mirrors       
This refers to how the data is written. A direct mirror writes the data directly to the mirror. An indirect mirror will have the data written to a staging folder so the data can be read asynchronously to be then written to the mirror. This method is like a print queue where the print jobs are queued for printing on the printer. The printer is a show device so the queue is there so users can print out a document then carries on working while the printer is printing. Once the print job completes the document can be delivered to the user. SIA supports internet and intranet based mirrors, the internet is not as fast as a hard drive so like the printer files written across the internet need to be queued in a staging area. An Synchronous write from the archive can be used to write the file to be mirrored to the staging area as this is a fast operation. However the process reading it from the staging area to the remote mirror can take as much time as needed. The down side is that the remote mirror cannot be synchronised as quickly as an On-Site local mirror.   
SIA backup support
SIA supports both direct and in-direct backups as part of the normal workflow of placing an image in the archive. As each image is added to the main archive it is also copied into the direct backup and indirect targets. Off-line media is handled separately. 
Direct backups
SIA only needs the destination folder on the hard drive of where the mirror is to be placed. Whenever a new image is added SIA automatically copied the image and associated files to the direct mirrors. This increases the time to archive an image but guarantees the mirror is up-to-date.
Indirect backups
Indirect mirrors are slightly different; each indirect mirror is associated with a staging folder. This folder acts as a temporary store for the images. The images are copied by SIA automatically to a staging folder along with information as to the location in the indirect mirror of where the image is located.  A second process then transfers the images and associated files indirectly to the indirect mirror using the location information. This acts as a buffer between the copying from the archive and potently moving over the internet at a much slow speed. In Addison if the internet is not available then the indirect updates can be resumed when the internet 
Off-line backups
An off-line backup is here the media containing the backup data is not immediately available. This normally refers to tape backups but can also refer to CD, DVD or Blu-ray and additionally USB hard drives.
Backup spanning
The backup media may or may not span the entire archive. for example a large USB  hard drive may easily contain the complete backup of the archive, however a DVD or even a Blu-ray probably not contain the complete backup necessitating a set of media to span the entire backup data set. This set of media is generally called a Volume Set. Each an item of media such as a DVD is called a Volume and labelled sequentially from 1 to the maximum required to contain the backup. SIA provides a method generating a volume set using temporary hard disk space to generating the volumes to then be copied on the target media.
For example, if you have an archive covering 340 Giga Bytes of storage. To create a Blu-ray set of archival disks will require 14 25 Gb Blu-ray Disks or 7 50 Gb Blu-ray Disks.  A hard Disk with at least 340 Gigi bytes is requited to create a staging area for the Volume set.
From the command line enter the following:
siasdmin –backup –all –archive=”c::\images” –target=”c:\temp\backups”

Data backed-up
In order to reliably restore an archive from a backup all key information needs to be backed-up however some data need not be backed up as it is regenerated by the archive as part of the restoration process.
Data needing to be backed up:
1.	All Images including the version set.
2.	All metadata.
3.	All Image change history.
Optional data such as summary logs can also be backed-up this will include the following:
1.	Summary and details session logs
2.	Application logs.
System Data, This is data that the system needs to operate is the same manner as before the system was backed-up then the backed-up data applied in the restoration process.
Indexed back-ups
All the images in the archive are indexed using an incrementing sequence Number. The first image has an index of 0 (zero) the next is 1 (one) etc. These will therefore increment in ascending order over time. This enables backups to backup    
Backing-up using the  

Verification
When the files are copied to the mirror SIA can verify that the file was successfully copied to the mirror and that the CRC checksums on both files agree. However, if the coping of files is carried out by a second process then that process must also carry out the verification operation as it has control of the mirroring activity. To help these processes, SIA provides the size, modification date and the CRC as part of the data passed in the staging area.
Backup Configuration   
This section details how the backup mirror options can be tailored to your requirements. These options are contained in a configuration file located in the SIA configuration folder. This file is named “mirror.dat”
The configuration file can contain a number of mirrors each of which is identified by a name. This name is then followed by a list of comer separated options. These are as follows:
1.	Direct or Indirect (direct|indirect)
This will switch between direct or indirect copying of files.
2.	The path to the mirror or staging area
This will be where the root folder of the mirrored archive or the root staging folder.
3.	Mirror type (mirror|stage)
This will switch between a direct mirror of the archive or a staged copy of the archive.
4.	Verify (on|off)
Whether the files passed to the staging area or mirror are verified that there are copied correctly without error. 
An example mirror will look like the following:
Secondary=direct,/backup/archive001,mirror,true
In this case:
 The name of the mirror is Secondary
The mode of mirroring is direct writes
The path is to a backup disk called backup and the root of the archive is Archive001
The type of mirror is a standard file mirror
Verification is on.
Mirror commands
In order to manage the Mirrors a number of commands are available to carry out action on mirrors, these are as follows:
Initialise Mirror
This command is used to initialise a mirror. When a mirror is defined in the configuration it will be empty. In order to get the Mirror in sync with the Master archive then the Mirror needs to be initialised. This command provides that function.
sia –mirror_initialise=<mirror name>
Verify Mirror
This command is used to verify that a mirror is in sync with the master archive
CheckDisk
History    
Versioning	
One common task to be carried out in digital photography is to edit images. This may be to remove spots on the image or may be to enhance it in some way. There may in the future a need to go back to the original or a version (revision) of an image. This will likely be the case if you have watermarked images in order to safeguard them then putting them up on the web.  In the future you will probably need to go back to the un-watermarked version in order to view these images without the watermark. This is where a version (or revision) control system comes in.
A Version control system is a system that tracks incremental versions of files, in this case image files. This image is placed in the archive system (added) which will be the original version. When you need to make a change you tell the system you wish to edit an image. You do this by checking the image out of the archiving system. The command is normally “checkout” or “get”; this will mark the images as being edited. If you or someone else tries to check the image out the system again it will report that it’s checked out for editing. This prevents two versions being edited simultaneously from the same image. When you have finished editing you use the command “checkin” or put; this will tell the system that editing is finished and you wish to put the image back into the system.  The system will make the last current image a new version and the newly edited version the current version. If you make a mistake and wish to un-edit the image the command “uncheck” or “unget”; will replace the edited version with the last unedited version thus undoing your edits. To get an old version you can use “chechout” followed by the version number.  
SIA  Revision Management
SIA versions image in the way described above:  To edit an image you will need to check it out. The command “checkout” will create a new temporary version in the hidden data folder. This makes sure there are at least two copies of the original version.  You can then edit the image. Once finished editing to add the new version into the system you need to check it in using the “checkin” command.  SIA creates the new version 
A Version control system is a system that tracks incremental versions of files, in this case image files. This image is placed in the system (added) which will be the original version. When you need to make a change you tell the system you wish to edit an image. You do this by checking the image out. The command is “checkout” or “co” is the shorthand does this; this will mark the images as being edited. If you or someone else tries to check the image out the system again it will report that it’s checked out for editing. This prevents two versions being edited simultaneously from the same image. When you have finished editing you use the command “checkin” or “ci” for shorthand, this will tell the system that editing is finished and you wish to put the image back into the system.  The system will make the last current image a new version and the newly edited version the current version. If you make a mistake and wish to un-edit the image the command “uncheck” or “uc” will replace the edited version with the last unedited version thus undoing your edits.
Viewing versions
Versions of an image can as a last resort in the data folder under a second folder labelled the same as the image filename. For example; an image name of “DSC_1325.jpg” with two  version of the image; will have a corresponding folder name “.data/DSC_1325.jpg containing DSC_1325[1].jpg and the current image will be in “.data with a filename DSC_1325.jpg. If then only one version of an image then the versions folder will not exist.    
Revision History
Your SIA repository is like a time machine. It keeps a record of every change ever committed and allows to the explore this history by examining previous versions of images as well as the metadata associated with them.
SIA Revision History
SIA provides two methods of viewing this history. The first is a general activity log of all the changes made on the images in archive. These changes  are also known as Events. The second is a log of events on a per image bases.
Note the history log is separate from the application event log. The application event log is more of a means of tracking how the SIA application is functioning. The history logs are focused on the images themselves. The application log will however overlap by logging actions preformed on images.   
The History is contained in CSV files which can be easily imported into a spread sheet application.  The general activity log is also used by SIA way of catching up changes, if a remote mirror is not contactable for a long period of time and the staging folder becomes full the archive will need to catch up with the possibly of a large number of files to be transfer to the mirror when it does come back on line. 
The general history log will over time become large so to manage this the general history log will be split into a daily log, and if the daily log grows too large then the daily log will be split into smaller file will a number appended to the end. These will take the form:
 hist<year><month><day>_<number>.log  
For example a log created on 23 09 2014 will be:
Hist20140923_1.log
The image history logs will hopefully not grow too large so will be contained in a single file with the image name simply encoded within it.  The encoding will simply take the image file and append “.hst” on the end. For example the file DSC_1276.jpg will have a history file of DSC_1276.jpg.hst. These history files will be contained in the “.metadata” folder within the image folder. 



Appendix A
Support Tools
These tools are designed to support the main SIA.
idxlookup
This is a command line tool that is used to return a sequence number or set of sequence numbers given one or more image paths. 
Imagelookup 
This is a command line tool that is used to return one or more image paths given one or more sequence numbers.  
Appendix B
Image Properties
File Properties
These properties are attributes of the image file. There may be more than one image file in the group therefore these properties are unique to the file. The Metadata ID and Group Id connect the Image file to the Image Group and Metadata. All the properties are obligatory. The values are generated automatically by SIA.
1.	Image Id 		
2.	Filename	
3.	File path	
4.	Original Name
5.	Unique Id
6.	Media Type
7.	Md5
8.	CRC
9.	File Size
10.	Date Create
11.	Date Modified
12.	Date Added
13.	Metadata Id
14.	Group ID
Property description
This section describes the details of each of the file properties. 
Image Id
This is the unique image identifier. This also is the sequence number for the image is the database.
Filename
This is the current images files achieved file name. this may not be the original name.	
File path
This is the relative path to the image into the archive. This path is relative to the archives root folder.
Original Name
This is the image file original file name. This name is normally the name given to it by the camera. 
Unique Id
This is the images unique id, also called the UUID (Universally unique identifier).
Media Type
This is the media type i.e. RAW, JPG TIFF etc.
Md5
The MD5 hash value checksum. This is used to verify the data integrity of the image. If the image is changed intentionally or unintentionally the hash value will change. 
CRC
This is also used to verify the data integrity of the image. This is quicker but less rigorous.
File Size
The size in bytes, of the image file.
Date Create
The date the file was created.
Date Modified
The date the file was last modified.
Date Added
The date added to the archive.
Metadata Id
The unique key into the metadata associated with this image file.
Group ID
The unique key into the group information associated with this image file.
Asset Properties
1.	Metadata Id
2.	Title
3.	Label
4.	Rating
5.	Tags
6.	Comment
7.	Date Create
8.	Date Modified
9.	Date Added
10.	Description
Metadata Id
Title
Label
Rating
Tags
Comment
Date Create
Date Modified
Date Added
Description

Camera Information
1.	Metadata Id
2.	Maker
3.	Model
4.	Software
5.	Source URL
6.	EXIF Version
7.	Capture Date
8.	Exposure Program
9.	ISO Speed Rating
10.	Exposure Bias
11.	Exposure Time
12.	Aperture
13.	Metering Mode
14.	Light Source
15.	Flash
16.	Focal Length
17.	Sensing Method
18.	Digital Zoom
Metadata Id
Maker
Model
Software
Source URL
EXIF Version
Capture Date
Exposure Program
ISO Speed Rating
Exposure Bias
Exposure Time
Aperture
Metering Mode
Light Source
Flash
Focal Length
Sensing Method
Digital Zoom

Copyright Properties
1.	Metadata Id
2.	Author
3.	Copyright
4.	Usage Rights
5.	Copyright URL
Metadata Id
Author
Copyright
Usage Rights
Copyright URL

GPS Properties
1.	Metadata Id
2.	Latitude
3.	Longitude
4.	GPS Time Stamp
Metadata Id
Latitude
Longitude
GPP Time Stamp

Media Properties
1.	Metadata Id
2.	Width
3.	Height
4.	Resolution
5.	Depth
6.	View Rotation
7.	Sample Colour
8.	Page
9.	Colour Space
10.	Compression
11.	Primary Encoding
Metadata Id
Width
Height
Resolution
Depth
View Rotation
Sample Colour
Page
Colour Space
Compression
Primary Encoding
Appendix C
These keywords are the properties SIA
The keywords used by the metadata template files are as follows:
Asset Properties
Sequence Id
File name
File path
Original Name
Unique Id
Label
Rating
Media Type
MD5
CRC
File Size
Date Create
Date Modified
Date Added
Description

Camera Information
Sequence Id
Maker
Model
Software
Source URL
Exif Version
Capture Date
Exposure Program
ISO Speed Rating
Exposure Bias
Exposure Time
Aperture
Metering Mode
Light Source
Flash
Focal Length
Sensing Method
Digital Zoom
Media Properties
Sequence Id
Width
Height
Resolution
Depth
View Rotation
Sample Colour
Page
Colour Space
Compression
Primary Encoding
GPS Properties
Sequence Id
Latitude
Longitude
Altitude
Not in the source
GPS Time Stamp
Copyright Properties
Sequence Id
Copyright
Usage Rights
Copyright URL
