#include "RemoteServer.h"
#include <memory>
#include <string>
#include<vector>
#include "ipsock.h"
#ifdef _WIN32
// windsock 1.0 #include <Winsock.h>
#include <Winsock2.h> // Windsock 2
#include <stdio.h>
#else
#include <sys/socket.h>
#endif

class Command {
	std::string m_name;
public:
	Command(const char *name) {
		m_name = name;

	}
	const char *getName() {
		std::string name;
		name += '<';
		name += m_name;
		name += '>';
		return name.c_str();
	}
};
class UnknownCommand : public Command {
public:
	UnknownCommand() : Command("Unknown") {}
};
class Commands : std::vector<Command> {
	UnknownCommand unknownCommand;
public:
	void add(Command command) {
		push_back(command);
	}
	bool find(const char *commandName);
	Command &getCommand(const char *commandName) {
		for (auto i = begin(); i != end(); i++) {
			Command c = *i;
			return c;
		}
		return unknownCommand;
	}
	
};

class HistoryCommand : public Command {
public:
	HistoryCommand() : Command("history") {}
};

class GetImageCommand : public Command {
public:
	GetImageCommand() : Command("Get") {}
};

class PutImageCommand : public Command {
public:
	PutImageCommand() : Command("Put") {}
};


class IACommands : public Commands {
public:
	IACommands() {
		HistoryCommand historyCommand;
		add(historyCommand);
		GetImageCommand getImageCommand;
		add(getImageCommand);
		PutImageCommand putImageCommand;
		add(putImageCommand);
	}
};



class RemoteServerChild : public CChildConnection {
	IACommands iaCommands;
public:
	RemoteServerChild(int sock) : CChildConnection(sock) {}
	CIPComms::EErrorCode talk();
};


class RemoteServerComms : public CIPServer {
public:
	virtual std::shared_ptr<CChildConnection> MakeClient(int sock) {
		std::shared_ptr<RemoteServerChild> ptr = std::make_shared<RemoteServerChild>(sock);
		return ptr;
	}
};


CIPComms::EErrorCode RemoteServerChild::talk() {
	EErrorCode status = SUCCESS;

	int res = ReceivePacket(m_ConnectSocket, m_CChBuffer);
	char buf[256];
	
	if (res > 0)
	{
		memcpy(buf, m_CChBuffer.GetData(), m_CChBuffer.GetSize());
		buf[m_CChBuffer.GetSize()] = '\0';
		printf("Message: %s", buf);
		//long size = m_CChBuffer.GetSize();
		Command &cmd = iaCommands.getCommand(buf);
		printf("Command found %s", cmd.getName());
		m_Read = true;
	}
	else if (!res)
	{
		// client has performed an orderly shutdown
		status = ORDERLY_SHUTDOWN;
#ifdef _WIN32
		if (closesocket(m_ConnectSocket) == SOCKET_ERROR)
		{
			m_LastError = WSAGetLastError();
			Sleep(1000);
		}
#else
		if (close(m_ConnectSocket) == SOCKET_ERROR)
		{
			m_LastError = errno;
			sleep(1);
		}
#endif
	}
	else
	{
		// Something went wrong with recv()
		status = SHUTDOWN;
#ifdef _WIN32			
		// Was it a connection reset? (client quit ungracefully)
		m_LastError = WSAGetLastError();
		if (m_LastError == WSAECONNRESET)
		{
			status = FAILED_EXTERNAL_RESET;
		}
#else
		m_LastError = errno;
#endif
		
	}
	Send("gggg", 4);
	return status;
}


RemoteServer::RemoteServer() :  m_Server(std::make_unique<RemoteServerComms>())
{
}


RemoteServer::~RemoteServer()
{
}

bool RemoteServer::Connect(int iPort) {
	return m_Server->ConnectToPB(iPort);
}

bool RemoteServer::Run()
{
	m_stop = false;
	while (!m_stop) {
		if (m_stop == false) {
			if (m_Server->Run(true)) {
				printf("No users connected\n");
			}
			else {
				return false;
			}
		}
		else {
			printf("No users connected and stop called\n");
		}
		
	}
	return true;
}
