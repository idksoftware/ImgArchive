/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c)2015 IDK Software Ltd
**	  All Rights Reserved.
**
****************************************************
**
**	Filename : CRegString.cpp
**	Author : I.Ferguson
**	Version : 1.000
**	Date : 26 - 05 - 2015
* *
** #$$@@$$# */


/** This software and its associated documentation contains proprietary,
** confidential and trade secret information of IDK Software Ltd.
** and except as provided by written agreement with IDK Software Ltd.
**
** a)   no part may be disclosed, distributed, reproduced, transmitted,
**      transcribed, stored in a retrieval system, adapted or translated
**      in any form or by any means electronic, mechanical, magnetic,
**      optical, chemical, manual or otherwise,
**
**      and
**
** b)   the recipient is not entitled to discover through reverse
**      engineering or reverse compiling or other such techniques or
**      processes the trade secrets contained therein or in the
**      documentation.
**
**
** $Id$ */

#ifdef _WIN32
// windsock 1.0 #include <Winsock.h>
#include <Winsock2.h> // Windsock 2
#include <stdio.h>
#else
#include <sys/socket.h>

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cport.h"
#endif

#include "ipsock.h"
//#include "cidklinklist.h"
#include <vector>
#include <algorithm>

const int RETRY_MAX = 5;

#ifdef _WIN32
static bool m_IsStarted = false;
#else
#define SOCKET_ERROR -1
#endif

CIPComms::CIPComms()
{
	/* The WinSock DLL is acceptable. Proceed. */
	m_ErrorCode = SUCCESS;
}

CIPComms::EErrorCode CIPClient::Connect(char *AddressStr, int port)
{
	strncpy_s(m_ServerAddressStr, AddressStr, 256);
	return ConnectToServer(AddressStr, port, &m_iSocket);
}



int CIPComms::GetSocket(int address_family, int type, int protocol)
{
    int socket_handle = -1;
    long tries;
    bool ok = true;
    
    for (tries = 0; tries < 100 && ok; tries++)
	{
        socket_handle = (int)socket(address_family, type, protocol);
        if (socket_handle == -1)						/* request failed ? */
		{
#if defined WIN32
            Sleep(1000);                            /* wait and try again */
#else
	    sleep(1);
#endif
		}
        else
		{
            break;								/* got socket, so exit loop */
		}
	}
    
    return socket_handle;
}
		
CIPComms::EErrorCode CIPClient::ConnectToServer(char *AddressStr, int port, int *iSocket)
{
	bool ok = true;
	struct sockaddr_in sockaddr_in;
	EErrorCode status = SUCCESS;
	int retry = RETRY_MAX;
		

	if (ok)		/* Allocate socket */
	{
		*iSocket = GetSocket(AF_INET, SOCK_STREAM, 0);

		if (*iSocket == -1)
		{
			ok = false;
			status = FAILED_GETSOCERR;
		}
	}

	if (ok)		/* Connect to server via socket */
	{
		while ((retry-- > 0))
		{
			//	struct in_addr l_addr;
			memset(&sockaddr_in, '\0', sizeof (sockaddr_in));			/* connect to the remote PortBroker */
			sockaddr_in.sin_family = AF_INET;
			sockaddr_in.sin_port = htons(port);
			sockaddr_in.sin_addr.s_addr = inet_addr(AddressStr);

			if (connect(*iSocket, (struct sockaddr *)&sockaddr_in, sizeof (sockaddr_in)) == -1) 
			{
				ok = false;
				status = FAILED_SOCCONFAIL;
#ifdef _WIN32
				m_LastError = WSAGetLastError();
#else
				m_LastError = errno;
#endif
				

			}
			else
			{
				ok = true;
				status = SUCCESS;
				retry = -1;
			}
		}
	}
	if (status != SUCCESS)
	{
#ifdef _WIN32
		if (closesocket(*iSocket))
		{

			m_LastError = WSAGetLastError();

		}
#else
		if (close(*iSocket))
		{

			m_LastError = errno;

		}
#endif
		*iSocket = 0; 
	}
	return status;   
}

