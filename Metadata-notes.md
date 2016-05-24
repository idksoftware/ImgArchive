===== Chapter 3 Metadata =====
Metadata is a term that generally refers to data about data. In this context, the data is the image and the data about the data is the information about the image.

There are a number types of data that can be applied to an image. These are listed below:
•	Media Identification 
•	Physical Attributes
•	Media Properties
•	General Camera Details

Media Identification 
This data identifies the media within the archive. If this was a 35mm slide then this information would detail the box and shelf numbers of where the slide was stored. In the digital domain this translates to a identity number and revision that is used within the archive in order to keep track of the image. In addition to this an image label and the original file names for the images before imported into the archive.

Physical Attributes
These attributes pertain to the physical properties of the image. i.e. last modified, is size etc. At the same time some of the physical attributes can be used to finger print the image. 
Image Archive uses two finger printing techniques, Cyclic Redundancy Check (CRC) and MD5.
A cyclic redundancy check is an error-detecting code designed to detect accidental changes to raw computer data, and is commonly used in digital networks and storage devices such as hard disk drives. Blocks of data entering these systems get a short check value attached, derived from the remainder of a polynomial division of their contents; on retrieval the calculation is repeated, and corrective action can be taken against presumed data corruption if the check values do not match. Only a small change in the image with make a large change in the checksum generated. The number generated is a 32bit number so there are 4290 million number combinations. A stronger finger print technique is the MD5.
The MD5 Message-Digest Algorithm is a widely used cryptographic hash function that produces a 128-bit (16-byte) hash value. MD5 has been employed in a wide variety of security applications, and is also commonly used to check data integrity. An MD5 hash is typically expressed as a 32-digit hexadecimal number. For example 
6838a143-44b7-4d81-94c9-8f401e32a8ee is a generated MD5 number and it is very unlikely to be generated for another different image.   
Image archive uses these techniques to determine of a new image currently being imported into the archive is unique or is a duplicate. CRC as a first point to determine the images uniqueness, if there is a match then the MD5 is checked if the MD5 is different then the image is deemed to be unique. In addition all controlled files, that is files controlled by Image Archive will have a CRC checksum. This is used to track data corruption within the archive.
 
The intent of UUIDs is to enable systems to uniquely identify information. In this context the word unique should be taken to mean "practically unique" rather than "guaranteed unique". Since the identifiers have a finite size it is possible for two differing items to share the same identifier. The identifier size and generation process need to be selected so as to make this sufficiently improbable in practice. By creating a UUID and use it to identify an image within the archive with reasonable confidence that the same identifier will never be unintentionally created for another image.

The Archive name is the name of the primary image. This is the image closest to the source of the generated image. For example, a raw image is closer to the formed accompanying JPG. The JPG is generated from the raw image by the camera so is a derived image not the master. 
  
Media Properties
The Media Properties are properties that are associated with the image. Things like width and height of the image are properties that fall into this category. However things like the aperture and exposure are properties of the camera.  


General Camera Details
This details are relevant to the camera and how the picture was taken from example the Aprature, exposure speed, iso rating whether Flash had fired, the make and model of the camera. All of these are the camera details. Luckily most of these will be furnished by the camera manufacturer is a standard way it can be automatically be retrieved.
Summary Information

The summery information is a summary of the metadata. The first sections are controled by Image Archive as a part of the maintances of the of the archive and is basic archive house keeping information. This includes the Identity of the image within the archive, the Jounaling of the image and the images files phyical perperties.  The second part the summary information is the part that contains the Image Properties. This information is the summary of the the image’s Metadata. These two parts are sumorised below:

	House keeping Information
•	Identity
•	Jounal
•	File

	Image Metadata
•	Asset Properties Information
•	Media Properties Information
•	Camera Properties Information
•	GPS Properties Information
This summary Information is kept in an XML file with the prefix IFO followed by it is 8 character achive identerty. This file is sectioned into the same section heading shown above. Each of these contain information fields that are detailed further in the following.
House keeping Information
Identity Section
This section contains the fields required to identify the image within the archive.

This section contains the following fields:
•	Number – This is the archive identity number. This is used to idenigfy the image within the archive. This number must never change. If it do’es then the integrity of the archive will be broken.
•	Revision – This is the current revision of the image. This is used be the versioning path of the archve. 
•	Lable – This is the lable given to the image. This may be used as a file or part of the file name if the image is exported out of the archive.
•	OrginalRaw – This is the orginal RAW file name. This may be empty if there is no RAW File 
•	OrginalPic – This is the orginal picture file. This may be empty if the is no Picture file.


Jounal
This section details the journal 
  TimesBackedUp
  InPrimaryStorage
  CheckedStatus
  Publishable


