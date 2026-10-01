#pragma once
#include <Windows.h>
#include "VoidList.h"

////	메모리 구조체
struct	FS_Mem
{
	//	버퍼 포인터
	BYTE	*m_Buffer;
	//	버퍼 크기
	int		m_Size;
	//	버퍼 타입
	int		m_Type;
	//	소스 정보
	int		m_Line;
	char	m_Source[260];
	//	다음 메모리
	FS_Mem	*m_Next;
};

// 메모리 관리자용
typedef struct MEM_DATA
{
	LPVOID 	pData;
	char	PositionString[MAX_PATH];
}*LPMEM_DATA;

class SHHashManager
{
	cVoidList*			m_pListBucket;
	int 				m_nBucketCount;
	int					m_nRSCCount;
	int					m_nTotalMemCount;

	char				m_szManagerName[MAX_PATH];

	cVoidList			m_MemDataCache;

public :
	SHHashManager( VOID );
	virtual ~SHHashManager( VOID );

protected :
	///<				관리 메모리 데이터 꺼내기 
	LPMEM_DATA			PopMemoryData( VOID );

	///<				관리 메모리 데이터 넣기 
	 VOID 				PushMemoryData( LPMEM_DATA pMemData );

	///<				해당 관리 오브젝트 찾기 
	BOOL				FindObject( LPVOID  pObject );



	///< c형 스타일 포인터 관리 (malloc , vAlloc)
	///<				메모리 관리 시작 함수
	BOOL				F_Start_Memory();

	///<				메모리 관리 종료(할당하고 종료되지 않은 메모리 제거)
	BOOL				F_End_Memory(bool	p_Type = false);

	///<				메모리 누수알림 
	 VOID 				OutputMessageBox( VOID );

public :
	///<				매니져 초기화 
	BOOL				InitManager( LPSTR pszManagerName, int bucketCount = 100 );
	///<				메모리 관리 매니져 종료 
	 VOID 				CloseManager( VOID );
	
	///<				관리 오브젝트 추가 
	BOOL				AddObject( LPVOID  pObject,  LPSTR pfile, int LineNo );

	///<				관리 오브젝트 제거
	BOOL				RemoveObject( LPVOID  pObject );
	
	///<				리소스 수량 얻기 
	int					GetResourceCount( VOID ) { return m_nRSCCount; }
	
	///<				현재 메모리 버퍼 할당 정보 출력
	BOOL				F_Current_Memory();

	///<				메모리 생성 함수
	BYTE*				SH_MAlloc_S(int p_Size,int p_MType,const char	*p_Source,int p_Line);

	///<				메모리 삭제 함수
	BOOL				F_Free(BYTE *p_Buffer);
};

extern SHHashManager*		GetMemManager( VOID );

///< c형 스타일 
#define	SH_MALLOC				0
#define	SH_VALLOC				1
#define	SH_Alloc(a)					 GetMemManager()->SH_MAlloc_S(a,SH_MALLOC,__FILE__,__LINE__) 
#define	SH_VAlloc(a)				 GetMemManager()->SH_MAlloc_S(a,SH_VALLOC,__FILE__,__LINE__) 
#define SH_FREE(a)					 GetMemManager()->F_Free((BYTE*)a)

// 디버그 일때만 메모리 누수 탐지관리자에 집어 넣는다.
#ifdef _DEBUG

#define SH_NEW_PTR( t, p )			{ p = new t; GetMemManager()->AddObject( p, __FILE__, __LINE__ ); }
#define SH_DEL_PTR( p )				{ if ( p!=NULL) { GetMemManager()->RemoveObject( p); delete p; p=NULL;} }
#define SH_NEW_PTR_ARRAY( t, p, s )	{ p = new t[s]; GetMemManager()->AddObject( p, __FILE__, __LINE__ ); }
#define SH_DEL_PTR_ARRAY( p )		{ if ( p!=NULL) { GetMemManager()->RemoveObject( p); delete [] p; p=NULL;} }

#else

#define SH_NEW_PTR( t, p )			{ p = new t; }
#define SH_DEL_PTR( p )				{ if ( p!=NULL) { delete p; p=NULL;} }

#define SH_NEW_PTR_ARRAY( t, p,s )	{ p = new t[s]; }
#define SH_DEL_PTR_ARRAY( p )		{ if ( p!=NULL) { delete [] p; p=NULL;} }

#endif

#define SH_SAFE_RELEASE(p)				{ if(p) { (p)->Release(); (p)=NULL; } }

/*
	사용법 : 
	c++스타일 
	일반 할당 SH_NEW_PTR( t, p )
	일반 삭제 SH_DEL_PTR( p )
	
	배열 할당
	배열 삭제 

	C형 스타일 
	일반 할당	SH_Alloc(a)		 
	ex)
	int* a = SH_Alloc(sizeof(int));

	대용량 할당 SH_VAlloc(a)	
	int* a = SH_VAlloc(100);
	
	일반 할당과 대용량 할당 모두 SH_FREE로 삭제한다.
	SH_FREE(a);
*/



