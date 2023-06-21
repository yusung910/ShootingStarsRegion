#include "stdafx.h"

IOCP::IOCP()
{
	InitSocket();
	BindAndListen(PORT);

	for (size_t i = 0; i < MAX_CLIENT_COUNT; ++i)
	{
		//서버 세션 객체 인스턴스 생성
		ServerSession* sess = new ServerSession(i);
		//멤버 변수 m_SessList vector 배열에 저장
		m_SessList.push_back(sess);
		//세션 번호를 Queue에 저자
		m_SessQueue.push_back(i);
	}
}

IOCP::~IOCP()
{
	//윈속 클린
	WSACleanup();

}

bool IOCP::Initialize()
{
	bool LocalRet = CreateAcceptThread();
	if (!LocalRet)
		return false;

	LocalRet = CreateWorkThread();
	if (!LocalRet)
		return false;
	return false;
}

bool IOCP::InitSocket()
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

	int option = FALSE;
	setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

	//IOCP 핸들 생성
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (m_hIOCP == NULL)
	{
		cout << "[Error] IOCP 핸들 생성 실패" << endl;
		return false;
	}
	
	return true;
}

bool IOCP::BindAndListen(int Port)
{
	SOCKADDR_IN m_SocketAddr;
	ZeroMemory(&m_SocketAddr, sizeof(m_SocketAddr));
	m_SocketAddr.sin_family = AF_INET;
	m_SocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_SocketAddr.sin_port = htons(Port);

	//소켓 ip주소, port번호 바인딩
	int ret = bind(m_Socket, (SOCKADDR*)&m_SocketAddr, sizeof(SOCKADDR_IN));

	if (ret != 0)
	{
		cout << "[Error] binding 실패" << endl;
		return false;
	}

	//소켓 리스너 등록
	ret = listen(m_Socket, SOMAXCONN);
	if (ret != 0)
	{
		cout << "[Error] listen 등록 실패" << endl;
		return false;
	}

	return true;
}

void IOCP::AcceptThread()
{
	SOCKADDR_IN ClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);

	cout << "[info] Accept Thread 시작" << endl;
	while (true)
	{
		//세션 큐에 있는 세션 id를 가져온다
		int SessID = m_SessQueue.front();

		//세션 리스트에 accept함수를 통해 아직 처리되지 않은 연결들이 대기하고 있는 큐에서 제일 처음 연결된 연결을 가져와 새롭게 연결된 소켓을 저장한다.
		m_SessList[SessID]->m_Socket = accept(m_Socket, (SOCKADDR*)&ClientAddr, &nAddrLen);

		if (m_SessList[SessID]->m_Socket == INVALID_SOCKET)
		{
			cout << "[Error] 소켓 연결 실패" << endl;
			break;
		}

		m_SessList[SessID]->SetBuffer();

		//소켓을 CreateIoCompletionPort에 넣는다 간섭을 받지않기 위하여 CriticalSection을 건다
		CSLOCK(m_csObj)
		{
			HANDLE hIOCP;
			hIOCP = CreateIoCompletionPort((HANDLE)m_SessList[SessID]->m_Socket,
				m_hIOCP,
				(ULONG_PTR)(m_SessList[SessID]),
				0);

			if (hIOCP == NULL || m_hIOCP != hIOCP)
			{
				cout << "[Error] IOCP 연결 에러" << endl;
				return;
			}
		}

		bool ret = m_SessList[SessID]->RecvPacket();
		if (!ret) return;

		//성공적으로 Accept를 했으면 큐에 저장된 맨 앞 데이터를 제거
		m_SessQueue.pop_front();


		cout << "[INFO] 새로운 계정 Login : " << SessID << endl;
		cout << "[INFO] 큐 잔여 데이터 : " << m_SessQueue.size() << endl;
	}
}

unsigned int WINAPI CallAcceptThread(LPVOID p)
{
	IOCP* pOverlappedEvent = (IOCP*)p;
	pOverlappedEvent->AcceptThread();
	return 0;
}

void IOCP::WorkThread()
{
	ServerSession* pServerSession = NULL;
	bool rslt = true;
	DWORD ioSize = 0;
	Buffer* pBuffer = NULL;

	while (true)
	{
		rslt = GetQueuedCompletionStatus(
			m_hIOCP,
			&ioSize,
			(PULONG_PTR)&pServerSession,
			(LPOVERLAPPED*)&pBuffer,
			INFINITE);

		if (rslt == TRUE && ioSize == 0 && pBuffer == NULL)
		{
			break;
		}

		//클라이언트 종료
		if (rslt == false || ioSize == 0)
		{
			cout << "[INFO] 클라이언트 접속 종료" << endl;
			CloseSession(pServerSession->GetSessionID());
			continue;
		}

		if (pBuffer == NULL)
		{
			cout << "[Error] 패킷 Recv 실패 : pBuffer" << endl;
			continue;
		}

		switch (pBuffer->bufState)
		{
		case BUFFER_STATE::SEND:
			break;
		case BUFFER_STATE::RECV:
			cout << "[INFO] 패킷 수신" << endl;
			pServerSession->Handle_Receive(pBuffer->cBuf, ioSize);
			//데이터를 받을 거임
			pServerSession->RecvPacket();
			break;
		}


	}

}

bool IOCP::CreateAcceptThread()
{
	unsigned int ThreadIdx = 0;

	//CREATE_SUSPEND : ResumeThread를 호출하기 전까지 스레드를 실행하지 않는다.
	m_AcceptThread = (HANDLE)_beginthreadex(NULL, 0, &CallAcceptThread, this, CREATE_SUSPENDED, &ThreadIdx);

	if (m_AcceptThread == NULL)
	{
		cout << "[Error] Accept 스레드 생성 실패" << endl;
		return false;
	}

	//스레드 실행!
	ResumeThread(m_AcceptThread);
	return true;
}

unsigned int WINAPI CallWorkThread(LPVOID p)
{
	IOCP* pOverlappedEvent = (IOCP*)p;
	pOverlappedEvent->WorkThread();
	return 0;
}

bool IOCP::CreateWorkThread()
{
	unsigned int ThreadIdx = 0;
	for (size_t i = 0; i < MAX_WORKTHREAD; i++)
	{
		m_WorkThread[i] = (HANDLE)_beginthreadex(NULL, 0, &CallWorkThread, this, CREATE_SUSPENDED, &ThreadIdx);
		if (m_WorkThread == NULL)
		{
			cout << "[Error] CreateWorkThread 생성 실패" << endl;
			return false;
		}
		ResumeThread(m_WorkThread[i]);
	}
	return true;
}

void IOCP::CloseSession(const int nSessID)
{
	cout << "[info] 클라이언트 접속 종료 : " << nSessID << endl;
	m_SessList[nSessID]->CloseSocket();
	//종료된 클라이언트 새션 ID는 삭제됨
	m_SessQueue.push_back(nSessID);
}
