#pragma once

template<typename T>
class GSingleton
{
public:
	inline static T& Singleton() { return SingletonCreator::Access(); }
	inline GSingleton() {}

private:
	GSingleton(const GSingleton&); //생성자
	GSingleton& operator=(const GSingleton&); //대입연산자 오버로딩
	
protected:
	class SingletonCreator
	{
	public:
		inline SingletonCreator()
		{
			Access();
		}

		inline static T& Access()
		{
			/* 
			* 정적 객체가 인라인 함수로 선언된 경우 둘 이상의 객체를 만들 수 있다.
			* 이 문제를 해결하려면 내부에 정적 인스턴스있던 비 인라인 기능에 엑세스를 해야한다.
			*/
			static T& ref = _CreateUniqueInstance();
			return ref;
		}
	private: 
		//메인함수가 실행되기 전 인스턴스가 생성되는 것을 보장하기 위한 함수
		inline void _ToGuaranteeInstanceCreationBeforeMainFunction() {}

		//__declspec(noinline) : 컴파일러에 특정 멤버 함수(클래스의 함수)를 인라인하지 않도록 지시.
		//url : https://learn.microsoft.com/ko-kr/cpp/cpp/noinline?view=msvc-170
		__declspec(noinline) static T& _CreateUniqueInstance()
		{
			static T obj;
			ms_SingletonCreator._ToGuaranteeInstanceCreationBeforeMainFunction();

			return obj;
		}
	private:
		static SingletonCreator ms_SingletonCreator;
	};
};

//상수 사용에 필요한 변수 선언
template<typename T>
typename GSingleton<T>::SingletonCreator GSingleton<T>::SingletonCreator::ms_SingletonCreator;