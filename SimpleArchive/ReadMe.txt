========================================================================
    CONSOLE APPLICATION : SimpleArchive Project Overview
========================================================================


AppWizard has created this SimpleArchive application for you.

This file contains a summary of what you will find in each of the files that
make up your SimpleArchive application.

SimpleArchive.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

SimpleArchive.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

SimpleArchive.cpp
    This is the main application source file.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named SimpleArchive.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

add --source-path="D:\tmp_pics"

checkin --image-address="2017-01-16/DSC01278.JPG" --comment "A comment"
checkin --force --image-address="2017-01-16/DSC01278.JPG" --comment "A comment"

checkout --image-address="2017-01-19/DSC01544.JPG" --comment "A comment"

checkout --image-address="2017-01-19/DSC01544.JPG" --comment "A comment"

checkout --image-address="2017-01-16/DSC01279.JPG" --comment "A comment"

uncheckout --image-address="2017-01-16/DSC01279.JPG" --comment "A comment"

checkout --force --image-address="2017-01-16/DSC01278.JPG" 

status --scope="2017/01/16-2017/01/18"

add --source-path="D:\tmp"

checkin --scope 2017

uncheckout --scope 2017

add --source-path="D:\tmp_pics"

add --source-path="C:\tmp_raw"

get --scope=2017


checkin --scope 2017-*-*/DSC01277.JPG

add --source-path="C:\local\Lightroom\2019"

add --lightroom

mode --remote-server

