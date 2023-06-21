#include "stdafx.h"

ServerSession::ServerSession(int idx) :
	m_nIdx(idx)
{

	ZeroMemory(&m_RecvBuf, sizeof(Buffer));
	ZeroMemory(&m_SendBuf, sizeof(Buffer));
	m_RecvBuf.bufState = BUFFER_STATE::RECV;
	m_SendBuf.bufState = BUFFER_STATE::SEND;
	m_Socket = INVALID_SOCKET;

}

ServerSession::~ServerSession()
{
}

bool ServerSession::InitSocket()
{
	WSADATA wsaData;
	//윈속 로드
	int LocalRet = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LocalRet != 0)
	{
		cout << "[Error] WinSock2.2 DLL 로드 실패" << endl;
		return false;
	}

	//윈 소켓 생성
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (m_Socket == INVALID_SOCKET)
	{
		cout << "[Error] Winsocket 생성 실패" << endl;
		return false;
	}

	return true;
}

bool ServerSession::SendPacket(const char* _data, int _size)
{
	CriticalSectionLock lock(m_cs);
	DWORD dwFlag = 0, dwRecvNum = 0;

	m_RecvBuf.wsaBuf.len = MAX_BUF_SIZE;
	m_RecvBuf.wsaBuf.buf = m_RecvBuf.cBuf;
	m_RecvBuf.bufState = BUFFER_STATE::RECV;

	//윈속 패킷 수신
	int rslt = WSARecv(
		m_Socket,
		&(m_RecvBuf.wsaBuf),
		1,
		&dwRecvNum,
		&dwFlag,
		(LPWSAOVERLAPPED) & (m_RecvBuf),
		NULL);
	if (rslt == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		cout << "[Error] 패킷 Recv 실패! CODE: " << WSAGetLastError() << endl;
		return false;
	}
	return true;
}

bool ServerSession::RecvPacket()
{
	CriticalSection lock(m_cs);
	DWORD dwFlag = 0, dwRecvNumByte = 0;

	m_RecvBuf.wsaBuf.len = MAX_BUF_SIZE;
	m_RecvBuf.wsaBuf.buf = m_RecvBuf.cBuf;
	m_RecvBuf.bufState = BUFFER_STATE::RECV;

	int rslt = WSARecv(m_Socket,
		&(m_RecvBuf.wsaBuf),
		1,
		&dwRecvNumByte,
		&dwFlag,
		(LPWSAOVERLAPPED) & (m_RecvBuf),
		NULL);
	if (rslt == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		cout << "[Error] 패킷 Recv 실패! Code : " << WSAGetLastError() << endl;
		return false;
	}

	return true;
}

void ServerSession::Handle_Receive(const char* data, int size)
{
	/*
네트워크 특성상 클라이언트에서 서버로 데이터를 보내기 요청을 동시에 여러 번 하면, 서버에서는 클라이언트에서 보내는 단위대로 받지 않는다.
한꺼번에 모두 받을 수도 있고 여러번 나눠서 받을 수 있다.(즉, 클라이언트는 Send를 두 번 했는데, 서버에서는 Receive가 한 번만 발생한다.)
이런, 경우를 처리하기 위해서 먼저 받은 데이터 mPacketBuffer에 저장한 후 클라이언트에 동시에 여러번 요청하면서 서버는 한 번에 다 받으므로,
각 요청별로 나누어 처리한다. 그리고 클라이언트가 보낸 데이터중 일부 도착한 경우, 우선은 처리하지 않고 남겨놓았다가 다음에 받은 데이터와 연결하여 처리한다.
*/

	//받은 데이터를 패킷 버퍼에 저장
	memcpy(&mPacketBuf[m_nPacketBufMark], data, size);

	int LocalPacketData = m_nPacketBufMark + size;
	int LocalReadData = 0;

	while (LocalPacketData > 0) //받은 데이터를 모두 처리할 때까지 반복
	{
		if (LocalPacketData < sizeof(PacketHeader))
		{
			//남은 데이터가 패킷 헤더보다 작으면 중지
			break;
		}

		PacketHeader* pHeader = (PacketHeader*)&mPacketBuf[LocalReadData];

		if (pHeader->size <= LocalPacketData)
		{
			InMemStream io((char*)data, pHeader->size);
			//클라이언트에 요청한 패킷을 처리한다
			_PacketProcess.CheckPacket(io);
			//전송받은 패킷 사이즈 중 처리한 만큼 감소한다
			LocalPacketData -= pHeader->size;
			//전송받은 패킷 사이즈 중 처리한 만큼 증가한다.
			LocalReadData += pHeader->size;
		}
		else
		{
			break;
		}
	}

	if (LocalPacketData > 0)
	{
		char TmpBuf[MAX_RECEIVE_BUFFER_LEN * 2] = { 0, };
		memcpy(&TmpBuf[0], &mPacketBuf[LocalReadData], LocalPacketData);
		memcpy(&mPacketBuf[0], &TmpBuf[0], LocalPacketData);
	}

	//남은 데이터 양을 저장하고 데이터 받기 요청
	m_nPacketBufMark = LocalPacketData;

}

void ServerSession::SetBuffer()
{
	memset(&m_RecvBuf, 0, sizeof(Buffer));
	memset(&m_SendBuf, 0, sizeof(Buffer));
}

void ServerSession::CloseSocket()
{
	CSLOCK(m_cs)
	{
		if (m_Socket != INVALID_SOCKET)
		{
			shutdown(m_Socket, SD_BOTH);
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			m_bIsSend = false;
		}
	}
}
