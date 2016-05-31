#Metadata
##Introduction
Normally one thing about photos is that in order to use them you need to see them to known what’s on them. This normally means looking through hundreds of images to find the one you looking for. A better way is to keep information on each image so the search for image is much more targeted so the number of images needed to be searched through is very much reduced. This information is called metadata. 
Metadata is traditionally found in in the card catalogues of libraries.  This information as become more digital and extended to describe digital data using metadata standards specific to a particular field, in this instance photography.
    
Metadata is the information about every aspect of the photographic image. This can be split into the main properties the image presences, this being: 

1)	Image File Properties

2)	Asset Properties
 
3)	Media Properties

4)	Camera Information

5)	Location Information

6)	Copyright Information

####Image File properties
These properties refer to the Image file its self. These properties can be applied to almost any file. These properties incl  


####Asset Properties
Asset Properties are things like the file name of the image, is location in the archive, the time data added to the archive etc. 
####Media Properties
Are the images properties such as the images width and height in pixels, its view rotation, is primary encoding. 
###Camera Information
These are camera orientated properties such is the make/model of the camera, the ISO speed rating, exposure time, aperture etc.
####Geographical Location Information
The location of where the image was taken using latitude and longitude measurements. 
####Copyright Information
Copyright Information is information on the owner of the image and the rights to the image. 
###Metadata Types
This refers to how the data is generated and is divided into levels. Each level adds greater descriptive value but at the expense of additional effort. This may be offset by the ease of finding images in the archive.
Automatically generated
This is the lowest level of metadata but lays a base for all higher levels and contains the boiler plate information on the image.

This information can come from four sources:

1.	base template information

2.	The image file information

3.	Exif standard information

4.	Tool generated information.

5.	Hand edited information.

###base template information
This information is default information held in the base template. This is stored in the templates folder. This information contains information that almost all images in the archive will contain. If you are the only photographer submitting to the archive then your name and copyright will be in the base template.
The image file information

This information is generated from the attributes of the file. For example the file will have the following information:

1.	Creation date and time

2.	Modified data and time

3.	A file size

4.	A file name

5.	A file path in the archive

In addition SIA will use the image file to generate the CRC and MD5 checksums. These checksums are used the guarantee the image has not changed after the checksum has been made. For example, if a checksum was created from an image. If that image is then changed (even one byte), if a new checksum is taken then the new checksum will not match the old indicating the image has indeed changed.

The checksums can be used for a number of purposes in the archive. This includes checking for accidental modification of images and testing if an image is not the same as another. One important use is preventing duplicates of an image being added to the archive by maintaining a list of checksums of images already in the archive. It is very unlikely for two different images having the same checksum (I in 14 million in the case of CRC). The MD5 checksum is even less likely to produce the sane checksum for different images.

A form of UUID (Universal Unique identifier) is also generated for each image. This is a unique identifier generated using a random key based the time and date in seconds plus other random data found on the commuter the identifier is generated.      
        
All this information is automatically saved a part of the images asset properties.

###Exif standard information
Exif information is a photographic standard that most if not all camera manufactures use to encode information into images. It is a photographic standard by which information about both the image and the camera that captured that image is stored within the image file. In order to read this information an Exif reader is required. This information is normally stored in JPG images; however other formats can also have the Exif information encoded.

SIA can read JPG image and generate the basic Exif information. In order to read the complete Exif information from formats other than JPG the an external reader will need to be used. SIA can be integrated with a number of external readers such as exiftool. Then SIA fails to read Exif in a image file will (if installed) use the external reader. Or optionally always use the external Exif tool.

###Tool generated information.
SIA will automatically generate some metadata for identification purposes this includes the following:

1. The CRC checksum of the image
2. The MD5 checksum of the image
3. A Unique Identifier of the image
4. A sequence identifier of the image

The first two items are to finger-print the image. The CRC is a number that is generated by reading each byte of data in the image and creating which is unique to the contents of the image. If one byte is changed in the image then the checksum will not match. There are about 14 million combinations. The next finger-print checksum is an MD5 digest. This is used to securely encode a finger-print of the image. This type of encoding is use by encryption products to validate secure media.
 
The MD5 algorithm is a widely used hash function producing a 128-bit hash value. Although MD5 was initially designed to be used as a cryptographic hash function, it has been found to suffer from extensive vulnerabilities. It can still be used as a checksum to verify data integrity, but only against unintentional corruption.

Like most hash functions, MD5 is neither encryption nor encoding. It can be reversed by brute-force attack and suffers from extensive vulnerabilities. However a large amount of time and effort needs to be used to attack MD5 and so for this propose is more than adequate. 

The last two items identify the image. The first is a unique number that is based on the time and the machine the number the number was generated. As the time is unique and the machine network adapter contains a unique number to then generate a long random number sequence almost guarantees a unique number. These numbers are normally call UUIDs and are used uniquely identify digital media.
The second is a sequence number used within the archive. If the database is used then this number is generated by the database, otherwise increasing number is used in a configuration file. This number will be unique within the archive.  

