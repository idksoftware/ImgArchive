
Backups are an important part of the workflow in that they help to under pin the integrity of the archive.
 Depending on the value placed on the images in the archive, ideally at least three backups copies of the photos, so if one backup is lost, two backups remain. This is one of the key rules of backing-up and is exepted standard for safety critical system where all critical system is in triplicate. If one system fails two are still operational. In the case of an aircraft, all critical system is in sets of three; if one system is lost two remain. The aircraft is not relying on one system. One system is in use and one system is a backup. It the aircraft flies without the backup if the system in use fails then this may lead to disaster. The same is true for the integrity of the archive. Another consideration is the location of the backups and time between backups. 



Backup location


The location of the backups is a consideration when creating a backup system. The metaphor “having all ones eggs in one basket” is a good one. If the entire backup sets and the computer containing the archive is lost say by a fire or stolen is in one location. Having another set of backups is a prudent strategy. This normal case for backups and so are characterised into to three sets: 

1.	On-site - Backups are normally in the same location of the computer containing the archive.


2.	Off-site - Backups are normally not in the same location of the computer containing the archive.


3.	Off-line – Backups and usually media such as tapes, DVD and Blu-Ray. These are used for long term archiving.




On-site Backups


On-site Backups (direct backups) are normally quickly available to use in the case of a failure in the integrity of the archive. These are the first line of backups. These backups are normally hard disks that mirror the main archive. These are not part of a RAID system but run alongside the RAID. A RAID system can run without any support from SIA and is part of the archive computers hard disk system. Using a RAID system is useful in insuring the integrity of the primary archive hard drive, however it cannot guard against accidental data corruption. The direct mirror disk can be a second hard disk, USB disk or a network disk, it is does need to be quite fast and available at all times the main archive is available. 



Off-site Backups (indirect backups)


Off-site Backups (indirect backups) are normally not as quickly available. If the entire backup and the computer containing the archive is lost. Then the second line of backups will remain to re-build the archive. This is easily achievable using a cloud backup system. This requires the internet to be available. The data it transmitted to a remote hard disk. As the internet is much slower than a local hard drive the data is sent at a slower rate than a direct mirror disk and the data is sent indirectly to the remote drive. 



Off-line backups


These are backups that are not available directly. The archive data is contained on Off-line such as Tapes, CDs or DVDs. The data will need to be copied back on to the on-line system in order to use. They will normally out live hard drives for example, Blu-Ray can have 100 years life. This is a third line of backups and slow to recover the data but is used for archiving and the last stop disaster recovery. 



SIA backup support


SIA supports both direct and in-direct backups as part of the normal workflow of placing an image in the archive. As each image is added to the main archive it is also copied into the direct backup and indirect targets. Off-line media is handled separately. 



Direct backups


SIA only needs the destination folder on the hard drive of where the mirror is to be placed. Whenever a new image is added or changed the action is automatically copied to the mirror or mirrors. This increases the time to archive an image but guarantees the mirrors are up-to-date in almost real time. 



Indirect backups


Indirect mirrors are slightly different; each indirect mirror is associated with a staging folder. This folder acts as a temporary store for the images. The images are copied by SIA automatically to a staging folder along with information as to the location in the indirect mirror of where the image is located. A second process then transfers the images and associated files indirectly to the indirect mirror using the location information. This acts as a buffer between the copying from the archive and potently moving over the internet at a much slow speed. In Addison if the internet is not available then the indirect updates can be resumed when the internet 



Using Off-line media


SIA supports off line media by providing a volume set of the all or part of the archive on to a hard disk with the space to contain the volume set. Each volume is always just less then the maximum size of the target media. For example if the maximum size of the Blu-Ray disk 50G byte then the volumes will be just less than 50G byte. If the complete archive is to be backed up then SIA will run through from the starting data of when the archive was first created to the date when the last image was modified. As it does this it will copy the archive into volumes until the archive is transferred into a complete set of volumes each no bigger then the size of the media. The volumes will be labelled from 1 to the last volume in the set.
 For example if the archive can be backed up on 9 volumes then the volumes will be labelled from 1 to 9. The ordering of the images within the volume sets will be based on the time the image was added to the archive, not the date the images was taken. The reason for this is that when images are added to the archive that are given a sequence number. As the images are modified and a new version is created then a new sequence number is given to the new version this means that the archive can be incrementally backed up by sequence number. 
