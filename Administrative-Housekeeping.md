#administrative housekeeping

1. 	Housekeeping Commands

2.	Mirror commands

3.	Archive validation commands

4.	Backup commands

In addition there are a number of global arguments that control how the SIA program executes. I.e. changing the logging level, changing command terminal options etc. 
Many of the command line options can be defaulted using environment variables or the main configuration file.

See environment variables and configuration file

While SIA has different options for its subcommands however all options have the same meaning if used in different subcommands. For example –verbose (-v) always means verbose output to the control terminal regardless of the subcommand being used.
SIA will normality exit quickly with an error if it is passed an option that does not apply to the subcommand  

##Housekeeping Commands

###initialise command (init)
The initialise command (init) initialises the environment in order for SIA to operate correctly. This entails creating required folders in locations accessible to SIA and populating these folders with operational files such as the main configuration file. SIA will locate these files by a environment variable SIA_HOME or default locations specified by the OS.

This command takes four arguments. The first specifies the installed user or users. This is common to almost all OS's that an application can install for one user or all uses who can login to the computer running the application.
`--users=<Myself|All>`

however, to install an application for all uses who can login to the computer normally requires elevated privileges i.e. root privileges under UNIX based systems (Linux and OSX) and administer privileges under windows. The preferred option it install in elevated privileges because normally the application is run under normal user privilege which protects system files against modification by users. Unfortunately sometimes the root or administer passwords are not available so the application as to be installed for the current logged in user.

if the --users option is not present then the installation will default to "all users" and elevated privileges will be required and will fail to install if the privilege are not found    

The other arguments relate to the location of archive components. these components are as follows:

* The archive repository i.e. where the archive are located (--shadow-path).

* The system environment i.e. where the system files are located (--archive-path). 

* The workspace i.e. where the working copies of images are located (workspace-path).

#### The archive Location
The location of the archive can be specified individually and not part of there the configuration is located as the archive can be quite large. Therefore this can be specified as a separate augment on the command line. If these is not supplied then the default will be to add it as a folder in the configuration.

#### The system environment location
This is the home location of the archive. All the core configuration will be in folders located as sub-folders from this home folder. If this augment is used, and the location used for the configuration is not one of the OS default locations then SIA_HOME environment variable will need to be created to contain the path to this location. 
By using the siaadmin tool this environment variable will be managed for you.

#### The workspace location
This is where all the working copies of images are located. This folder can also become quite large so it is specified as a specified individually. In addition this will need to be a folder that you have permission to modify as the current user.

###Show configuration command (show)
This command will display the current configuration settings to the screen. This command helps you view the archives configuration settings as the archive reads them. Any problems with the configuration can hopefully be corrected by inspecting the output from this command.  

##Mirror commands (--mirror)
These commands control the image mirroring functions. The default configuration is normal maintained by using configuration files as this function is carried out as part of an automatic operation.

--sync

--verify

--all

##Archive validation commands (--validate)

This command helps to maintain the integrity of the mirror by scanning each file in the archive and comparing it with a list of checksums that validate that the files are not corrupted or missing. Because the archive contains images that may not be accessed for some time images, sometimes months, even years, these image can go missing or get corrupted without you knowing for some time. This can be done by accident or maliciously. However the damage is done and the longer left untreated the more likely that the repair will be harder as the backup may be harder to find. The process of validating the archive may take some time to scan the entire archive so this command allows you to be selective of what is to be scanned.
   
--all
--archive
--view
--mirror

##Backup commands (--backup)

##Miscellaneous Commands
Version Command (version)
This command displays the current version and build of the siaadmin software. This is useful if you wish to make sure the software you are using is the most up-to-date. 

##Global Arguments

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

config.setHomePath(homePath.c_str());
	std::string temp;
	temp = SAUtils::GetEnvironment("SIA_ARCHIVE");
	if (temp.empty() == false) {
		config.setWorkspacePath(temp.c_str());
	}
	temp = SAUtils::GetEnvironment("SIA_SOURCE");
	if (temp.empty() == false) {
		config.setSourcePath(temp.c_str());
	}
	temp = SAUtils::GetEnvironment("SIA_LOGLEVEL");
	if (temp.empty() == false) {
		config.setLogLevel(temp.c_str());
	}

