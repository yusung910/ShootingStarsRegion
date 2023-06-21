#include "stdafx.h"

//크리티컬섹션(임계영역) 구조체
struct CriticalSection : public CRITICAL_SECTION
{
	//CS 구조체 생성자
	CriticalSection() { InitializeCriticalSection(this); }
	//CS 구조체 소멸자
	~CriticalSection() { DeleteCriticalSection(this); }
};

/*
RAII
C++ 같이 개발자가 직접 리소스 관리를 해줘야하는 언어에서 메모리 누수를 방지하기 위한 중요한 기법으로 해당 리소스의 사용범위가
끝날 경우에 자동으로 해제를 해주어 예외가 발생하거나 하는 경우에는 획득한 자원이 해제됨을 보장하여 안전한 코드를 작성할 수 있다.
*/

//Lock을 설정할 CS 포인터 변수를 생성자를 통한 인자값으로 전달받아 설정하고 해제하는 클래스
class CriticalSectionLock
{
public:
	CriticalSectionLock(const CRITICAL_SECTION& cs) : m_Pcs(&cs)
	{
		//디버깅 모드에서 개발자가 오류가 생기면 치명적일 것이라는 곳에 심어 놓는 에러 검출용 코드.
		assert(m_Pcs != nullptr);
		//CS 설정
		EnterCriticalSection((LPCRITICAL_SECTION)m_Pcs);
	}

	~CriticalSectionLock()
	{
		if (m_Pcs != nullptr)
			//CS 해제
			LeaveCriticalSection((LPCRITICAL_SECTION)m_Pcs);
	}

	//explicit : 암시적인 형변환을 막기 위해 사용
	explicit operator bool() { return true; }

private:
	//크리티컬 섹션 포인터 변수
	const CRITICAL_SECTION* m_Pcs;
};

//c#에서는 lock이라는 메크로를 선언해서 CriticalSection을 보다 쉽게 구현이 가능하지만
//c++은 없기 때문에 따로 만들어줘야 한다
#define CSLOCK(cs_) if(CriticalSectionLock CONCATENATE(lock,__LINE__) = cs_)
//__LINE__ : 디버거가 포함된 컴파일러들은 보통 실행하면서 어떤 라인에서 에러가났는지 출력해줌