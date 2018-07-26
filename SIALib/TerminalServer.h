#pragma once

#include <string>
#include "ipsock.h"

class CIPCommsThreadFunctor;
class CTerminalServer
{
public:
	typedef enum {
		Running,
		Paused,
		Stopped,
		Unknown
	} State;
private:
	CIPServer m_Server;
	
	friend CIPCommsThreadFunctor;
	static State m_state;
	static std::string m_buffer;
	std::string Handler(CIPPacket &pkt);
public:
	CTerminalServer();
	~CTerminalServer();
	bool Run();
	bool Init(int port);
	static bool start();
	
	static CTerminalServer *getInstance();
	static State getState() {
		return m_state;
	}
};

class CIPCommsThread;
class CTerminalServerManager {
	static CTerminalServerManager *m_this;
	static CIPCommsThread *m_SIAComms;
public:
	CTerminalServerManager();
	~CTerminalServerManager();
	static CTerminalServerManager &getInstance();
	bool start();
};
