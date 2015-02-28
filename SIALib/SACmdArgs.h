#ifndef SACMDARGS_H_
#define SACMDARGS_H_

namespace simplearchive {

	class SACmdArgs
	{
		static bool m_isMakeNoChanges;
		static SACmdArgs *m_this;
	public:

		bool IsMakeNoChanges() {
			return m_isMakeNoChanges;
		}

		SACmdArgs();
		~SACmdArgs();
		static SACmdArgs &get();
	};

}
#endif // SACMDARGS_H_