int CIPComms::ReceivePacket(int socket, CIPPacket &pIPPacket)
{
	unsigned long msglen = 0L;
	unsigned long opt = 1L;

//	char buffer[4 * 1024];

	pIPPacket.Empty();
/*
#ifdef _WIN32
	if (SOCKET_ERROR == ioctlsocket(socket,FIONREAD ,&opt))
	{
		m_LastError = WSAGetLastError();
		return -1;
	}
#else
	if (SOCKET_ERROR == ioctl(socket,FIONREAD ,&opt))
	{
		m_LastError = errno;
		return -1;
	}
#endif
*/
//	if (opt == 0)
//	{
//		return 0;
//	}
	
	char c;
	msglen = recv(socket, &c, 1, 0);
	if (!msglen)
	{
		return 0; // If no messages are available to be received and the peer
			  // has performed an orderly shutdown, recv() shall return 0
	}
#ifdef _WIN32
	if (SOCKET_ERROR == msglen)
	{
		m_LastError = WSAGetLastError();
		return -1;
	}
#else
	if (SOCKET_ERROR == msglen)
	{
		m_LastError = errno;
		return -1;
	}
#endif

#ifdef _WIN32
	if (SOCKET_ERROR == ioctlsocket(socket,FIONREAD ,&opt))
	{
		m_LastError = WSAGetLastError();
		return -1;
	}
#else
	if (SOCKET_ERROR == ioctl(socket,FIONREAD ,&opt))
	{
		m_LastError = errno;
		return -1;
	}
#endif
	pIPPacket.SetData(opt+1);
	char *l_Temp = (char *)pIPPacket.GetData();
	*l_Temp = c;
	msglen = recv(socket, l_Temp + 1, opt, 0);
#ifdef _WIN32
	if (SOCKET_ERROR == msglen)
	{
		m_LastError = WSAGetLastError();
		return -1;
	}
#else
	if (SOCKET_ERROR == msglen)
	{
		m_LastError = errno;
		return -1;
	}
#endif
	msglen++;
	if (opt+1 < msglen)
	{
		return -1; // allocated too little space?? 
	}
	if (opt+1 > msglen)
	{
		return -1; // more data to get fix later 
	}
	
	//if (msglen != opt)
	//{
	//	return -1;
	//}
/* Temp fix to get things working
	
		
	unsigned int l_iBufferSize = 1024 * 2;
	char l_szStrbuffer[l_iBufferSize];


	while (1)
	{
		unsigned long opt = 0L;

		if (SOCKET_ERROR == ioctlsocket(socket,FIONREAD ,&opt))
		{
#ifdef _WIN32
			m_LastError = WSAGetLastError();
#endif
			return -1;
		}
		if (opt == 0)
		{
			break;
		}
		if (opt > l_iBufferSize)
		{
			msglen = recv(socket, l_szStrbuffer, l_iBufferSize, 0);
		}
		else
		{
			msglen = recv(socket, l_szStrbuffer, opt, 0);

		}
			
		if (SOCKET_ERROR == msglen)  	
		{
			return -1;
		}
		l_szStrbuffer[msglen] = 0;
		*pChBuffer += l_szStrbuffer;
	}
*/	
	

	return msglen;
}

bool CIPClient::Receive(CIPPacket &pIPPacket)
{
	if (ReceivePacket(m_iSocket, pIPPacket) == SOCKET_ERROR)
	{
#ifdef _WIN32
		m_LastError = WSAGetLastError();
#else
		m_LastError = errno;
#endif
		return false;
	}
	return true;
}

bool CIPClient::Send(char *data, long size)
{
	if (send(m_iSocket, data, size, 0) == SOCKET_ERROR)
	{
#ifdef _WIN32
		m_LastError = WSAGetLastError();
#else
		m_LastError = errno;
#endif
		return false;
	}
	return true;
}