###Bulk entry metadata
This is second level template data entry based on a set of images. Most imports into the archive will be a number of images taken in the same context. It may be a photo shoot of a subject or location, most of the images will contain the same information. SIA uses a second level template process to apply this metadata. When the images are to be imported all the images that are related need to be within the same folder. When that folder is imported then the template to be associated with the images within that folder can be specified. All the image will then contain the information in the template and the end of the import process.

###Unique image metadata
This is the most time-consuming as it is applied to individual images normally by you the user of the archive. This sort of information may uniquely identify image to you. For example, in an image description you may put “James blowing out candles at his birthday at 5 years old.” The content being James blowing out candles? This description will make finding this image quite easy, but time consuming to do. But the importance of the image may make it worth doing.

##How SIA handles Metadata
SIA uses a number of diffident methods to generated and manipulate metadata from the base template bulk Metadata to the top level image specific metadata.
Starting at the bottom the template based data is entered using cascading templates. These templates are contained in the template folder. The first default information is placed in this template. The sort of information that fits in here is probably your name in the Author field and your copyright information. This will make sure your name and copyright is on all images in the archive. To create this file you will find a file call template.dat. This contains all the fields SIA will read, however each field is commented-out. Copy this file to a file called base.dat, open it, and uncomment the options you need. For example the author field will look like the following:
Author = Your Name
To update it with your correct  find   The next level may be the general information about   


# Camera Generated Metadata.
When the camera captures an image it will store information on that image at the same time, this can be quite detailed. However most of the important details can be stored in a short set of data this will include the image size, is orientation, the date and time the image was taken, the ISO, aperture, exposure, if a flash was used and in some circumstances the GPS location. This information is in a standard called [Exif](Exif).  SIA will read the basic EXIF information. However it can also operate with other tools to provide a more detail set of EXIF date if required.
The basic data SIA extracts:

•	Camera Make/model
•	Software
•	Bits per sample
•	Image width/Height
•	Description
•	Orientation
•	Copyright
•	Image date/time
•	Original date/time
•	Digitalised date/time
•	Sub-second time
•	Exposure Time
•	F-stop
•	ISO Speed
•	Subject distance
•	Exposure Bias
•	Flash used
•	Meter mode
•	Lens focal length
•	35mm focal length
•	GPS latitude/longitude and altitude

Bulk Generated Metadata

###Metadata Templates
Metadata templates are used to create basic information to be attached to images. For example, you will be the author of the images you take and modify you also will most probably be the copyright owner etc. However a friend may have asked you to archive some image for them, in which case the author and copyright will be them. To apply the correct keywords a new template can be added that is read after the one applying your name with the friends name, thus there name will be the last keyword substitution.
This information will normally be cascaded i.e. base data may be preplaced will more targeted data for the image that the templates relate. 
The templates are text files that contain a metadata keyword and the value that the will be placed in that keyword. One template may include other templates. Templates are read from the top down to the bottom of the page, any included templates will be read when the include statement is encountered.  A primary template will be associated with an image or images. This will be the first template to be read and the process will continue until this template is read from top to bottom. An in memory template will be created at the end of the session to be used in further metadata processing namely the addition of Exif metadata and user generated metadata. 

This contains the metadata to be substituted during the template process. As each template is read the contents will be placed into this class. This class will be used for further substitutions by the Exif reading and user defined keyword substitutions.
The need for DNG
If you shoot RAW images then the RAW image format you capture will be proprietary to the manufacturer of the camera you are using. Each major manufacturer will have their own format which will probably incompatible with other manufacturer’s format   The manufacturer will control that format and may change it at any time. This is not good news for an archive that may be around for many years and have RAW images from any number of cameras from different manufacturers. What is required if a RAW format that is independent and open to all. Adobe saw the need for such a format and came up with DNG. Digital Negative (DNG) is open source so any one can read or write the format and the are no licence restrictions. 
SIA support for DNG
SIA supports DNG using a SIA process-raw hook. This hook is called on a RAW image file before being added to the archive. After this hook is call the source directory is scanned for new file with the same name as the one being archived. For example if a RAW file called DSC_1243.nef is being processed then if the process-raw hook creates  DSC_1243.dng, SIA will add this to the archive and add it as a side car file of DSC_1243.
Adobe DNG converter
This is a command line tool produced be Adobe to generate DNG files from RAW files. Using –e the tool will not show a options dialog but work only with the command line.
The Digital image Workflow
This defines the normal activities you may carry out from capturing the image to the final cataloguing it in the archive. A lot of these steps can be automated using SIA. But to start here is a basic work order:
1.	Capture Image(s)
2.	If out on location without full back-office support make second backup copy to second memory card, data-bank drop-box etc.
3.	Download to computer.
4.	Make DNGs
5.	Rename 
6.	Apply Metadata
7.	Apply Ratings
8.	Apply Keywords
9.	Archive Images
10.	Copy to primary backup system
11.	Copy to off-site or cloud backup system
12.	Erase cards  
Some of the above are optional. However it may be prudent not to erase the memory until you are confident that your images are safe in the archive and backed-up
