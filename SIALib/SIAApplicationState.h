#pragma once

#include <string>
#include <stdio.h>
#include <stdlib.h>


namespace simplearchive {

	class SIAConnection;
	class SIAApplicationState {
	public:
		typedef enum {
			Running,
			Paused,
			Stopped,
			Unknown
		} RunState;
	private:
		friend class SIAConnection;
		static RunState m_runState;
		static std::string m_buffer;
	public:
		static RunState getState();
	};

}