bool CIPClient::CloseConnection()
{
#ifdef _WIN32
	if (closesocket(m_iSocket) == SOCKET_ERROR)
	{
		m_LastError = WSAGetLastError();
#else
	if (close(m_iSocket) == SOCKET_ERROR)
	{
		m_LastError = errno;
#endif
		return false;
	}
	return true;
}
/*
CIPEventHandler::CIPEventHandler()
{
#ifdef _WIN32
	if (!m_IsStarted)
	{
		WORD    m_wSVer;
		WSADATA m_wsadata;
		m_wSVer = MAKEWORD(1, 1);
		if (WSAStartup(m_wSVer, &m_wsadata) != 0)
		{
			 m_ErrorCode = FAILED_INITALISATION;
		}
		m_IsStarted = true;
	}
#endif
	m_ErrorCode = SUCCESS;
}

CIPEventHandler::~CIPEventHandler(void)
{

}
*/

class ChildConnectionList
{
private:
	std::vector<CChildConnection *> m_list;
	std::vector<CChildConnection *>::iterator m_iter;
	bool m_cleanUp;
public:
	ChildConnectionList() {
		m_cleanUp = false;
	};
	CChildConnection *GetFirst()
	{
		//if (m_cleanUp) {
		//	for (std::vector<CChildConnection *>::iterator i = m_list.begin(); i != m_list.end(); i++) {
		//		if (*i == nullptr) {
		//			m_list.erase(i);
		//		}
		//	}
		//}
		m_iter = m_list.begin();
		if (m_iter == m_list.end()) {
			return nullptr;
		}
		CChildConnection *item = *m_iter;
		return item;
	}

	CChildConnection *GetNext()
	{
		if (m_iter == m_list.end()) {
			return nullptr;
		}
		m_iter++;
		if (m_iter == m_list.end()) {
			return nullptr;
		}
		CChildConnection *item = *m_iter;
		return item;
	}
	
	void DeleteChild (CChildConnection *ChildConnection_in, CIPComms::EErrorCode status)
	{
		std::vector<CChildConnection *>::iterator iter = std::find(m_list.begin(), m_list.end(), ChildConnection_in);
		if (iter == m_list.end()) {
			return; // not found
		}
		printf("Deleting child connection\n");
		if (status == CIPComms::ORDERLY_SHUTDOWN)
		{
			// socker is closed
			return;
		}
#ifdef _WIN32
		if ((shutdown(ChildConnection_in->GetSocket(), SD_SEND)) == SOCKET_ERROR)
#else
		if ((shutdown(ChildConnection_in->GetSocket(), SHUT_WR)) == SOCKET_ERROR)
#endif
		{
#ifdef _WIN32
			ChildConnection_in->m_LastError = WSAGetLastError();
#else
			ChildConnection_in->m_LastError = errno;
#endif
		}
		char buf[256];
		bool l_exit = false;
		while (1)
		{
			switch(recv(ChildConnection_in->GetSocket(),buf,256,0))
			{
			case SOCKET_ERROR:
#ifdef _WIN32
				ChildConnection_in->m_LastError = WSAGetLastError();
#else
				ChildConnection_in->m_LastError = errno;
#endif
				l_exit = true;
				break;
			case 0:
				l_exit = true;
				break; // flushed the socket
			default:
				continue;
			}
			if (l_exit)
			{
				break;
			}
		};
		
		delete ChildConnection_in;
		CChildConnection *item = *iter;
		//m_list.erase(iter);
		//delete item;
		*item = 0;
		m_cleanUp = true;
		
	}
	
	void AddObject(CChildConnection* object_in)
	{
		m_list.push_back(object_in);
	}
};

bool CChildConnection::Send(char *data, long size)
{
	if (send(m_ConnectSocket, data, size, 0) == SOCKET_ERROR)
	{
#ifdef _WIN32
		m_LastError = WSAGetLastError();
#else
		m_LastError = errno;
#endif
		return false;
	}
	return true;
}

CIPServer::CIPServer()
{
	
#ifdef _WIN32
	if (!m_IsStarted)
	{
		WORD    m_wSVer;
		WSADATA m_wsadata;
		m_wSVer = MAKEWORD(1, 1);
		if (WSAStartup(m_wSVer, &m_wsadata) != 0)
		{
			 m_ErrorCode = FAILED_INITALISATION;
		}
		m_IsStarted = true;
	}
#endif
	m_pChildList = new ChildConnectionList;
	m_ErrorCode = SUCCESS;
	m_bNewConnection = false;
}

CIPServer::~CIPServer()
{
	delete m_pChildList;
}

bool CIPServer::ConnectToPB(int iPort)
{
	bool ok = true;
		
	m_ServerListenSocket = -1;
//	const char *protocol = "tcp";
	int server_socket;               
//	char hostname[65];
//	int broker_socket;
#ifdef _WIN32
	int namelen;
#else
	unsigned int namelen;
#endif
	u_short dynamic_port;
//	u_long actual_protocol;
//	bool broker_connected = false;
//	struct hostent *hostent;
	struct sockaddr_in sockaddr_in;

	server_socket = GetSocket(AF_INET, SOCK_STREAM, 0);			// socket that server will listen on
	if (server_socket == -1)
	{
		ok = false;
	}

	if (ok)
	{
		int yes = true;
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(int)) == SOCKET_ERROR)
		{
#ifdef _WIN32
				m_LastError = WSAGetLastError();
#else
				m_LastError = errno;
#endif
				ok = false;
		}
	}
	
	if (ok)
	{
		SetServerListenSocket(server_socket);
		int l_iRetry = 5;

		// Dave's fix?
		int flag = 1;
		setsockopt (server_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&flag, sizeof(flag));
		
		while (l_iRetry > 0)
		{
			memset(&sockaddr_in, '\0', sizeof (sockaddr_in));			/* bind the socket so that it is 'ours' */
			sockaddr_in.sin_family = AF_INET;
//			sockaddr_in.sin_port = htons(0);					/* any port in a storm */
			sockaddr_in.sin_port = htons(iPort);					/* Set port */
//			sockaddr_in.sin_addr.s_addr = inet_addr(szAddressStr);
			sockaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
			ok = true;
			if (bind(server_socket, (struct sockaddr *)&sockaddr_in, sizeof (sockaddr_in)) == SOCKET_ERROR)
			{
#ifdef _WIN32
				m_LastError = WSAGetLastError();
				Sleep(1000);
#else
				m_LastError = errno;
				sleep(1);
#endif
				l_iRetry--;
				ok = false;
				
			}
			else
			{
				ok = true;;
				break;
			}
		}

	}
	if (ok)
	{
		if (listen(server_socket, 10) != 0)					/* must start listening before telling */
		{									/* PortBroker about the port */
			ok = false;
		}
	}
	if (ok)
	{
		namelen = sizeof (sockaddr_in);						/* get port number etc. allocated by bind */
		if (getsockname(server_socket, (struct sockaddr *)&sockaddr_in, &namelen) != 0)
		{
			ok = false;
		}
		else
		{
			dynamic_port = sockaddr_in.sin_port;
		}
	}
	return ok;
}

