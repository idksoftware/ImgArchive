
Appendix A.	Annotation Mapping 
Expression Media supports a set of pre-defined fields for annotating your media files. This set (also known as the IPTC/XMP Core) enables you to define metadata for your media files, such as Creator, Date Created, Location, and Copyright. These annotation standards have been defined by the International Press and Telecommunications Council (IPTC) and the Newspaper Association of America (NAA). Adobe uses this standard, in part to define the XMP metadata standard. Expression Media also supports metadata defined by Apple QuickTime. To simplify annotating media, and prevent multiple sets of annotations for each standard, Expression Media has mapped the QuickTime annotation fields to the IPTC fields. The following table lists the field names used by Expression Media, along with their corresponding names in the IPTC and QuickTime standards, as well as in Photoshop. Because the names of each annotation are not integrated into the data, other applications might refer to an annotation by a different name. In Expression Media, you can choose which field names to display in the Info and Organize panels. In the General Preferences dialog box, choose from either the official IPTC Core names, those names used in Expression Media or those used in Photoshop. 

Expression Media 
Photoshop CS & CS2 
IPTC/XMP Core 
Aperture 
Label 
Urgency 
Label 
- 
Title 
Headline 
Headline 
Headline 
Product 
Document Title 
Title 
Object Name 
Genre 
Category 
People 
Category 
Intellectual Genre 
Intellectual Genre 
Intellectual Genre 
- 
Event 
Event + 
Event 
Fixture Identifier 
Event Date 
Date Created 
Date Created 
Date Created 
Author 
Author 
Creator 
Byline 
Credit 
Credit 
Provider 
Credit 
Source 
Source 
Source 
Source 
Copyright 
Copyright 
Copyright 
Copyright 
Transmission 
Transmission Reference 
Job Identifier 
Original Transmission Reference 




Rights Usage Terms 
Rights Usage Terms 
Rights Usage Terms 
-
- 
URL 
Copyright Info URL 
URL 
-
- 
Location 
Location 
Location 
Sublocation 


Sublocation 
City 
City 
City 
City


City 
State 
State/Province 
State/Province 
State/Province 
State/Province 
Country 
Country 
Country 

Country / PrimaryLocationName 
ISO Country Code 
ISO Country Code 
ISO Country Code 

- 
Instructions 
Instructions 
Instructions 

Special Instructions 
Status 
Status + 
Status 

Edit Status 
Writer 
Description Writer 
Description Writer 

Writer/Editor 
People * 
People + 
People * 

Contact 
Categories * 
Supplemental Categories * 
Supplemental Categories * 

Supplemental Category 
Keywords * 
Keywords * 
Keyword * 

Keywords 
Scenes * 
Scenes * 
Scenes * 

- 
Subject Codes * 
Subject Codes * 
Subject Codes * 

- 
Caption 
Description 
Description 

Caption IPTC annotation definitions

Below are the official definitions for IPTC Core annotation fields. For additional information about the IPTC metadata standard, refer to the IPTC Web site.
Headline
A publishable entry providing a synopsis of the contents of the item. Headline is not the same as Title.

Title A shorthand reference for the item. While a technical identifier goes to an identifier element, Title holds a short verbal and human readable name. Title is not the same as Headline.

Intellectual Genre Describes the nature, intellectual, or journalistic characteristic of an item, not specifically its content. Microsoft Expression Media User Guide 159 
Date Created Designates the date and optionally the time the intellectual content of the item was created rather than the date of the creation of the physical representation. If no time is given, the value should default to 00:00:00.
Creator Contains preferably the name of the person who created the content of this item, a photographer for photographs, a graphic artist for graphics, or a writer for textual news. If it is not appropriate to add the name of a person, the name of a company or organization could be applied as well.
Provider Identifies the provider of the item, who is not necessarily the owner/creator.
Source Identifies the original owner of the copyright for the intellectual content of the item. This could be an agency, a member of an agency, or an individual. Source could be different from Creator and from the entities in the Copyright.
Copyright Notice Contains any necessary copyright notice for claiming the intellectual property for this item and should identify the current owner of the copyright for the item. Other entities, such as the creator of the item may be added. Notes on usage rights should be provided in ?Rights Usage Terms
Job Identifier Number or identifier for the purpose of improved workflow handling. This identifier should be added by the creator or provider for transmission and routing purposes only, and should have no significance for archiving.
Rights Usage Terms Free text instructions on how this item can be used legally.
Location Name of a location on which the content is focusing–either the location shown in visual media or referenced by text or audio media. This location name could either be the name of a sub-location of a city or the name of a well known location or (natural) monument outside a city. In the sense of a sub-location to a city, this element is at the fourth level of a top-down geographical hierarchy. 
City Name of the city on which the content is focusing on–either the place shown in visual media or referenced by text or audio media. This element is at the third level of a top-down geographical hierarchy.
 Province/State Name of the subregion of a country–either called province or state–on which is the content focusing–either the subregion shown in visual media or referenced by text or audio media. This element is at the second level of a top-down geographical hierarchy. Country Full name of the country on which the content is focusing–either the country shown in visual media or referenced in text or audio media. This element is at the top/first level of a top-down geographical hierarchy. The full name should be expressed as a verbal name and not as a code. A code should go to the element ?Country Code.? 
Country Code Code of the country on which the content is focused–either the country shown in visual media or referenced in text or audio media. This element is at the top/first level of a top-160 
down geographical hierarchy. The code should be taken from ISO 3166 two or three letter code. The full name of a country should go to the ?Country? element. 
Instructions Any of a number of instructions from the provider or creator to the receiver of the news object that might include any of the following: embargoes (NewsMagazines OUT) and other restrictions not covered by the ?Rights Usage Terms? field; information regarding the original means of capture (scanning notes, colourspace info) or other specific text information that the user might need for accurate reproduction; additional permissions or credits required when publishing. Description writer Identifier or the name of the person involved in writing, editing, or correcting the description of the item.
 Keywords Keywords to express the subject of the content. Keywords may be free text and do not need to be taken from a controlled vocabulary. Values from the controlled vocabulary IPTC Subject Codes must go to the ?Subject Code? element. Scenes Describes the scene of a photo content. Specifies one or more terms from the IPTC ?Scene-NewsCodes.? Each Scene is represented as a string of 6 digits in an unordered list. 
Subject Codes Specifies one or more Subjects from the IPTC ?Subject-News Codes? taxonomy to categorize the content. Each Subject is represented as a string of 8 digits in an unordered list. Description A textual description, including captions, of the item’s content, particularly used where the object is not text.
