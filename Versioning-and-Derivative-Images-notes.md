===== Chapter 6. Versioning and Derivative Images ===== 
Introduction

This chapter takes you through how image archive deals with versioning derivative images. The first part is to define what we mean by versioning and derivative images. 

Master images
Master images are the original created image that formed on the camera’s sensor. For example, a raw image is closer to the image formed in the camera than the accompanying JPG as this is an almost unprocessed image file. This image is the master.
Derivative Images
What are derivative files? Each time you take a photo of the resulting image file located in the camera's memory card will be the original. All copies after of the original are then derivatives of the original. This also includes changes to the copies of the original. For example, a raw image is closer to the image formed in the camera than the accompanying JPG. The JPG is generated from the raw image by the camera so is a derived image. 

Transformed derived images
These images are transformed images of the master into another format. If you take RAW plus Jpg when out taking photos, the jpg is the transformed derived image as it is a transformation of the RAW. These images can live with the Master (RAW) image as they will have the same name, but a different file extension. At a later date you may add a tiff version, That is ok as it will have .tiff or .tif as the file extension.
The Image Set

The images set is the 

Handling Derivations
Image archive handles derivations in two ways:
•	Versioning - Each version of the original will be kept in the same image entry in the archive. 
•	Linking – A new image ID made in the archive. The derived image is moved into the new image ID and linked to the original image.




Versioning
When using versioning each version of the original will be kept in the same image directory this enables you to keep the original and all the versions of the original in the same place. This is great for when you need to retouch the image such as getting spots and blemishes out of the original photo.


Each new version will be labelled the same as the original, but with an extra version attribute is made in the file name. The most resent image will not have a version attribute, and will be the one used as the master.

For example, the original may have some spots and blemishes. You remove these, and save back into the archive, you then decide to it needs some further work with to bring out the best in the image. This may mean a bit of colour correction, retouching and sharpening. So you again get the image out of the archive, and do the work and save it back in the archive. 
The archive will now have three versions of the image, version 1 the original, version 2 the one with the spots removed, and the last version 3, the new master, that has the sharpening work done to it.
 
Adding new transformed derived images

Linking

Linking on the other hand, deals with derivations of the original in a completely different way in that the new derivation will occupy a new image ID. But, in order to preserve the relationship between the original and derived image, two way links are made in the archive. Derived image or to all intents and purposes, is a completely new image.

Preserving almost read-only partitions
One feature of image archive is to allow almost read-only partitions. Once a partition is inactive, i.e. files are no longer being added on a regular biases. Image Archive shall avoid added extra maintenance file to the partition.  

This keeps the management of backups are much more easier to maintain in that, once a partition has become pretty much dormant a number of copies may be made and then stored in a safe place within knowledge that the files within these partition is will not change.
In order to do this image archive keeps links from the original files pointing to the derived files in a separate index. However, the links pointing backwards from the derived files to the originals will be stored with the derived images so once the new partition become dormant these links will become dormant with them.


                                                                                                                               
This convention is described in the following sections:

File prefix
This is a prefix that  

Tags or Codes should be appended to the filenames to indicate derivative versions of image files
Successive versions of the same file—a master file, for instance, or one converted to black and white—should have some kind of tag added to the filename to note the enhancements. This is generally preferable to renaming the entire file. (There are times when you might want to rename a file entirely, though, as discussed in Chapter 8.)

The filename does not have to carry important content information about the file
Putting content information in the filename can get complicated, and it takes up valuable character space. However, I do suggest appending format information about the derivative files to the filenames.

A file naming system that incorporates your name can help your clients keep track of your images
As discussed below, adding a string (a sequence of letters) in the filename that identifies a file as yours will help your clients remember where it came from.

 
Metadata pre-sets
This commands are in the form of an xml file found in the template folder for the command in question. This can be used to fill-in arguments that are not present in the main command line xml file.
An example whould be an Import command where a batch of image are to be processed.  A template file with most of the arguments defaulted can be used to create a template of metadata parameters for the batch. Say you had a holiday in Cornwall all the shots were landscape then the Keywords and Tags will be the same for most if not all the batch. The Camera will probably be the same along with a lot of other information that is the same for all the image s in the batch.
