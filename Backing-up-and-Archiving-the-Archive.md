#Archive Storage
##Backup vs. Archiving
Good DAM management includes both backup and archiving. However, while these terms are often used interchangeably, it is important to distinguish between them when considering a DAM process. Backup is used for data recovery, while archiving is used for preserving and retrieving data in the event of a disaster, inquiry or litigation.

In simple terms, backup is short-term and archival is long-term data retention strategy. Specifically, backup is a snapshot or picture of the state of the data at the date it was carried out, before it disappeared or was destroyed, with the data periodically overwritten as it changes. In the backup process, a copy of data at a specific point in time is created in case something should happen to the original. Therefore, in the case of a failure, the data can be reconstructed from that time. Therefore, to be most effective must be done as often as possible. 

On the other hand, archiving is long-term and unalterable. Used for compliance or disaster recovery, redundancy and physical separation are crucial to effective archiving. Archived files can be kept for decades in the case of some of your images, Best at an offsite location, and two or more remote copies are better than one to properly safeguard this data. Retention schedules are the foundation of a successful DAM management process. These schedules take into account an organization’s legal, regulatory and operational requirements  while providing guidance on how long images need to be kept and what to do with them when they are no longer image. It is important to develop a schedule for backing up and archiving 

So, Backup: Short-term storage Archiving: Long-term storage.

###Backing up an Image archive.
The strategy for backing up an archive is slightly different to backing up say your desktop PC. The first thing is that the archive is arranged in date order. That is, as you add images to your archive the image number grows and so the newness image have the highest number and the oldest the smaller number so the place of where old an new is known. This is by contrast to your PC that has a random pattern of new and old file spread over the PC’s hard drive. The other main difference is that the images will not change very often, once in the archive new images will be added but old images will not be deleted. This will not be true for the PC. And one last main difference is that the images will need to be kept for a long time, where files on the PC will be a mix and range from file that need not be backed up i.e. temperay files, to files that need to be archive in the same way as your image files. So the strategy that is adopted may be summarised as follows:
Obviously, since properly testing an archiving strategy would take at least 50 years, and my deadline is much shorter, I can't guarantee that any of my suggestions will work. Still, a few wise choices can improve your odds of success.
Archival media should satisfy the following criteria.
The media must be inerasable: According to the law of data entropy, whatever can be erased, eventually will be erased. (But beware of the corollary to that unalterable law, pointed out by PCWorld.com community member JimH443: "That which cannot be erased will be misplaced.")
Media and mechanicals must be separate: Hard drives hold a vast amount of data--up to 2TB on a single drive--but like other mechanical devices with moving parts, they can break. A well-made, properly stored CD or DVD (see "Burning Questions: Ten Tips for Durable DVDs" for more on how to store discs) frees you from having to worry about mechanical-component failure. If you expect moving parts to last for decades, you'll be disappointed.
The media must be inexpensive: The cheaper copies are, the more likely you are to make multiple copies, which in turn will increase the odds that at least some will survive.
The media must be ubiquitous: If everyone uses the medium now, chances are better that someone will be able to use it in the 22nd century--or at least later in this century.
The media must be robust: It needs to survive for decades.
CD-Rs, DVD-Rs, and DVD+Rs meet the first four criteria, but--despite various claims of longevity based on laboratory aging--no one knows how robust they'll be in the long run. Representatives of leading data-recovery companies DriveSavers and Ontrack told me that they occasionally see optical discs with symptoms possibly related to age and poor manufacture, but that it isn't a common problem.
Your best bets among optical discs are relatively expensive archival discs such as Delkin's Archival Gold, Kodak's Gold Preservation, and MAM-A's Archive Gold. The makers of these discs claim to use higher standards for them than for run-of-the-mill data discs; and all use gold rather than silver in the disc's manufacture to increase longevity (see "Burning Questions: When Good Discs Go Bad"). But archiving concerns extend to other issues such as whether the file format will be readable in 50 or 100 years. Your chances are better if you stick to popular formats such as .jpg, .mp3, .doc (but not .docx), .txt, .html, and .pdf. And the more formats you can save the file in, the better.
You should store the discs in jewel cases, upright, away from direct sunlight, humidity, and extreme temperatures.
Another possibility is to burn a copy to Blu-ray Disc. Blu-ray doesn't yet enjoy the same reach that DVD and CD have, and it remains pricey (about $25 for a 50GB disc). But Blu-ray far exceeds DVD and CD in capacity, and the format is gaining acceptance rapidly. Only Buffalo, LG, and Pioneer market Blu-ray burners today, but the LG Electronics NAS NB41 packs four drive bays and a Blu-ray Disc burner.
With a little luck, your great-grandchildren will enjoy your mementos from the early days of digital photography.


