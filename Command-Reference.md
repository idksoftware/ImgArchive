#Command Reference
This contains the command reference for the general operation of sia. For the administration of the archive you will need the [administrative housekeeping](Administrative Housekeeping) reference.

[Import](Import) adds new images to the archive.", 

[Exporting](Exporting) get images out of the archive.", 

[Checking in](Checking in) Check in images from archive

[Checking out](Checking out) Check out images from archive

[Un-checking out](Un checking out) Undo checking out of images from archive

[version](version) prints the build and version of the SIA software.

[show](show) Show details

[propert](prop) Show details



`
m_argvParser->defineOption("add", "add new images to the archive.", ArgvParser::MasterOption); 
202 	m_argvParser->defineOption("checkout", "Checkout images from archive.", ArgvParser::MasterOption); 
203 	m_argvParser->defineOption("checkin", "Checkin images to archive.", ArgvParser::MasterOption); 
204 	m_argvParser->defineOption("uncheckout", "Un-checkout images to archive.", ArgvParser::MasterOption); 
205 	m_argvParser->defineOption("export", "Export images from archive.", ArgvParser::MasterOption); 
206 	m_argvParser->defineOption("about", "prints the version information", ArgvParser::MasterOption); 
207 	m_argvParser->defineOption("view", "View commands", ArgvParser::MasterOption); 
208 	m_argvParser->defineOption("show", "Show details", ArgvParser::MasterOption); 
209 	m_argvParser->defineOption("prop", "Manage image properties", ArgvParser::MasterOption); 
`
