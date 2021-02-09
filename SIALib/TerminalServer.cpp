#include "TerminalServer.h"
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include "ipsock.h"
#include "Threads.h"
#include "SIAApplicationState.h"



CTerminalServer::State CTerminalServer::m_state = CTerminalServer::Running;

CTerminalServer::CTerminalServer()
{

}
CTerminalServer::~CTerminalServer()
{
	
}


bool CTerminalServer::Init(int port)
{
	//if (!m_Server.ConnectToPB(30001))
	if (!m_Server.ConnectToPB(22))
	{
		return false;
	}
	return true;
}

bool CTerminalServer::Run()
{
	/* 
	while (1)
	{
		m_Server.Run(true);
		if (m_Server.IsNeedRead())
		{
			std::shared_ptr<CChildConnection> l_CC = m_Server.GetFirst();
			do {
				if (l_CC->IsNeedRead())
				{

					CIPPacket &l_pkt = l_CC->GetReadBuffer();
					std::string reply = Handler(l_pkt);
					printf("Reply: %s\n", reply.c_str());

					if (!m_Server.Send((char *)reply.c_str(), reply.length(), l_CC))
					{
						printf("Server send failed: Error %s\n", m_Server.GetLastErrorString(m_Server.GetLastSocketError()));
					}
				}
			} while ((l_CC = m_Server.GetNext()));
		}
	}
	*/
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////

class CIPPacket;

class CIPCommsThreadFunctor : public SIAThreadFunctor
{
	//CSAMIPServer m_Server;
	CTerminalServer m_Server;
protected:

public:
	/// .
	CIPCommsThreadFunctor()
	{};
	/// .
	virtual bool Start();
	/// .
	virtual bool Init();
	/// .
	bool IsEmpty();
	/// .
	//CIPPacket *GetIPPacket();
	/// This CEventRouter object if present enables the Events to be routed to the correct cluster.
	/// To use this interface the CEventRouter Object needs to be derived from. Then using the
	/// pure virtual process() function enables the required functionalty to be added.  	
};





class CIPCommsThread : public SIAThread {

	CIPCommsThreadFunctor *m_pThreadFunctor;

public:
	/// .
	CIPCommsThread()
		: SIAThread(m_pThreadFunctor = new CIPCommsThreadFunctor())
	{}
	/// .
	bool IsEmpty();
	/// .
	CIPPacket *GetIPPacket();
	/// .
};


bool CIPCommsThreadFunctor::Init()
{

	if (!m_Server.Init(5000))
	{
		return false;
	}
	return true;
}

bool CIPCommsThreadFunctor::Start()
{
	if (Init() == false) return false;
	return m_Server.Run();
}

bool CIPCommsThreadFunctor::IsEmpty()
{
	return false;// m_EventQueue.IsEmpty();
}

bool CIPCommsThread::IsEmpty()
{
	return m_pThreadFunctor->IsEmpty();
}


std::string CTerminalServer::m_buffer;

std::string CTerminalServer::Handler(CIPPacket &pkt) {
	char tmp[50];
	if (pkt.GetSize() >= 49) {
		return "Error";
	}
	memcpy(tmp, pkt.GetData(), pkt.GetSize());
	tmp[pkt.GetSize()] = '\0';
	m_buffer += tmp;
	size_t cmdLen = 0;
	if ((cmdLen = m_buffer.find_last_not_of('\r')) == std::string::npos) {
		if (m_buffer.length() >= 50) {
			m_buffer.clear();
			return "Error";
		}
		return "Error";
	}
	
	std::string cmd = m_buffer.substr(0, cmdLen+1);
	
	if (cmd.compare("Run") == 0) {
		m_state = State::Running;
		m_buffer.clear();
		return "Ok";
	}
	if (cmd.compare("Pause") == 0) {
		m_state = State::Paused;
		m_buffer.clear();
		return "Ok";
	}
	if (cmd.compare("Stop") == 0) {
		m_state = State::Stopped;
		m_buffer.clear();
		return "Ok";
	}
	
	m_buffer.clear();
	return "Error";
}

CTerminalServerManager *CTerminalServerManager::m_this = nullptr;
CIPCommsThread *CTerminalServerManager::m_SIAComms = nullptr;

CTerminalServerManager::CTerminalServerManager() {
	m_SIAComms = new CIPCommsThread;
}
CTerminalServerManager::~CTerminalServerManager() {
	delete m_SIAComms;
}

CTerminalServerManager &CTerminalServerManager::getInstance() {
	if (m_this == nullptr) {
		m_this = new CTerminalServerManager;
	}
	return *m_this;
}

bool CTerminalServerManager::start() {
	return m_SIAComms->Start();
}

