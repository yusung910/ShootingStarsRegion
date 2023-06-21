#include "stdafx.h"

//재할당
void OutMemStream::ReallocBuffer(int LocalNewStreamSize)
{
	
	if (m_Buf == nullptr)
	{
		//기존 m_Buf에 할당된 포인터가 없을 경우
		//포인터 변수를 인자값으로 전달받은 크기만큼 동적할당
		m_Buf = static_cast<char*>(malloc(LocalNewStreamSize));
	}
	else
	{
		//기존 m_Buf에 할당된 포인터가 있을 경우
		//기존 m_Buf에 저장된 포인터 값을 초기화 하고 새롭게 저장
		char* tmpBuf = static_cast<char*>(malloc(LocalNewStreamSize));
		//0으로 초기화
		memset(m_Buf, 0, LocalNewStreamSize);
		//tmpBuf에 m_Buf을 복사한다.
		memcpy(tmpBuf, m_Buf, m_nCapa);
		//m_Buf에 기존에 저장된 값 초기화
		free(m_Buf);
		//m_Buf에 tmpBuf값을 저장
		m_Buf = tmpBuf;
	}
}

void OutMemStream::SetSize()
{
	//m_nHead에 short만큼 더한 동적 할당
	char* tmpBuf = static_cast<char*>(malloc(m_nHead + sizeof(short)));

	//short 변수 타입 사이즈만큼 헤드 값 증가
	m_nHead += sizeof(short);
	//m_nHead 값을 short로 형변환 하여 tmpBuf 주소 값에 저장 
	(*(short*)(tmpBuf)) = (short)m_nHead;

	//tmpBuf 포인터 변수를 +2만큼 증가한 값에, m_nHead 값에 short사이즈를 뺀 크기만큼m_Buf값을 복사한다.
	memcpy(tmpBuf + 2, m_Buf, m_nHead - sizeof(short));

	//m_Buf값에 tmpBuf 값을 m_nHead사이즈 만큼 복사하여 넣는다
	memcpy(m_Buf, tmpBuf, m_nHead);

	//동적 할당된 tmpBuf변수를 해제한다.
	free(tmpBuf);
}