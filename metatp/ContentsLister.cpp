#include "ContentsLister.h"
#include "ArchiveVisitor.h"
#include "SAUtils.h"

namespace simplearchive {
	ContentsLister::ContentsLister(const char *MasterPath)
	{
		m_MasterPath = MasterPath;
	}


	ContentsLister::~ContentsLister()
	{
	}

	class ListerAction : public AVAction {

	protected:

		virtual void onStart() {
			DEBUG_PRINT("onStart\n");
		};
		/// At the end of each directory found, this function is run.
		virtual void onEnd() {
			DEBUG_PRINT("onEnd\n");
		};
		/// On finding a file, this function is run.
		virtual void onYearFolder(const char *name) {
			DEBUG_PRINT("onYearFolder %s: \n", name);
		};
		/// On finding a file, this function is run.
		virtual void onYearEnd() {
			DEBUG_PRINT("onYearEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onDayFolder(const char *name) {
			DEBUG_PRINT("onDayFolder %s: \n", name);
		};
		/// On finding a directory, this function is run.
		virtual void onDayEnd() {
			DEBUG_PRINT("onDayEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onImage(const char *path, const char *name) {
			DEBUG_PRINT("\t\tonImage %s: \n", path);
		};

		/// This function is a factory function used to create new FolderVisitor objects.
		//virtual AVAction *make() { return new CatalogAction; }
	public:

	
		/// Constructor
		ListerAction() {};
		/// Distructor
		virtual ~ListerAction() {
		}

	};





	bool ContentsLister::List() {
		ArchiveVisitor archiveVisitor(new ListerAction(), m_MasterPath.c_str());
		archiveVisitor.archive();
		return true;
	}

}; // namespace