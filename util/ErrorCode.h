#pragma once
/*
* ErrorCode.cpp
*
*/

#include <memory>
#include <string>
#include <cstdint>

/*
** The ALWAYS and NEVER macros surround boolean expressions which
** are intended to always be true or false, respectively.  Such
** expressions could be omitted from the code completely.  But they
** are included in a few cases in order to enhance the resilience
** of SQLite to unexpected behavior - to make the code "self-healing"
** or "ductile" rather than being "brittle" and crashing at the first
** hint of unplanned behavior.
**
** In other words, ALWAYS and NEVER are added for defensive code.
**
** When doing coverage testing ALWAYS and NEVER are hard-coded to
** be true and false so that the unreachable code they specify will
** not be counted as untested code.
*/
#if defined(SIA_COVERAGE_TEST)
# define ALWAYS(X)      (1)
# define NEVER(X)       (0)
#elif !defined(NDEBUG)
# define ALWAYS(X)      ((X)?1:(assert(0),0))
# define NEVER(X)       ((X)?(assert(0),1):0)
#else
# define ALWAYS(X)      (X)
# define NEVER(X)       (X)
#endif

namespace simplearchive {

#define IMGA_ERRORS_START 5000




#define IMGA_ERRORS_DEF \
	IMGA_ERROR_DEF( SUCCESS,							"Success" ) \
	IMGA_ERROR_DEF( ALREADY_CHECKED_OUT,				"Already checked out" ) \
	IMGA_ERROR_DEF( ALREADY_CHECKED_IN,				"Already checked in") \
	IMGA_ERROR_DEF( ALREADY_CHECKED_IN_CHANGES,		"Already checked in, changes found") \
	IMGA_ERROR_DEF( ALREADY_CHECKED_IN_NO_CHANGES,	"Already checked in, no changes found") \
	IMGA_ERROR_DEF( NO_CHANGE_IN_IMAGE,				"No changes found in image") \
	IMGA_ERROR_DEF( CHANGE_MAY_BE_LOST,				"Changes may be lost by checkout") \
	IMGA_ERROR_DEF( IMAGE_INDEXING_CORRUPT,			"Image indexing corrupt") \
	IMGA_ERROR_DEF( NOT_BEEN_CHECKED_OUT,			"Not been checked out") \
	IMGA_ERROR_DEF( DUPLICATE_IMAGE,					"Duplicate image found") \
	IMGA_ERROR_DEF( UNABLE_TO_SAVE_JOUNAL,			"Unable to save Journal File") \
	IMGA_ERROR_DEF( PERMISSION_ERROR,				"Permission Error" ) \
	IMGA_ERROR_DEF( NOT_IN_MAINENANCE_MODE,			"Not In Maintenance Mode" ) \
	IMGA_ERROR_DEF( TIMESTAMP_NOT_FOUND,				"Timestamp Not Found" ) \
	IMGA_ERROR_DEF( ERROR,							"Error" ) \
	IMGA_ERROR_DEF( SETTING_ALREADY_EXISTS,			"Setting Already Exists" ) \
	IMGA_ERROR_DEF( WILL_OVERWRITE_CHANGES,			"will overwrite changes" ) \
	IMGA_ERROR_DEF( FILE_NOT_FOUND,					"File not found") \
	IMGA_ERROR_DEF( FOLDER_FOUND,					"Folder found") \
	IMGA_ERROR_DEF( IMAGE_NOT_FOUND,					"Image not found") \
	IMGA_ERROR_DEF( INVALID_PATH,					"Invalid path") \
	IMGA_ERROR_DEF( TARGET_INVALID_PATH,				"Target invalid path") \
	IMGA_ERROR_DEF( TARGET_NOT_FOUND,				"Target not found") \
	IMGA_ERROR_DEF( NO_IMAGE,						"No image") \
	IMGA_ERROR_DEF( OPEN_ERROR,						"Error Opening file" ) \
	IMGA_ERROR_DEF( READ_ERROR,						"Error reading file" ) \
	IMGA_ERROR_DEF( WRITE_ERROR,						"Error writing file" ) \
	IMGA_ERROR_DEF( XML_WRITE_ERROR,					"XML write error" ) \
	IMGA_ERROR_DEF( NUM_CONFIG_RESPONSES,			"Unused" ) \
	IMGA_ERROR_DEF( TYPE_MISMATCH,					"Type mismatch" ) \
	IMGA_ERROR_DEF( ERROR_ADDING_HISTORY,			"Error adding history record" ) \
	IMGA_ERROR_DEF( INVALID_IMAGE_TYPE,				"Error invalid image type" ) \
	IMGA_ERROR_DEF( ROW_SCHEMA_MISMATCH,				"Row schema mismatch" ) \
	IMGA_ERROR_DEF( WORKSPACE_NOT_FOUND,				"Workspace not found")


	/**
	* Cofiguration Manager enumeration - constructed automatically from above macro definition so that list is only declared once (once for declaration and once for operator)
	*/
#define IMGA_ERROR_DEF( identifier, name )  identifier, 
	enum class IMGA_ERROR { IMGA_ERRORS_DEF };
#undef IMGA_ERROR_DEF

	class ErrorCode {
		static const int errorOffset;
		static IMGA_ERROR m_errorCode;
	public:
		ErrorCode();
		virtual ~ErrorCode();
		static const IMGA_ERROR getErrorCode();
		static void setErrorCode(IMGA_ERROR errorCode);
		static const char *toString(IMGA_ERROR type);
		static const int toInt(IMGA_ERROR type);
	};

	std::ostream& operator<<(std::ostream& out, const IMGA_ERROR value);

} /* namespace simplearchive */


