
#include "siaglobal.h"
#include "TerminalServer.h"
#include "SIAApplicationState.h"
#include "CLogger.h"

namespace simplearchive {

	SIAApplicationState::RunState SIAApplicationState::m_runState = SIAApplicationState::Running;

	SIAApplicationState::RunState SIAApplicationState::getState() {
		
		CTerminalServer::State state = CTerminalServer::getState();

		CLogger &logger = CLogger::getLogger();

		switch (state) {
		case CTerminalServer::Running:
			if (m_runState != SIAApplicationState::Running) {
				logger.log(LOG_OK, CLogger::Level::INFO, "Changed run state to Running");
			}
			return SIAApplicationState::Running;
		case CTerminalServer::Paused:
			if (SIAApplicationState::Stopped) {
				return SIAApplicationState::Stopped;
			}
			if (m_runState != SIAApplicationState::Paused) {
				logger.log(LOG_OK, CLogger::Level::INFO, "Changed run state to Paused");
			}
			return SIAApplicationState::Paused;
		case CTerminalServer::Stopped:
			if (m_runState != SIAApplicationState::Stopped) {
				logger.log(LOG_OK, CLogger::Level::INFO, "Changed run state to Stopped");
			}
			return SIAApplicationState::Stopped;
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Changed run state to Unknown");
		return SIAApplicationState::Unknown;
	}

}
