#Installation
Initial installation is simple because SIA consists of two executables called:
* sia.exe - The main archive management tool
* siaadim.exe - This supports the archive by providing housekeeping functions. 


###Windows
Under windows, this application uses Microsoft’s redistributable packages for Visual Studio 2013. These packages need to be installed before sia.exe can be used. However a MSI install package “sia.msi”  this contains all the required packages and installs the application in the default locations.
###Apple Mac
Under the Apple Mac SIA can be installed using Apple Install package sia.pic. This will install SIA into the default locations. 
###Linux
Under the Apple Mac SIA can be installed using Apple Install package sia.pic. This will install SIA into the default locations. 

##Standard OS platform installation locations
SIA is normally installed in a default location defined for the OS platform it is running under. Each OS platform has a default location for third party applications. The standard locations or each of the three main platforms are described as follows:
###Windows
Windows uses the environment variables; ProgramData and ProgramFiles. These locate the standard locations of the program data and the program files. SIA extends these to make a SIA specified folder path. For example the standard location under windows for SIA to install is self is $(ProgramData)\IDK Software\sia1.1 for the data files needed to run SIA and $(DrogramFiles)\IDK Software\sia1.1\bin

_Note_ the windows environment variable ProgramData will normally be defined as:
ProgramData=C:\ProgramData
The program files may look like this:
ProgramFiles=C:\Program Files

SIA will search these two paths looking for the SIA installed files. SIA_HOME may contain the windows data file plus the SIA specified path i.e.:
SIA_HOME=”c:\ProgramData\IDK Software\sia1.1”
Or can be left blank.
The standard location for windows binaries is C:\Program Files
###Apple Mac
Under the Apple Mac SIA can be installed using Apple Install package sia.pic. This will install SIA into the default locations. 
###Linux
Under the Apple Mac SIA can be installed using Apple Install package sia.pic. This will install SIA into the default locations
 

