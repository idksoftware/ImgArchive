ImgArchive
==========

Beta release 12/10/2020

This software installs as a command line tools only. These are:

iaarc.exe - The ImgArchive client tool. This provides the means to operate ImgArchive.
iaadmin.exe - The ImgArchive administration Tool. This provides configuration and administration house
		keeping support for ImgArchive.

Quick install instructions
==========================

To run the software you will need to open a new command
 prompt (cmd.exe).

Note: For more information please goto https://idk-solutions.atlassian.net/wiki/spaces/ImgaWiki/overview 

To run the ImgArchive client tool
---------------------------------

type:

c:\iaarc.exe<cr>

If installed correctly you will see the following help message:

iaarc - Image archive client provides the means to operate ImgArchive from the
command line

Note:
The administration of the archive is carried out by the "iaadmin" command-line
interface.


These are common ImgArchive commands used in various situations:
.......



To run the ImgArchive administration tool
-----------------------------------------

type:

c:\iaadmin.exe<cr>

If installed correctly you will see the following help message:

iaadmin Tool provides administration house keeping support for ImgArchive.

usage: iaadmin subcommand [options] [args]
ImgArchive command line administrator, version 1.0.0.1
Type 'iaadmin help <subcommand>' for help on a specific subcommand.
iaadmin is the primary command-line interface to administer ImgArchive.

These are common ImgArchive commands used in various situations:

.......


Checking the installation
-------------------------

The iaadmin Tool provides a show sub-command to display the current statue of the installation. It
Use this command type the following:

C:\Users\iferg>iaadmin.exe show --setting=folders

The output from this will be the following:

Folders
=======
ImaArchive Home Location
  IMGARCHIVE_HOME using System Environment at loacation:
        C:\ProgramData\IDK-Software\ImgArchive

Application paths
  Configuration path:        C:\ProgramData\IDK-Software\ImgArchive/config
  System path:               C:\ProgramData\IDK-Software\ImgArchive/system
  Log path:                  C:\ProgramData\IDK-Software\ImgArchive/logs
  Tools path:                C:\ProgramData\IDK-Software\ImgArchive/tools
  Hook path:                 C:\ProgramData\IDK-Software\ImgArchive/hooks
  History path:              C:\ProgramData\IDK-Software\ImgArchive/history
  SQL Database path:         C:\ProgramData\IDK-Software\ImgArchive/sqldb
  Templates path:            C:\ProgramData\IDK-Software\ImgArchive/templates
  Duplicates path:           C:\ProgramData\IDK-Software\ImgArchive/dups

This display's the location of the of there ImgArchive is located.

To display where the primary repositories are located we need two other settings to be used. These are
the following:

Type:

C:\Users\iferg>iaadmin.exe show --setting=master

to display the master repository settings. This will be displayed in the format below:

Master Archive
==============
  Master Archive Location
        C:\ProgramData\IDK-Software\ImgArchive/master

  Master Archive Backups
        Backup One Enabled:        False
        Path not Valid:            C:\ProgramData\IDK-Software\ImgArchive/backup/MasterBackup1
        Backup Two Enabled:        False
        Path not Valid:            C:\ProgramData\IDK-Software\ImgArchive/backup/MasterBackup2

And Type:

C:\Users\iferg>iaadmin.exe show --setting=derivative

to display the derivative repository settings. This will be displayed in the format below:

Derivative Archive
==============
  Derivative Archive Location
        C:\ProgramData\IDK-Software\ImgArchive/derivative

  Derivative Archive Backups
        Backup One Enabled:        False
        Path not Valid:            C:\ProgramData\IDK-Software\ImgArchive/backup/DerivativeBackup1
        Backup Two Enabled:        False
        Path not Valid:            C:\ProgramData\IDK-Software\ImgArchive/backup/DerivativeBackup2

One further setting you may needs is the location of the working folders. These being

The Workspace, the picture catalogue and the Web picture catalogue.

For this installation we will only check the location of the workspace. This can be done by typing the following:

C:\Users\iferg>iaadmin.exe show --setting=workspace

As you can see below by default the workspace will automaticaly get newly imported images into the workspace.
However this mages will be set to read-only.
To edit an image you will need to check the image out of the archive. See http://imgawiki.org to perform that action. 

Workspace
=========
  Workspace Location
        C:\Users\iferg\ImgArchive\Workspace
      Auto view: On
  Auto checkout: Off

Finally
=======

To add images into the archive you will need to import them. The simplist way is just to point iaarc.exe to the location
of the images to be imported by the following:

iaarc.exe import --source-path=<path to images to be imported>

for example:

iaarc.exe import --source-path=C:\Pictures

Where C:\Pictures is the folder with the images to be imported.

iaarc.exe will display the status of the import and will end with sometime like the folllowing:

[   SUMMARY ]   Stage 4: Validating images completed
[   SUMMARY ]   Imported 4 image files found in 3 Folder(s) into the archive, 0 image files rejected
[   SUMMARY ]   Application completed successfully at Wed Oct 14 12:34:33 2020

This will mean 4 image files found and imported.

If you now find the location of the workspace the 4 images will be contained in the workspace under the date created or shot.