administrative housekeeping

1. 	Housekeeping Commands

2.	Mirror commands

3.	Archive validation commands

4.	Backup commands

In addition there are a number of global arguments that control how the SIA program executes. I.e. changing the logging level, changing command terminal options etc. 
Many of the command line options can be defaulted using environment variables or the main configuration file. See environment variables and   configuration file
While SIA has different options for its subcommands however all options have the same meaning if used in different subcommands. For example –verbose (-v) always means verbose output to the control terminal regardless of the subcommand being used.
SIA will normality exit quickly with an error if it is passed an option that does not apply to the subcommand  

Housekeeping Commands

Mirror commands (--mirror)
These commands control the image mirroring functions. The default configuration is normal maintained by using configuration files as this function is carried out as part of an automatic operation.
--sync
--verify
--all
Archive validation commands (--validate)
This command helps to maintain the integrity of the mirror by scanning each file in the archive and comparing it with a list of checksums that validate that the files are not corrupted or missing. Because the archive contains images that may not be accessed for some time images, sometimes months, even years, these image can go missing or get corrupted without you knowing for some time. This can be done by accident or maliciously. However the damage is done and the longer left untreated the more likely that the repair will be harder as the backup may be harder to find. The process of validating the archive may take some time to scan the entire archive so this command allows you to be selective of what is to be scanned.   
--all
--archive
--view
--mirror
Backup commands (--backup)

Global Commands

Quiet
-q –Quiet
Verbose 
-v –Verbose
Logging Level
-l –logginglevel=<Value> this will temporarily change the logging level for the scope of this command session.
Dry run
Goes through the motions of running the subcommand but makes no actual changes ether disk or repository. It does create entries in the log file indicating what would have happened if running normally. This is used for dry running command before committing to doing it for real.
-n --dry-run 
Archive Folder
This provides SIA with the full path to the root folder of the current archive repository.
-a –archivefolder=<value>
<value> is the full path to the root folder of the archive repository that the images are to be archived
Example:
-- archivefolder=”/home/photos/repository”
Source Folder
This contains the full path of where the images to be import can be found. By default all sub folders will also searched for images.
-s –sourcefolder=<value>
<value> is the full path to the root folder of the source of images to be imported.

