Chapter 7.	Importing images
Importing Images
In order to add images into the archive you must Import them.  Image Archive allows you to import images in many ways so to support the growing number of options cameras have to shoot and manipulate those images while on the camera memory card.
Most of today’s cameras can shoot a number of formats, but these are grouped into two sets,  RAW and Picture formats. A growing number of cameras can now shoot a mixture. 
RAW images are those that are direct from the cameras image array with little or no processing by the camera. These are normally proprietary belonging to the camera makers own format.
Picture formats are those formats that applications can view directly because they follow a industrial standard that is widely recognized. These are JPG, TIFF, PNG, BMP etc.  
Supporting RAW and Picture formats.
As digital photography evolves new file formats will be introduced, so in order to support new formats Image Archive contains all the file format extension into a configuration file. When Image Archive starts it loads this file. 

Importing images into Image Archive is how you put images in to Image Archive  
The first stage of processing images to be imported into the system is to sorts them into there types and groupings.
 * It will handle the case where both a Raw file and a jpg file was produced
 * by the camera at the same time, as well as simgle RAW or JPG image files.
ImgArchive Comand Reference
Table of Contents
This chapter is intended to be a complete reference to using ImgArchive commands..

Command search paths.
If the name of the command is passed on the command line then the paths with be searched in the following order:
Any paths passed on the command line.
If no command xml path is passed but the Command name is passed then the default xml file associated with the command is used.
If no command line arguments are passed then the command folder is searched for a command.
XML Command format
<?xml version="1.0"?>
<Envelope>
<Header>
...
</Header>
<Body>
...
</Body>
</Envelope>
  If the command block can be ignored if the command is known explicitly.
