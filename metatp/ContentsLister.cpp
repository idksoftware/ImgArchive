#include "ContentsLister.h"
#include "ArchiveVisitor.h"

namespace simplearchive {
	ContentsLister::ContentsLister(const char *shadowPath)
	{
		m_shadowPath = shadowPath;
	}


	ContentsLister::~ContentsLister()
	{
	}

	class ListerAction : public AVAction {

	protected:

		virtual void onStart() {
			printf("onStart\n");
		};
		/// At the end of each directory found, this function is run.
		virtual void onEnd() {
			printf("onEnd\n");
		};
		/// On finding a file, this function is run.
		virtual void onYearFolder(const char *name) {
			printf("onYearFolder %s: \n", name);
		};
		/// On finding a file, this function is run.
		virtual void onYearEnd() {
			printf("onYearEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onDayFolder(const char *name) {
			printf("onDayFolder %s: \n", name);
		};
		/// On finding a directory, this function is run.
		virtual void onDayEnd() {
			printf("onDayEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onImage(const char *path, const char *name) {
			printf("\t\tonImage %s: \n", path);
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
		ArchiveVisitor archiveVisitor(new ListerAction(), m_shadowPath.c_str());
		archiveVisitor.archive();
		return true;
	}

}; // namespace