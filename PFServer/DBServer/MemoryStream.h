#pragma once
#include<cstdint>
#include<cstdlib>

using namespace std;
//메모리에 저장된 스트림을 출력한다
class OutMemStream
{
public:
	OutMemStream() :
		m_nHead(0),
		m_Buf(nullptr)
	{
		ReallocBuffer(1500 * 8);
	}

	~OutMemStream() { free(m_Buf); }

	//버퍼 포인터 값 리턴
	const char* GetBufPtr() const { return m_Buf; }

	//버퍼 사이즈 리턴
	int GetDataLength() const { return m_nHead; }

	//버퍼에 저장된 값의 뒷부분에 인자값으로 전달받은 데이터를 쓴다
	template<typename T>
	void Write(T in, int inSize = sizeof(T))
	{
		//is_arithmetic : 산술형식인지 확인
		//is_enum : 열거형식인지 확인
		//산술형식 또는 열거형식일 경우 C2338 에러를 출력한다
		static_assert(is_arithmetic<T>::value ||
			is_enum<T>::value,
			"입력한 데이터는 기본테이터가 아닙니다.");

		//기존 버퍼 주소 값에 인자값으로 전달받은 값을 덮어쓴다.
		(*(T*)(m_Buf + m_nHead)) = in;

		//붙여 넣은 수 만큼 사이즈를 증가한다.
		m_nHead += inSize;
	}


	//버퍼에 저장된 값의 사이즈를 측정한다.
	void SetSize();


private:
	void ReallocBuffer(int LocalNewStreamSize); //버퍼 재할당
	char* m_Buf;	//버퍼
	int m_nHead;	//버퍼 사이즈
	int m_nCapa;	//버퍼에 저장할 수 있는 양
};

//스트림에 값을 메모리에 입력.
class InMemStream
{
public:
	//생성자
	InMemStream(char* inBuf, int inSize) :
		m_Buf(inBuf),
		m_nCapa(inSize),
		m_nHead(sizeof(short)),
		m_bIsOwner(false) {}

	//소멸자
	~InMemStream() { if (m_bIsOwner) free(m_Buf); };
	//버퍼가 저장된 포인터 변수를 리턴한다
	const char* GetBufPtr() const { return m_Buf; };
	//버퍼의 잔여 수용량을 계산하여 리턴한다.
	int GetRemainingCapacity() { return m_nCapa - m_nHead; }

	//인자값만큼 버퍼 수용량과 버퍼 사이즈를 0으로 초기화한다.
	void ResetCapacity(int nCapa) { m_nCapa = nCapa; m_nHead = 0; }

	//인자값으로 전달받은 타입과 타입의 크기만큼의 데이터를
	//버퍼에서 읽는다.
	template<typename T>
	void Read(T& in, int nSize = sizeof(T))
	{
		//is_arithmetic : 산술형식인지 확인
		//is_enum : 열거형식인지 확인
		//산술형식 또는 열거형식일 경우 C2338 에러를 출력한다
		static_assert(is_arithmetic<T>::value ||
			is_enum<T>::value,
			"입력한 데이터는 기본테이터가 아닙니다.");

		//입력받은 주소연산자 값(&in)에 nSize만큼 버퍼에 저장된 값을 덮어써서 리턴한다.
		in = (*(T*)(m_Buf + m_nHead));

		//버퍼 헤드 사이즈를 증가시킨다.
		m_nHead += nSize;
	}

	void Read(string& in)
	{
		short LocalElemInSize;
		Read(LocalElemInSize);
		in.resize(LocalElemInSize);
		for (auto& elem : in)
		{
			Read(elem);
		}
	}
	
private:
	char* m_Buf;
	int m_nCapa;
	int m_nHead;
	int m_bIsOwner;

};