CChildConnection *CIPServer::GetFirst()
{
	return m_pChildList->GetFirst();
}

CChildConnection *CIPServer::GetNext()
{
	return m_pChildList->GetNext();
}

bool CIPServer::Send(char *pBuffer, int iSize, CChildConnection *pChildConnection)
{

	// This tests that the child connection is still valid.
	bool found = false;
	CChildConnection *l_pCC = m_pChildList->GetFirst();
	do 
	{
		if (l_pCC == pChildConnection)
		{
			found = true;
			break;
		}
	}
	while ((l_pCC = m_pChildList->GetNext()));
	if (!found)
	{
		return false;
	}
	return pChildConnection->Send(pBuffer, iSize);
}

bool CIPServer::Run(bool Listening)
	{
	m_IsNeedRead = 0;
#ifdef _WIN32
    int client_addr_len;
#else
    unsigned int client_addr_len;
#endif
	int connect_socket = -1;
    struct sockaddr_in client_addr;
    bool done = false;
    int sock;
    struct timeval l_tvTime;

	CChildConnection *temp_childconnection;

    CIPComms::EErrorCode status;
    int fd_count;
	
    // Set up sockets for select
    fd_set noSockets2Read;
    fd_set noSockets2Write;
    fd_set noSockets2Error;
    
    client_addr_len = sizeof (client_addr);
    l_tvTime.tv_sec =  60 * 60;
    l_tvTime.tv_usec  = 1;
	
	
    // Loop until nothing to read from read sockets
    while (!done)
    {  
        // Initialise variables required for select()
        FD_ZERO(&noSockets2Read);
        FD_ZERO(&noSockets2Write);
        FD_ZERO(&noSockets2Error);
        if (Listening)
	{
			int l_ListenSocket = GetServerListenSocket();
            FD_SET(l_ListenSocket, &noSockets2Read); // internal server
	}

    CChildConnection *l_item = m_pChildList->GetFirst();
	if (l_item)
	{
		 // Set socket for reading per child connection
		do
		{
			FD_SET(l_item->GetSocket(), &noSockets2Read);
		} while ((l_item = m_pChildList->GetNext()));
	}

        // 1st parameter - for UNIX compatibility
	if((fd_count = select(FD_SETSIZE, &noSockets2Read, &noSockets2Write, &noSockets2Error, &l_tvTime))  == -1) // SOCKET ERROR
	{
#ifdef _WIN32
		m_LastError = WSAGetLastError();
#else
		m_LastError = errno;
#endif
            return false; // cannot select so returning
	}
        
        // No Read data
        if (fd_count == 0 ) 
	{
            done = true;
	}				
        else
	{
            // Is there a new connection to the listen socket ??
		if (FD_ISSET(GetServerListenSocket(), &noSockets2Read) && Listening) 
		{
			connect_socket = (int)accept(GetServerListenSocket(),
						(struct sockaddr *)&client_addr,&client_addr_len);
			if (connect_socket < 0)
			{
#ifdef _WIN32
				m_LastError = WSAGetLastError();
#else
				m_LastError = errno;
#endif
				return false; // cannot connect so returning
			}
                
                // Create a new Child connection
			m_bNewConnection = true;
			m_pChildList->AddObject(new CChildConnection(connect_socket));
		} 
        else
	{
                connect_socket = -1;
				// See which child corresponds to socket

				temp_childconnection = m_pChildList->GetFirst();
				if (temp_childconnection)
				{
					do {
				
						if ((sock = temp_childconnection->GetSocket()) != GetServerListenSocket()) 
						{
							if (FD_ISSET(sock, &noSockets2Read))
							{
								if ((status = temp_childconnection->talk()) != SUCCESS) 
								{
									//shutdown command sent or external disconnection
									if (status == SHUTDOWN || status == FAILED_EXTERNAL_RESET || status == ORDERLY_SHUTDOWN)
									{
									// Close this child connection down
										m_pChildList->DeleteChild(temp_childconnection, status);
									
									}
								}
								else
								{
									m_IsNeedRead++;
								}
							}
						}
					} while ((temp_childconnection = m_pChildList->GetNext()));
					done = true;
				}
			}
		} 
	}
    return true;
}