You should never have only one copy of anything, including your backup. Multiple backups add to your protection.
If you follow more than one of the strategies above, you'll have multiple backups. For instance, the image backup you create to protect Windows and your applications adds supplemental data protection.
I endorse having more than one image backup, as well. Archiving your digital heirlooms introduces additional copies of files that should also be part of your regular, daily backup routine.
A few other options deserve mention.
A NAS drive such as the Synology DS209j permits you to back up your backup. You can plug an external USB drive into the DS209j and back everything up on it. Taking this step is essential if you use the NAS drive to store shared media that is unavailable on your local PCs, but it's a good thing to do even if you're simply backing up the backup.
You can use different backup programs and media to cover yourself in interchangeable ways. For instance, you might back up to an external hard drive with one program one day, and over the Internet via a service like Mozy the next.
You should protect immediately important files (as opposed to long-term important ones) as you go, by e-mailing them to yourself--preferably to an account that you access via the Web--as you work. For instance, I just now used Microsoft Word's Mail To feature to e-mail this article to my Gmail account. It will stay on Google's server until I delete it.
Get in the backup habit, and you'll be glad you did. Avoid backing up, and you'll eventually regret it.
Backup terms
The difference between differential and incremental backups
Both differential and incremental backups are backups that save time and backup media, by only backing up changed files. But they differ significantly in how they do it, and how useful the result is.
A full backup created from within Windows, of course, backs up all the files in a partition or on a disk by copying all disk sectors with data to the backup image file. Creating a full backup for unknown or damaged file systems Acronis True Image copies all sectors to the image file, whether or not the sector contains data. This is the simplest form of backup, but it is also the most time-consuming, space-intensive and the least flexible.
Typically full backups are only done once a week and are part of an overall backup plan. Sometimes a full backup is done after a major change of the data on the disk, such as an operating system upgrade or software install. The relatively long intervals between backups mean that if something goes wrong, a lot of data is going to be lost. That's why it is wise to back up data between full backups.
Most of the information on a computer changes very slowly or not at all. This includes the applications themselves, the operating system and even most of the user data. Typically, only a small percentage of the information in a partition or disk changes on a daily, or even a weekly, basis. For that reason, it makes sense only to back up the data that has changed on a daily basis. This is the basis of sophisticated backup strategies.
Differential backups were the next step in the evolution of backup strategies. A differential backup backs up only the files that changed since the last full back. For example, suppose you do a full backup on Sunday. On Monday you back up only the files that changed since Sunday, on Tuesday you back up only the files that changed since Sunday, and so on until the next full backup. Differential backups are quicker than full backups because so much less data is being backed up. But the amount of data being backed up grows with each differential backup until the next full back up. Differential backups are more flexible than full backups, but still unwieldy to do more than about once a day, especially as the next full backup approaches.
Incremental backups also back up only the changed data, but they only back up the data that has changed since the last backup — be it a full or incremental backup. They are sometimes called "differential incremental backups," while differential backups are sometimes called "cumulative incremental backups." Confused yet? Don't be.
If you do an incremental backup on Tuesday, you only back up the data that changed since the incremental backup on Monday. The result is a much smaller, faster backup. The characteristic of incremental backups is the shorter the time interval between backups, the less data to be backed up. In fact, with sophisticated backup software like Acronis True Image, the backups are so small and so fast you can actually back up every hour, or even more frequently, depending on the work you're doing and how important it is to have current backups.
While incremental backups give much greater flexibility and granularity (time between backups), they have the reputation for taking longer to restore because the backup has to be reconstituted from the last full backup and all the incremental backups since. Acronis True Image uses special snapshot technology to rebuild the full image quickly for restoration. This makes incremental backups much more practical for the average enterprise.
Over time the media the archive is stored will need changing. This will apply to both the backup media and the media the primary archive is stored. Hard Drives have a life span of approximately five years. CD/DVD are longer depending on the quality of the CD/DVD. However as the rapid increase in drive sizes means that it may well be more efficient to carry out consolidation of the archive by migrating the archive from smaller drives to fewer larger 