File
This section details the file attributs of the image these are as as follows.
•	OriginalFile – the primary file name of the image.
•	LastModified – the last time modified.
•	Size – Its size in bytes.
•	Crc – The CRC checksum of primary image.
•	Md5 – The MD5 checksum of the primary image.
•	ArchiveName – The file name of the primary image stored in the archive. 
•	UUID – The unque number for the primary image.

Image Metadata
These sections are controled be you the user. The  first,  
Asset Properties Information
  Caption – The caption or name of the image.
  Category – The category that the image is cataloged under.
  HardCopyLocation – If there is a hard copy of image, this details where it can be found. 
  Language – The language the image was taken under. This relates to the language the imbeded metadata is captured under, or the languege the image editor uses.
  Type – The type of image.
UserComment – Used to store any comments on the image.
 Editor – The image editor used to manipulate the image post capture.
 Tags – Tags allow the image to be taged by tag names.
  Rating – Quality rating of the image.



Media Properties Information
Width
Height
DateTime 
Orientation
XResolution
YResolution
ResolutionUnit
ColorSpace 
  Compression

Camera Properties Information
  Maker 
  Model
  ExifVersion
  F-Number
  ExposureProgram 
  ISOSpeedRatings
ExposureBias
DateTimeDigitized
ComponentsConfiguration
  ExposureTime 
  WhiteBalance 
  MeteringMode 
  ExposureTime 
  Aperture 
  ExposureBias 
  MeteringMode 
  Flash 
  FocalLength 
  FlashPixVersion 
  LightSource 
  SensingMethod 
 FileSource 
  SceneType  

GPS Properties Information

  
  
 VersionID 
  LatitudeRef
  Latitude 
  LongitudeRef 
  Longitude 
  TimeStamp
  MapDatum 

Photographic Metadata Standards
Photographic Metadata Standards are governed by organizations that develop the following standards. They include, but are not limited to:

•	IPTC Information Interchange Model IIM (International Press Telecommunications Council),
•	IPTC Core Schema for XMP
•	XMP – Extensible Metadata Platform (an Adobe standard)
•	Exif – Exchangeable image file format, Maintained by CIPA (Camera & Imaging Products Association) and published by JEITA (Japan Electronics and Information Technology Industries Association)
•	Dublin Core (Dublin Core Metadata Initiative – DCMI)
•	PLUS (Picture Licensing Universal System).

Image Archive at this time supports IPTC Core Schema, XMP and Exif. Each of these standards are detailed below:

Exchangeable image file format (Exif) 

Exif  is a standard that specifies the formats for metadata that is stored in images by digital cameras (including smartphones), scanners and other systems handling images by manufactures. The specification uses the following existing file formats with the addition of specific metadata tags: JPEG DCT for compressed image files, TIFF Rev. 6.0 (RGB or YCbCr) for uncompressed image files, It is not supported in JPEG 2000, PNG, or GIF.

This information is normally embedded in the image JPEG. Most cameras now will embed this information automatically in any JPEG image camera produces. Most high-end SLR digital cameras can shoot images in raw mode however it  also can create a second image as JPEG. As raw images are sometimes difficult to view having a JPEG file along with the raw file does not take up much in the flash memory but makes archiving very much easier. Image Archive supports this arrangement as a growing number of camera manufactures. 
Image archive enables you to import both files as a single image using the JPEG and its associated EXIF information to be transferred into the XML information file.
Alternatively, if an image is taken in raw format only, third-party tools such as DCRaw or UFRaw. These allow the EXIF information to be exported into a second JPEG. Image archive allows such tools to be used as part of the import process.

XMP information
This information is generated by Adobe Photoshop and Lightroom. Adobe has the option to save this information as an external XMP File. Image archive has the ability to import this file as well as read this file so its own XML information file can use the additional information.

International Press Telecommunications Council (IPTC) information
The Information Interchange Model (IIM) is a file structure and set of metadata attributes that can be applied to text, images and other media types. It was developed in the early 1990s by the International Press Telecommunications Council (IPTC) to expedite the international exchange of news among newspapers and news agencies. Most of this information can be found in the Exif standard. 


IPTC information

Propeties supplied by the camera maker
Artists Contact details
Creator

Creator’s Job Title
Contact Address 
Contact Email
General Image details
Date Created
Copyright Notice
Job Identifier Number 
Location
Content details

Description
Keywords
Status details

Title
Job Identifier

Instructions
Rights Usage Term


Importing Metadata Information

