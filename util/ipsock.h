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

// Version 12341234
#include <memory>
#include <cinttypes>

#ifndef _IPSOCK_
#define _IPSOCK_

#include <string.h>

/**
 */
class CIPPacket
{
	int8_t *m_Data;
	int m_iSize;

public:
	/// .
	CIPPacket()
	{
		m_Data = nullptr;
		m_iSize = 0;
	}
	/// .
	CIPPacket(CIPPacket &r)
	{
		
		m_Data = new int8_t[r.m_iSize];
		memcpy(m_Data, r.m_Data, r.m_iSize);
		m_iSize = r.m_iSize;
	};

	virtual ~CIPPacket() {
		delete [] m_Data;
	}

	/// .
	void SetData(int iSize)
	{
		if (m_Data != nullptr) {
			delete[] m_Data;
		}
		m_Data = new int8_t[iSize];
		m_iSize = iSize;
	}
/* do this if needed
	void Resize(int ResizeBy = 1024 * 2)
	{
		char *l_pTmpBuffer;
		int l_OldBufferSize = ResizeBy;
		nLineBufferSize += m_nSize;
		l_pTmpBuffer = new char[nLineBufferSize];

		memcpy(l_pTmpBuffer, m_ReadBuffer, l_OldBufferSize );
		delete m_ReadBuffer;
		m_ReadBuffer = l_pTmpBuffer;
	}
*/
	/// .
	void Empty()
	{
		if (m_Data != nullptr) {
			delete[](int8_t *)m_Data;
		}
		m_Data = nullptr;
		m_iSize = 0;
	}
	/// .
	void *GetData()
	{
		return m_Data;
	}
	/// .
	int GetSize()
	{
		return m_iSize;
	}
};


class CIPComms
{
private:
	
public:
    typedef enum
    {
            SUCCESS,
	    SHUTDOWN,
		ORDERLY_SHUTDOWN,
	    FAILED_GETSOCERR,
	    FAILED_REGFAIL,
	    FAILED_SOCCONFAIL,
	    FAILED_EXTERNAL_RESET,
	    FAILED_INITALISATION
	} EErrorCode;
	/// .
	int GetLastSocketError()
	{
		return m_LastError;
	}
	/// .
	const char *GetLastErrorString(int error);
protected:
	/// .
	int m_LastError;
	/// .
	void SetLastError(int iError)
	{
		m_LastError = iError;
	}
	/// .
	CIPComms();
	/// .
	EErrorCode m_ErrorCode;
	/// .
	int GetSocket(int address_family, int type, int protocol);
	/// .
	int ReceivePacket(int socket, CIPPacket &IPPacket);

	
	
};


class CIPClient : public CIPComms
{
	char m_ServerAddressStr[256];
	//int m_ServerListenSocket;
	int m_iSocket;

	CIPComms::EErrorCode ConnectToServer(char *AddressStr, int port, int *iSocket);
public:
	
	/// .
	CIPComms::EErrorCode Connect(char *AddressStr, int port);

	/// .
	bool Disconnect()
	{
		return true;
	}

	/// .
	bool Send(char *data, long size);
	/// .
	bool Receive(CIPPacket &pIPPacket);
	
	/// .
	bool CloseConnection();
};

class ChildConnectionList;
class CChildConnection : public CIPComms
{
    protected:
        int m_ConnectSocket;
     
		CChildConnection() {};
		CIPPacket m_CChBuffer;
		bool m_Read;
	friend class ChildConnectionList;
    public:
	/// .
        const int GetSocket() { return m_ConnectSocket; };
	/// .
        CChildConnection(int ConnectSocket_in)
		{
			m_Read = false;
			m_ConnectSocket = ConnectSocket_in;
		};

	/// .
		bool IsNeedRead()
		{
			return m_Read;
		}

	/// .
		CIPPacket &GetReadBuffer()
		{
		return m_CChBuffer;
		}

	/// .
        virtual CIPComms::EErrorCode talk();
	/// .
		virtual std::shared_ptr<CChildConnection> make(int sock) {
			return std::make_shared<CChildConnection>(sock);
		}
	/// .
        bool  operator == (CChildConnection CChildConnection_in);
	/// .
        bool Send(char *data, size_t size);


};



/**
 */
class ChildConnectionList;
class CIPServer : public CIPComms
{
private:
	bool m_bNewConnection;
	std::shared_ptr<ChildConnectionList>  m_pChildList;
	int m_ServerListenSocket;
	long m_sec;
	long m_usec;
protected:

	/// .
	int SetServerListenSocket(int iServerListenSocket)
	{
		return m_ServerListenSocket = iServerListenSocket;
	};

	/// .
	int GetServerListenSocket()
	{
		return m_ServerListenSocket;
	};

	/// .
	int m_IsNeedRead;
public:
	
	/// .
	int IsNeedRead()
	{
      return m_IsNeedRead;
	}
/*
	bool OpenSession(int socket);
	bool CloseSession(int socket);   
	bool SendComms(int socket, 
			int source_id,
			int destination_id,
			enum communications_type type, 
			char *data, 
			unsigned long size,
			enum communications_mode mode);
*/		
	/// .
	CIPServer();
	/// .
	virtual ~CIPServer();
	
	virtual std::shared_ptr<CChildConnection> MakeClient(int sock) {
		return std::make_shared<CChildConnection>(sock);
	}
	/// .
	bool ConnectToPB(int iPort);
	/// .
	bool Run(bool Listening);
	///.
	void SetSelectTimeout(int sec, int usec)
	{
		m_sec = sec;
		m_usec = usec;
	}
	/// .
	bool ConnectToPB(const char *szAddressStr, int iPort);
	/// .
	bool NewConnection()
	{
		return m_bNewConnection;
	}
	/// .
	bool Send(char *pBuffer, int iSize, std::shared_ptr<CChildConnection> pChildConnection);
};

/*
class CIPEventTrigger : public CIPClient
{
	
public:
	CIPEventTrigger();
	~CIPEventTrigger();
};
*/

/*
class CIPEventHandler : public CIPServer
{

public:
	CIPEventHandler();
	~CIPEventHandler();
};
*/
#endif