CIPComms::EErrorCode CChildConnection::talk()
{
	EErrorCode status = SUCCESS;
	
	int res = ReceivePacket(m_ConnectSocket, m_CChBuffer);
	if (res > 0)
	{				
		//long size = m_CChBuffer.GetSize();
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
	
	return status;
}

#ifdef _WIN32
/*
 * Windows Sockets definitions of regular Microsoft C error constants
 */
const char *CIPComms::GetLastErrorString(int error)
{
	switch(error)
	{
	case WSAEINTR:
		return "EINTR";
	case WSAEBADF:
		return "EBADF";
	case WSAEACCES:
		return "EACCES";
	case WSAEFAULT:
		return "EFAULT";
	case WSAEINVAL:
		return "EINVAL";
	case WSAEMFILE:
		return "EMFILE";

/*
 * Windows Sockets definitions of regular Berkeley error constants
 */
	case WSAEWOULDBLOCK:
		return "WOULDBLOCK";
	case WSAEINPROGRESS:
		return "INPROGRESS";
	case WSAEALREADY:
		return "ALREADY";
	case WSAENOTSOCK:
		return "NOTSOCK";
	case WSAEDESTADDRREQ:
		return "DESTADDRREQ";
	case WSAEMSGSIZE:
		return "MSGSIZE";
	case WSAEPROTOTYPE:
		return "PROTOTYPE";
	case WSAENOPROTOOPT:
		return "NOPROTOOPT";
	case WSAEPROTONOSUPPORT:
		return "PROTONOSUPPORT";
	case WSAESOCKTNOSUPPORT:
		return "SOCKTNOSUPPORT";
	case WSAEOPNOTSUPP:
		return "OPNOTSUPP";
	case WSAEPFNOSUPPORT:
		return "EPFNOSUPPORT";
	case WSAEAFNOSUPPORT:
		return "EAFNOSUPPORT";
	case WSAEADDRINUSE:
		return "EADDRINUSE";
	case WSAEADDRNOTAVAIL:
		return "EADDRNOTAVAIL";
	case WSAENETDOWN:
		return "ENETDOWN";
	case WSAENETUNREACH:
		return "ENETUNREACH";
	case WSAENETRESET:
		return "ENETRESET";
	case WSAECONNABORTED:
		return "ECONNABORTED";
	case WSAECONNRESET:
		return "ECONNRESET";
	case WSAENOBUFS:
		return "ENOBUFS";
	case WSAEISCONN:
		return "EISCONN";
	case WSAENOTCONN:
		return "ENOTCONN";
	case WSAESHUTDOWN:
		return "ESHUTDOWN";
	case WSAETOOMANYREFS:
		return "ETOOMANYREFS";
	case WSAETIMEDOUT:
		return "ETIMEDOUT";
	case WSAECONNREFUSED:
		return "CONNREFUSED";
	case WSAELOOP:
		return "ELOOP";
	case WSAENAMETOOLONG:
		return "ENAMETOOLONG";
	case WSAEHOSTDOWN:
		return "EHOSTDOWN";
	case WSAEHOSTUNREACH:
		return "EHOSTUNREACH";
	case WSAENOTEMPTY:
		return "ENOTEMPTY";
	case WSAEPROCLIM:
		return "EPROCLIM";
	case WSAEUSERS:
		return "EUSERS";
	case WSAEDQUOT:
		return "EDQUOT";
	case WSAESTALE:
		return "ESTALE";
	case WSAEREMOTE:
		return "EREMOTE";

/*
 * Extended Windows Sockets error constant definitions
 */
	case WSASYSNOTREADY:
		return "SYSNOTREADY";
	case WSAVERNOTSUPPORTED:
		return "VERNOTSUPPORTED";
	case WSANOTINITIALISED:
		return "NOTINITIALISED";
	case WSAEDISCON:
		return "EDISCON";
	case WSAENOMORE:
		return "ENOMORE";
	case WSAECANCELLED:
		return "ECANCELLED";
	case WSAEINVALIDPROCTABLE:
		return "EINVALIDPROCTABLE";
	case WSAEINVALIDPROVIDER:
		return "EINVALIDPROVIDER";
	case WSAEPROVIDERFAILEDINIT:
		return "EPROVIDERFAILEDINIT";
	case WSASYSCALLFAILURE:
		return "SYSCALLFAILURE";
	case WSASERVICE_NOT_FOUND:
		return "SERVICE_NOT_FOUND";
	case WSATYPE_NOT_FOUND:
		return "TYPE_NOT_FOUND";
	case WSA_E_NO_MORE:
		return "E_NO_MORE";
	case WSA_E_CANCELLED:
		return "E_CANCELLED";
	case WSAEREFUSED:
		return "EREFUSED";

/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * (when using the resolver). Note that these errors are
 * retrieved via WSAGetLastError() and must therefore follow
 * the rules for avoiding clashes with error numbers from
 * specific implementations or language run-time systems.
 * For this reason the codes are based at WSABASEERR+1001.
 * Note also that [WSA]NO_ACMCRESS is defined only for
 * compatibility purposes.
 */

/* Authoritative Answer: Host not found */
	case WSAHOST_NOT_FOUND:
		return "HOST_NOT_FOUND";

/* Non-Authoritative: Host not found, or SERVERFAIL */
	case WSATRY_AGAIN:
		return "TRY_AGAIN";

/* Non-recoverable errors, FORMERR, REFUSED, NOTIMP */
	case WSANO_RECOVERY:
		return "NO_RECOVERY";

/* Valid name, no data record of requested type */
	case WSANO_DATA:
		return "NO_DATA";

/*
 * Define QOS related error return codes
 *
 */
	case WSA_QOS_RECEIVERS:
		return "QOS_RECEIVERS";
         /* at least one Reserve has arrived */
	case WSA_QOS_SENDERS:
		return "QOS_SENDERS";
         /* at least one Path has arrived */
	case WSA_QOS_NO_SENDERS:
		return "QOS_NO_SENDERS";
         /* there are no senders */
	case WSA_QOS_NO_RECEIVERS:
		return "QOS_NO_RECEIVERS";
         /* there are no receivers */
	case WSA_QOS_REQUEST_CONFIRMED:
		return "QOS_REQUEST_CONFIRMED";
         /* Reserve has been confirmed */
	case WSA_QOS_ADMISSION_FAILURE:
		return "QOS_ADMISSION_FAILURE";
         /* error due to lack of resources */
	case WSA_QOS_POLICY_FAILURE:
		return "QOS_POLICY_FAILURE";
         /* rejected for administrative reasons - bad credentials */
	case WSA_QOS_BAD_STYLE:
		return "QOS_BAD_STYLE";
         /* unknown or conflicting style */
	case WSA_QOS_BAD_OBJECT:
		return "QOS_BAD_OBJECT";
         /* problem with some part of the filterspec or providerspecific
          * buffer in general */
	case WSA_QOS_TRAFFIC_CTRL_ERROR:
		return "QOS_TRAFFIC_CTRL_ERROR";
         /* problem with some part of the flowspec */
	case WSA_QOS_GENERIC_ERROR:
		return "QOS_GENERIC_ERROR";
         /* general error */
	case WSA_QOS_ESERVICETYPE:
		return "QOS_ESERVICETYPE";
         /* invalid service type in flowspec */
	case WSA_QOS_EFLOWSPEC:
		return "QOS_EFLOWSPEC";
         /* invalid flowspec */
	case WSA_QOS_EPROVSPECBUF:
		return "QOS_EPROVSPECBUF";
         /* invalid provider specific buffer */
	case WSA_QOS_EFILTERSTYLE:
		return "QOS_EFILTERSTYLE";
         /* invalid filter style */
	case WSA_QOS_EFILTERTYPE:
		return "QOS_EFILTERTYPE";
         /* invalid filter type */
	case WSA_QOS_EFILTERCOUNT:
		return "QOS_EFILTERCOUNT";
         /* incorrect number of filters */
	case WSA_QOS_EOBJLENGTH:
		return "QOS_EOBJLENGTH";
         /* invalid object length */
	case WSA_QOS_EFLOWCOUNT:
		return "QOS_EFLOWCOUNT";
         /* incorrect number of flows */
	case WSA_QOS_EUNKOWNPSOBJ:
		return "QOS_EUNKOWNPSOBJ";
         /* unknown object in provider specific buffer */
	case WSA_QOS_EPOLICYOBJ:
		return "QOS_EPOLICYOBJ";
         /* invalid policy object in provider specific buffer */
	case WSA_QOS_EFLOWDESC:
		return "QOS_EFLOWDESC";
         /* invalid flow descriptor in the list */
	case WSA_QOS_EPSFLOWSPEC:
		return "QOS_EPSFLOWSPEC";
         /* inconsistent flow spec in provider specific buffer */
	case WSA_QOS_EPSFILTERSPEC:
		return "QOS_EPSFILTERSPEC";
         /* invalid filter spec in provider specific buffer */
	case WSA_QOS_ESDMODEOBJ:
		return "QOS_ESDMODEOBJ";
         /* invalid shape discard mode object in provider specific buffer */
	case WSA_QOS_ESHAPERATEOBJ:
		return "QOS_ESHAPERATEOBJ";
         /* invalid shaping rate object in provider specific buffer */
	case WSA_QOS_RESERVED_PETYPE:
		return "QOS_RESERVED_PETYPE";
         /* reserved policy element in provider specific buffer */
	default:
		break; // fall into INVALID_ERROR_CODE;
	}
	return "INVALID_ERROR_CODE";
	
}

#else

const char *CIPComms::GetLastErrorString(int error)
{
	return strerror(error);
}

#endif
