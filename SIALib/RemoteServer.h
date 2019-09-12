#pragma once
#include "ipsock.h"

class RemoteServerComms;
class IACommands;

class RemoteServer
{
	bool m_stop;
	std::unique_ptr<RemoteServerComms> m_Server;
	std::unique_ptr<IACommands> m_iaCommands;
public:
	RemoteServer();
	~RemoteServer();
	bool Connect(int iPort);
	bool Run();
};

