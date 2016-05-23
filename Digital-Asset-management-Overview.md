#Digital Asset management Overview

Can you find your digital images when you need them, or do you spent more time looking through endless folders on your hard drive? This can be even worse if you spent hours editing the image in Photoshop but can’t find the latest version. The Image archive provides an organised place to store digital images for safe keeping, editing and viewing. SIA is a full featured Digital Image archive system with version control following the Digital Asset Management (DAM) design goals. 

## 1. Do No Harm

Never manipulate your original media, i.e. the original image or images. Once the original is changed there is no way to revert to the untouched original its lost forever, always work on a copy of the original never the only copy of the original? SIA supports versioning, when you make changes to an image in SIA you check the image out of the workspace for changes, once the changes are made you then check the image back into the workspace. The changed version will be the current image in the workspace but the original will be preserved in the archive untouched. Each time you make a change a new version of the image will be made. If you wish to go back to the original or a previous version of the original the you can check it out into the workspace. The original is safe guarded along with any changes you made of versions of the original which in them selves may have had a number of hours invested in them.  

## 2. Be Safe With Your Data

Digital images fractions of a second to make and fractions of a second to destroy or lost. This can happen in a number of ways. For best protection, use the 3-2-1 rule of duplicating your data. This is standard practise in the IT Industry. 

◾ 3 Keep at least three copies of your data: your primary copy and two backups.

◾ 2 Keep your copies on two different, physically separate media. One set of files will likely be on your computer hard drive. Keep your two copies separated, each backed up to different media (external hard drives, USB flash drives, or cloud storage services).

◾ 1 Keep one copy of your images offsite. Some people use a system of rotating two external hard drives, swapping out one hard drive each week or each month, depending upon how frequently you add data. I use USB hard drives and store them away from main archive. A cloud storage service is also a good way of keeping a copy offsite, however some cloud storage does not store RAW image data or metadata. A non photographic cloud storage site may be better as your image data will be just that, and the cloud site will not try to manipulate your image data in any way. 

## 3. Back Up Supporting Documents

As part of the DAM process is to create metadata on you images 

At its core SIA uses the base Operating Systems file system in order to store image in an organised file structure. Metadata associated with each image is also stored in an organised file structure with addition of an optional SQLite database. 

## 4. Never Use Your Archive for a Backup.

An archive is the place where one copy of each version of an image are stored. This usually means that your archive contains the original, untouched image file, and possibly the final working versions of the edited image files derived from the original or originals. These may be your originals which may be irreplaceable, but also may contain versions that may of taken some time to edit and get them just how you like them. If the archive is lost how would you recover?
Luckily SIA contains the ability to maintain  


It is so tempting to use a little corner of the drive holding your archive to back up working files. It would be so easy. I knew the advice not to use an archive for a backup, but I did it anyway. Just once. And I forgot to protect the root structure in the destination drive. I had a great backup of all of my working files, which had nicely overwritten my archived files. I was lucky and was able to restore my archive from my archive backup, but I can tell you that it took hours for my heart to settle back down into a normal rhythm. Save yourself the potential for disaster and keep the medium you use for your archive just for your archive.

## 5. Regularly Check and Update Your Archive

Looking in your file directory and seeing an organized list of all of your image files is such a reassuring vision, but don’t be fooled. Just because the image file shows up in your file directory does not mean that the file is usable. The file could be corrupted or it could be in an out-dated format. The storage medium could also be corrupt, damaged, or out-dated.

It’s a pretty safe bet that you’ll be able to open your files without proprietary software in years to come if you save your RAW files as .DNGs and your finished working files as .TIFFs (with layers where necessary), but that doesn’t preclude other problems with the files or storage media. Establish a process for validating your files and verifying the integrity of your storage media. (There’s much to learn about data validation; we’ll be publishing an article dealing with that in this series.)
