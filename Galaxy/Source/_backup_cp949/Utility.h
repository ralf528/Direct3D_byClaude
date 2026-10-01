#pragma once
#include <assert.h>

#define	USE_DUMP	0


#pragma warning(disable:4067)

#define MAX_BUFFER_ROW		2		// 두개의 버퍼
#define MAX_BUFFER_COLUMN	1024

extern char * Format(const  char * pFormat, ...);
extern  VOID  PrintDebugStr(const  char * pFormat, ...);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///<==============================================================================================================================
///                                              문자열 관련 함수
///<==============================================================================================================================
extern  VOID  RePlaceAll(std::string& strSrc, const std::string& strFind, const std::string& strDest) ;	///< 문자열 치환

extern bool Str_UTF8ToUnicodeDynamic(const char* pUTF8, std::wstring& rUnicode);	// UTF8 -> wchar_t 타입 스트링 변환
extern bool Str_UnicodeToCharDynamic(const wchar_t* pUni, std::string& rACP);		// wchar_t -> ACP 타입 스트링 변환
extern  VOID  str_UTF8ToAnsi(char* pszCode , std::string& strOut);					// UTF8 -> ANSI 타입 스트링 변환 

extern int CalcTotalDay(int nYear, int nMonth, int nDay);
extern int GetD_Day(int nTargetYear, int nTargetMonth, int nTargetDay);

extern std::wstring wstrFromAsc( const std::string& ascstring  );					// string -> wstring
extern std::string wstrToAsc(const std::wstring& widestring);						// wstring -> ansi

extern std::wstring wstrFromCodePage( unsigned int codpage , const std::string& utf8string );
extern std::string strToCodePage(unsigned int codepage, const std::wstring& widestring);


#define _DIMENSION(a)	(sizeof(a)/sizeof(*(a)))

#define _SWAP(a, b)		{	(a)^=(b)^=(a)^=(b);	}

#define ALPHA_MIN	0
#define ALPHA_MAX	255

// SAFE_DELETE
#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif /*SAFE_DELETE(p)*/
// SAFE_DELETE_ARRAY
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif /*SAFE_DELETE_ARRAY(p)*/
// SAFE_RELEASE
#ifndef SAFE_RELEASE 
	#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif /*SAFE_RELEASE(p)*/ 


/**
 * \ingroup Utility
 * \todo 
 * 간단한 레퍼런스 카운트 관리 클래스
 * 소멸시 레퍼런스가 제대로 줄어들지 않았으면 assert
 * 레퍼런스 카운트 0xFFFF 까지만 지원
 * \bug 
 *
 */
class IReferenceAble
{
public:
	IReferenceAble( VOID )
	: m_dwRefCount(0)
	{
		m_dwRefCount++;
	}
	virtual ~IReferenceAble( VOID )
	{
		m_dwRefCount--;
		assert(m_dwRefCount == 0);
	}
public:
	 VOID  inc( VOID )	// 레퍼
	{
		m_dwRefCount++;
		assert(m_dwRefCount < 0xFFFF);
	}
	
	 VOID  dec( VOID )
	{
		if(m_dwRefCount > 1)
		{
			m_dwRefCount--;
			return;
		}

		delete this;
	}
	
	DWORD GetRef( VOID )
	{
		return m_dwRefCount;
	}

	bool IsLastRef( VOID )
	{
		if(m_dwRefCount == 1)
		{
			return true;
		}
		return false;
	}

private:
	DWORD	m_dwRefCount;
};

