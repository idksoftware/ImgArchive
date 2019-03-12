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

#define SIA_ERRORS_START 5000




#define SIA_ERRORS_DEF \
	SIA_ERROR_DEF( SUCCESS,							"Success" ) \
	SIA_ERROR_DEF( ALREADY_CHECKED_OUT,				"Already checked out" ) \
	SIA_ERROR_DEF( ALREADY_CHECKED_IN,				"Already checked in") \
	SIA_ERROR_DEF( ALREADY_CHECKED_IN_CHANGES,		"Already checked in, changes found") \
	SIA_ERROR_DEF( ALREADY_CHECKED_IN_NO_CHANGES,	"Already checked in, no changes found") \
	SIA_ERROR_DEF( NO_CHANGE_IN_IMAGE,				"No changes found in image") \
	SIA_ERROR_DEF( CHANGE_MAY_BE_LOST,				"Changes may be lost by checkout") \
	SIA_ERROR_DEF( IMAGE_INDEXING_CORRUPT,			"Image indexing corrupt") \
	SIA_ERROR_DEF( NOT_BEEN_CHECKED_OUT,			"Not been checked out") \
	SIA_ERROR_DEF( DUPLICATE_IMAGE,					"Duplicate image found") \
	SIA_ERROR_DEF( UNABLE_TO_SAVE_JOUNAL,			"Unable to save Journal File") \
	SIA_ERROR_DEF( PERMISSION_ERROR,				"Permission Error" ) \
	SIA_ERROR_DEF( NOT_IN_MAINENANCE_MODE,			"Not In Maintenance Mode" ) \
	SIA_ERROR_DEF( TIMESTAMP_NOT_FOUND,				"Timestamp Not Found" ) \
	SIA_ERROR_DEF( ERROR,							"Error" ) \
	SIA_ERROR_DEF( SETTING_ALREADY_EXISTS,			"Setting Already Exists" ) \
	SIA_ERROR_DEF( WILL_OVERWRITE_CHANGES,			"will overwrite changes" ) \
	SIA_ERROR_DEF( FILE_NOT_FOUND,					"File not found") \
	SIA_ERROR_DEF( FOLDER_FOUND,					"Folder found") \
	SIA_ERROR_DEF( IMAGE_NOT_FOUND,					"Image not found") \
	SIA_ERROR_DEF( INVALID_PATH,					"Invalid path") \
	SIA_ERROR_DEF( TARGET_INVALID_PATH,				"Target invalid path") \
	SIA_ERROR_DEF( TARGET_NOT_FOUND,				"Target not found") \
	SIA_ERROR_DEF( NO_IMAGE,						"No image") \
	SIA_ERROR_DEF( OPEN_ERROR,						"Error Opening file" ) \
	SIA_ERROR_DEF( READ_ERROR,						"Error reading file" ) \
	SIA_ERROR_DEF( WRITE_ERROR,						"Error writing file" ) \
	SIA_ERROR_DEF( XML_WRITE_ERROR,					"XML write error" ) \
	SIA_ERROR_DEF( NUM_CONFIG_RESPONSES,			"Unused" ) \
	SIA_ERROR_DEF( TYPE_MISMATCH,					"Type mismatch" ) \
	SIA_ERROR_DEF( ERROR_ADDING_HISTORY,			"Error adding history record" ) \
	SIA_ERROR_DEF( INVALID_IMAGE_TYPE,				"Error invalid image type" ) \
	SIA_ERROR_DEF( ROW_SCHEMA_MISMATCH,				"Row schema mismatch" ) \
	SIA_ERROR_DEF( WORKSPACE_NOT_FOUND,				"Workspace not found")


	/**
	* Cofiguration Manager enumeration - constructed automatically from above macro definition so that list is only declared once (once for declaration and once for operator)
	*/
#define SIA_ERROR_DEF( identifier, name )  identifier, 
	enum class SIA_ERROR { SIA_ERRORS_DEF };
#undef SIA_ERROR_DEF

	class ErrorCode {
		static const int errorOffset;
		static SIA_ERROR m_errorCode;
	public:
		ErrorCode();
		virtual ~ErrorCode();
		static const SIA_ERROR getErrorCode();
		static void setErrorCode(SIA_ERROR errorCode);
		static const char *toString(SIA_ERROR type);
		static const int toInt(SIA_ERROR type);
	};

	std::ostream& operator<<(std::ostream& out, const SIA_ERROR value);

} /* namespace simplearchive */