Importing metadata falls into four main types:
•	Defaulted Generated
•	Automatically Generated
•	Template Entered
•	User Entered
Automatically Generated
Automatically generated information can come from only tree sources:
•	Information from the image media
•	Information from the Archive
•	Information imbedded within the image
Information from the image media
This information is derived from the media file, i.e the file size,  crc, md5 etc can all be generated by analysing the image file. This information is normally controlled by the archive as it can determine properties of of the file automatically.
Information from the Archive
This information is generated by the archive in order to maintain an orderly maintenance of its housekeeping, for example maintaining version control on images, the last time backed up. This information again is controlled by the archive and is important that most of this information is not modified by the user as some of the information is used internally to maintain the integrity of the archive itself one such field is the internal image number. This number is used within the archive to track the image if this is modified within the archive will lose track of this particular image.
Information imbedded within the image
information embedded within image normally accompany Exif standard most camera manufacturers will embed this information and it is available in JPEG and TIFF and most raw format files image archive reads this information and generates an optional ex-if file as well as using this information to populate fields within the summary information file. This information is very useful in that it comes directly from the camera if example you have setup your name is the author and a copyright message this will also be applied to  meta data within the summary file using the exif standard. Information found in the exif standard can be broadly described in two parts. The first part is the media information, this information relates to the image itself surfaces height and width orientation etc.
The second part is the information is only relevant camera he second part is information that is only relevant to the camera and how the camera was used to take the image such as the exposure time white balance the Aperture etc.

In addition the excess standard also provides limited amount of information that can be used in the asset information section. A new part of the ex-if standard is to provide GPS information this necessitates normally a GPS to be either installed internally in the camera or externally on some sort of Mount this information will then allow the exact position of where the camera was positioned when the shot was taken. In order to store this information a GPS section is included in the summary.
Template Entered
This information in some asset management systems call this bulk entry data. This must be user generated information that can be applied to many images at once. This information can be built-up of tiers starting at basic information and building up to the more detailed information that may only be applied to a particular small set of images.
			
image archive provides a method by which you can generate a template file or files fit store commonly used information needs to be copied to each image taken a prime example of this would be your name each image you take will be belonged to you and this information would be automatically copied into summary information. In addition to this and number of template files can be used together this means that you can have a standard base template and if example you have been taking photos at a particular place you can generate news tinplate which will fill most of the fields for their particular shoot example if you took a load of photos say a firework display all the photos will need to have details of the same firework display in the differences within the image set would be differences between the individual images this intend to reflect the tiers of information starting from basic up to small sets of individual groups of images. Image archive allows you to have a directory these templates to be called up whenever required.
User Entered

The first is the Media Information. This includes information such as location, file size and format, and other file characteristics of the Media file itself. Typical information will be something like the following:
Filename
Preserved filename.
Document kind (i.e. TIFF, RAW, JPG)
Application (i.e. Photoshop CS4).
Date
File size
Resolution
Bit Depth
Color Mode
Color Profile
The other kind of automatically generated information (for digital capture, at least) is the EXIF information. EXIF (Exchangeable Image File Format, pronounced ex-if) is a standard used by camera manufacturers to store properties such as shutter speed, ISO, date/time shot, and so on. Although manufacturers don't store all of this information in exactly the same way, the bulk of it is easily accessible and index able by most DAM software.
The Second type of Metadata 

Metadata Type 2: Bulk Entry Data
In the second tier of metadata information, we move into the category of user-supplied information. This data is not automatically generated and must be entered by hand. The most basic of this, bulk entry data, is that which can be applied to many images at once.
This could include information such as the name of the subject, the location where the photographs were shot, the client's name, and other characteristics that don't involve individual evaluation. Certain kinds of bulk metadata should be entered for all images, because doing so is quick and provides a very good return on the time invested.
Metadata Type 3: Higher Metadata
The most valuable metadata—but also the most time-consuming to generate—is higher metadata. This includes the higher-touch information you generate when you rate, assign keywords to, and group image files so that you can find them quickly when you're searching. Typically, this information takes longer to assign because it is part of a more complex evaluation process. In the next section, we'll take a closer look at these higher levels of metadata information.
Meta data.
Meta data is a term that generally refers to data about data in this context Metadata refers to the data about the photograph or image.
Metadata is split into a number of parts. Each part refers to a particular aspect of the photograph.

File properties
file properties are the most basic part of a digital image namely it is all about the file that the image contained in, This includes things like the size and format of  the file, its location, the time is added into the archive and checksums, so that if  the image for one reason or another becomes corrupted it can be spotted and replaced by a backup of the image.
Image properties
these properties describe how the camera was set up when the image was taken, these include the make and model of the camera shutter speed and aperture the ISO whether Flash was used almost every aspect of how the image was taken by the camera is captured in this data.
User supplied information
This information is not automatically generated but needs to be added by hand. This deals with the business side of the image along with the information required to properly catalogue image so that it may be found easily in the future. This information is as diverse as the title or subject the image to who has rights to the image.
Both file and image properties are normally in some way either captured by the camera taking the image and/or the archiving software. A number of standards have emerged by the industry the main three are IPTC information, XMP information and EXIF information.
Image archive captures this Meta data in an information file or files. The format of these files are XML.