The 3-2-1 Backup rule

This rule is the simplest way to remember one of the best practise rules to backup strategies.
It is recommend keeping 3 copies of any important file (a primary and two backups)
It is recommend having the files on 2 different media types (such as hard drive and optical media), to protect against different types of hazards.
And lastly it is recommended that 1 copy should be stored offsite (or at least offline).
This is a best practice minimum, having more copies is even better but management of large numbers of backups can become unwieldy.  



Archiving an Image archive.
Image Archive can be easily archived as this is built into the Image Archive as a normal function of running the archive. Image Archive supports a full system of documentation to provide an audit of the Archive process and a history of both when previous Archives where carried out and what was archived. This is an important job that any DAM system must be capable of as part of the proper management of the Archive. 
Journal files

Image Archive uses Journal files as the means of documenting the archive process. 
Journal files are files that document what needs to be backed-up or archived and record what has been backed up or archived.
When you request a backup or archive, Image Archive will need to create a journal file. This will contain a list of the volumes required to complete the backup. Then, as the backup progresses, the completed volumes will be marked as complete, when all the volumes are marked complete then the backup is complete. The journal file will then be stored. This Journal file can then be used in the future to provide a history of this backup.
Archive Volumes and Volume sets
An archive volume is a unit of backup media. This may be a tape, a CD, DVD or Hard disk. Historically many backup systems used tape; However CD and DVD are becoming the norm and BlueRay possibly the next cheap non-volatile backup media. Hard Disks may be used but are mechanical and may not be the best solution for long term archiving. Each volume will contain a number of complete partitions.
Archive volume sets are sets of volumes that contain all or part of the archive. A complete set will normally contain the entire archive. As part of the archive process image archive will log archive volume sets. It will log details about the set, such as the number of volumes in the set, the total number of partitions and the total number of images etc. Then log details on each of the volumes and in to the Journal file.

Volume and Volume set labels
Each volume set is given an ID this consists of the date plus a two digit unique 
index. an example would be: 11052301 11 being the year 05 the month, day and finally the id which is 01. Volumes start from zero have three digits, so if you had eleven volumes then the first volume will be 000 and the last being 010. The full volume name will be 11052301-000 and 11052301-010 respectively.

The staging area
the staging area is a temporary space is required to create volumes for backing up. For example, if an archive is several gigabytes in order to backup onto volumes image archive needs a area to create those volumes for archiving. A likely scenario would be that an area is available on a hard drive big enough to create three volumes. You can inform image archive to use that space and it will create the first three volumes in that space. This will then allow you to create say three DVDs which will become three volumes. You can then perform image archive that you have burnt the three volumes and need the next three image archival then provide the next three volumes in the set so you can then burn the next three this can then continue until all of the volumes required by the volume set have been created.
Query the Archive
Querying the archive is a usefull first step, the query command allows you to Query the archive in a number of ways. By using a number of query types The first way and possibly the simplest is the FullQueryType this type provides a full query of the archive.
And will return you a complete list of volumes required to do a complete backup of the archive.
A more likely scenario would be that you have been archiving the archive on a regular basis and need to an incremental backup from the last time an incremental archive was carried out. This can be done by using the fromLastType query. This will return you a list of volumes required to take you from the last time the archive was backed up to the present time. This will then provide you with a complete set of volumes using the volumes previously used in the backup.

