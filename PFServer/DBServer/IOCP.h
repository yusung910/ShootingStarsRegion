#pragma once

//최대 버퍼 사이즈
#define MAX_BUF_SIZE 1024
//포트번호
#define PORT 8000
//최대 클라이언트 수
#define MAX_CLIENT_COUNT 3
//최대 워크 스레드 수
#define MAX_WORKTHREAD 2

class ServerSession;

//버퍼 형식 열거형
enum BUFFER_STATE
{
	SEND,
	RECV
};

struct Buffer : OVERLAPPED
{
	//버퍼가 저장될 변수
	char cBuf[MAX_BUF_SIZE];
	//소켓 데이터의 송수신에서 데이터를 저장하고 보내기 위해서 사용하는 버퍼.
	WSABUF wsaBuf;
	//전송될 버퍼 형식
	BUFFER_STATE bufState;
};


class IOCP : public GSingleton<IOCP>
{
public:
	//IOCP 클래스 생성자
	IOCP();

	//IOCP 클래스 소멸자
	~IOCP();

	//IOCP 초기화
	bool Initialize();

	//윈속 소켓 초기화
	bool InitSocket();

	//윈속 주소 바인딩 및 리스너 등록
	bool BindAndListen(int Port);

	//iocp 스레드
	void AcceptThread();

	//워크 스레드
	void WorkThread();


private:
	//IOCP 스레드 생성
	bool CreateAcceptThread();

	//워크 스레드 생성
	bool CreateWorkThread();

	//세션 종료
	void CloseSession(const int nSessID);

private:
	SOCKET m_Socket; //소켓

	HANDLE m_hIOCP; //iocp핸들
	HANDLE m_AcceptThread = NULL; //스레드 핸들
	HANDLE m_WorkThread[MAX_WORKTHREAD] = { NULL, };

	vector<ServerSession*> m_SessList; //접속한 클라이언트가 저장된 배열
	deque<int> m_SessQueue; //접속한 클라이언트의 번호가 저장될 큐

	CriticalSection m_csObj;
};

#define _IOCP IOCP::Singleton()