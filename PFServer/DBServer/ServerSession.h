#pragma once
#define MAX_RECEIVE_BUFFER_LEN 1024

struct PacketHeader
{
	short size;
};

class ServerSession
{
public:
	ServerSession(int idx);
	~ServerSession();

	//서버 세션의 소켓 초기화
	bool InitSocket();
	//패킷 전송
	bool SendPacket(const char* _data, int _size);
	//패킷 수신
	bool RecvPacket();


	//Recv, Send 버퍼 사이즈 만큼 메모리에 세팅
	void SetBuffer();

	//소켓 종료
	void CloseSocket();

	//세션 아이디 리턴
	int GetSessionID() { return m_nIdx; }

	//수신 핸들
	void Handle_Receive(const char* data, int size);

	//패킷 수신 여부 변수 setter, getter
	bool GetIsSendOk() { return m_bIsSend; }
	void SetIsSendOk(bool b) { m_bIsSend = b; }

public:
	SOCKET m_Socket;
	Buffer m_RecvBuf;
	Buffer m_SendBuf;

private:
	int m_nIdx;
	bool m_bIsSend = false;
	int m_nPacketBufMark = 0;
	char mPacketBuf[MAX_RECEIVE_BUFFER_LEN * 2];

	CriticalSection m_cs;
};

