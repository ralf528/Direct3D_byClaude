#pragma once
//< 복사방지 와 대입방지
#define COPYPREVENT(type)  \
	type( const type &rhw );   \
	type& operator=( const type &rhw );

//< 싱글톤 매크로
#define SINGLETON(type)\
	COPYPREVENT(type)\
	friend Singleton<type>;

template <typename T>
class Singleton
{
private:
	Singleton(void) {}
	~Singleton(void) {}

	//< 외부인터페이스 
public:
	static T*	getInstance( void );
};

template <typename T>
T*	Singleton<T>::getInstance( void )
{
	static T instance;
	return &instance;
}