Creating the journal file
In order to progress a backup a journal file will first need to be created there are two options for creating The first would be a full journal file which encompasses the complete archive 

The backup process
the backup process is the process by which the archive is backed up into volumes. For this example we will use the fact that the archive will be backed up to DVDs. The first part of the process would be to create a journal file. This will be used as a means to control the progress of the backup. As the backup proceeds more volumes in the journal file will be ticked off as complete until the complete set is finished. The backup process has for commands the controlling how the backup progresses

		StartBackupSet,
		
		NextBackupSet,
		
		CompletedBackupSet,
		
		UndoCompletedBackupSet


Checking and restoring the archive.
To check an archive the CheckArchive command is run. This will return a response containing if the archive is intact or not. If the command finds any inconsistencies a list of effected partitions will be reported so they can be restored from a backup.
 
Temporary  and Permanent lock files
Permanent  lock files are managed by the manifest files so in the event of a lost of the archive the Permanent  lock file will be restored.
Mirror Archives
Mirror archives enable a consistent backup of the primary archive, in addition, enables a read only version of the primary archive.
Command format
Commands
Create
Import
Export
Tag Update
The commands used by IA are passed in an XML file.
Commands can be passed on the command line in the form of:
imgarch [<Command>] [<main xml file path>] [<template xml file>]
Command – Can be any of the following:
Create
Import
Export
TagUpdate
If only the command is passed without an xml file path then a default path is traced. If a default xml file is found then the arguments in the file are used, however if not file found then the command will fail.
 
Xml file path – Is the path to the xml command file used to hold the command and arguments.
Data Verification
One of the primary goals of an archive system  is to keep the archive file data intact. 
  Manifest files
These files are used to maintain the integrity of the archive. Each managed file that is added, modified or deleted in the archive has an entry in a manifest file.
Each entry consists of the following:
Filename – This is the name of the file being managed.
md5 – this is the MD5 signature of the file.
Crc – cyclic redundancy check sum of the file
size – The files size.
Last Modified – Time and date the last time modified (also the time/date when the file was added if that was the last operation carried out on the file.
This information enables the system to check the integrity of each file and restore any file that has become corrupt from a backup.
As the manifest file itself can become corrupt the image manifest files are managed in a partition manifest file, and likewise all the partition manifest files are maintained and managed by a master manifest file.
The Partition manifest file contains and entry for each image manifest containing the same Size, CRC, MD5 and Last Modified Time/Date. 
The Master manifest file then contains all the information about each of the Partition manifest files. This then maintains the integrity of each Partition file. 
The Systems Last Modified file. This file contains the last time the archive was modified with its integrity intact. If files in the archive are updated but if this file is not updated then the updated file(s) will make the archive inconsistent and will need to be rolled back.  
Archive integrity scanning
Image Archive is able to scan the archive for inconsistencies. The process is as follows:
First find any inconsistencies in the image manifest files.
Read the systems Last Update file
 Each managed image entry in the archive is accessed in turn.
Each image is processed as follows:

The image read and the size, CRC and MD5 sum is calculated and the Last Modified Time/Date read.
This is compired with

Migrating the Archive
		Over time the media the archive is stored will need changing. This will apply to both the backup media and the media the primary archive is stored. Hard Drives have a life span of aproxamatly five years. CD/DVD are longer depending on the qualty of the CD/DVD. However as the rapid increase in drive sizes means that it may well be more efficient to carry out consolidation of the archive by migrating the archive from smaller drives to fewer larger
