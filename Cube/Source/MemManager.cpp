#include "stdafx.h"
#include "Utility.h"
#include "MemManager.h"

///< 싱글톤 정적변수 초기화 
SHHashManager*	g_CommonMemManager	= NULL;

///< 생성자 
SHHashManager* GetMemManager( VOID )
{
	///< 메모리 매니져가 생성되지않앗을때생성
	if ( g_CommonMemManager == NULL )
	{
		g_CommonMemManager	= new SHHashManager;

		///<메모리 매니져 해쉬 (bucket생성)
		g_CommonMemManager->InitManager( "MEM_MANAGER",10000 );
	}
	return g_CommonMemManager;
}
// 메모리 관리자
SHHashManager::SHHashManager( VOID )
{
	m_pListBucket		= NULL;
	m_nBucketCount		= 0;
	m_nRSCCount			= 0;
	m_szManagerName[0]	= NULL;
	m_nTotalMemCount	 = 0;
}

SHHashManager::~SHHashManager( VOID )
{
	///< 싱글톤으로 구현한 클래스는 명시적으로 죽일때가 아니면
	///< 소멸자가 호출되지 않기 때문에 이렇게 호출해줘야 제대로 체크가된다.(주의할것)!!!!
	///< 메모리 관리 클래스 호출 
	/// CloseManager();
}

///< 메모리 꺼내기 
LPMEM_DATA SHHashManager::PopMemoryData( VOID )
{
	///< 메모리 관리자용 구조체 선언 
	LPMEM_DATA pMemData	= NULL;
	
	///< 현재 메모리가 비어있다면 
	if ( m_MemDataCache.empty() )
	{
		pMemData = new MEM_DATA;
		return pMemData;
	}
	
	///< 앞에서 꺼내기 
	pMemData = ( LPMEM_DATA ) m_MemDataCache.pop_front();
	return pMemData;
}

///< 메모리 집어넣기 (뒤에넣기 )
 VOID  SHHashManager::PushMemoryData( LPMEM_DATA pMemData )
{
	m_MemDataCache.push_back( pMemData );
}

///< 매니져 초기화 ( 해당 이름과 , 버켓개수만큼 생성)
BOOL SHHashManager::InitManager( LPSTR pszManagerName, int bucketCount )
{
	if( pszManagerName == NULL )
	{
		return FALSE;
	}

	int nLen = strlen(pszManagerName );
	///< 매니져 이름 복사 
	strcpy_s( m_szManagerName, nLen + 1, pszManagerName );

	m_nBucketCount = bucketCount;		///< 버켓 카운트 설정 
	m_nRSCCount			= 0;			///< 리소스개수 초기화 
	m_pListBucket = new cVoidList[ m_nBucketCount ];	///< 버켓개수만큼  VOID List생성

	///< 메모리초기화
	LPMEM_DATA	pMemData = NULL;	
	
#ifdef _DEBUG
	///< 버켓 카운트 만큼 메모리 관리자 생성해서 넣기
	for ( int i=0;i< bucketCount;i++)
	{
		pMemData = new MEM_DATA;
		m_MemDataCache.push_back( pMemData );
	}
#else
	m_MemDataCache.clear();
#endif

	return TRUE;
}

///< 매니저 종료 
 VOID  SHHashManager::CloseManager( VOID )
{
#ifdef _DEBUG
	///< 할당 되어있다면 
	if ( m_pListBucket != NULL )
	{
		///< 리소스 개수 확인 (남아있는 개수 확인 )
		if(m_nRSCCount)
		{
			m_nTotalMemCount+= m_nRSCCount;

			//	메모리 정보 출력
// 			char	Temp_S[260];
// 			wsprintfA(Temp_S,"삭제안된 C++형 %d개의 메모리가 있어서 강제 삭제함",m_nRSCCount);
// 			::MessageBoxA(NULL,Temp_S,"확인",MB_OK);
			PrintDebugStr("\n============================================================================================\n");
			PrintDebugStr("C++ Memory leak detected!!! remain count : %d\n", m_nRSCCount  );
		}
		
		///< 버켓 개수 만큼 돌면서 리스트가 비어있지 않다면 해당 메모리의 릭을 처리한다.
		for ( int i = 0; i < m_nBucketCount; i++ )
		{
			if ( FALSE == m_pListBucket[i].empty() )
			{
				LPMEM_DATA pData	= NULL;
				BOOL			bHave	= m_pListBucket[i].first( (LPVOID *) &pData );
				while ( bHave )
				{
					PrintDebugStr("Memory leak detected!!!!, %s\n", pData->PositionString  );
					PrintDebugStr("============================================================================================\n\n");
					SAFE_DELETE( pData );
					bHave = m_pListBucket[i].next( (LPVOID *) &pData );
				}
			}
			m_pListBucket[i].forceClear();
		}
	}

	LPMEM_DATA pData	= NULL;
	BOOL			bHave	= m_MemDataCache.first( (LPVOID *) &pData );
		
	while ( bHave == TRUE )
	{
		SAFE_DELETE( pData );
		bHave = m_MemDataCache.next( (LPVOID *) &pData );
	}

	SAFE_DELETE_ARRAY( m_pListBucket );

	///< C형 스타일 모두 지우기 
	F_End_Memory(true);

	///< 누수 메모리 메세지 박스로 알림 
	OutputMessageBox();

#endif
	//< 
	m_MemDataCache.clear();
}

///< 해쉬에 저장된 오브젝트 찾기 
BOOL SHHashManager::FindObject( LPVOID  pObject )
{
#pragma warning ( disable : 4311 )
	DWORD Key = (DWORD) pObject;
#pragma warning ( default : 4311 )
	Key %= m_nBucketCount;

	LPMEM_DATA	pMemData	= NULL;
	BOOL		bHave		= m_pListBucket[Key].first( (LPVOID *) &pMemData );
	while ( bHave )
	{
		if ( pMemData->pData == pObject )
		{
			return TRUE;
		}
		bHave		= m_pListBucket[Key].next( (LPVOID *) &pMemData );
	}

	return FALSE;
}

///< 오브젝트 추가 
BOOL SHHashManager::AddObject( LPVOID  pObject, LPSTR pFile, int LineNo )
{
	///< 같은 오브젝트가 있는지 확인후 추가한다.
	BOOL bFind  = SHHashManager::FindObject( pObject );
	if ( bFind )
	{
/*			__asm int 3;*/
		return FALSE;
	}

#pragma warning ( disable : 4311 )
	DWORD Key = (DWORD) pObject;
#pragma warning ( default : 4311 )
	Key %= m_nBucketCount;

	LPMEM_DATA pMemData = PopMemoryData();
	pMemData->pData = pObject;
	sprintf_s( pMemData->PositionString, _MAX_FNAME, "FILE : %s, LINE : %d", pFile, LineNo );
	m_pListBucket[Key].push_back( pMemData );

	m_nRSCCount++;

	return TRUE;
}

///< 오브젝트 제거 
BOOL SHHashManager::RemoveObject( LPVOID  pObject )
{
#pragma warning ( disable : 4311 )
	DWORD Key = (DWORD) pObject;
#pragma warning ( default : 4311 )
	///< 키값구하기 
	Key %= m_nBucketCount;

	LPMEM_DATA	pMemData	= NULL;
	BOOL		bHave		= m_pListBucket[Key].first( (LPVOID *) &pMemData );
	while ( bHave )
	{
		if ( pMemData->pData == pObject )
		{
			m_pListBucket[Key].eraseF();
			pMemData->pData = NULL;
			pMemData->PositionString[0]= NULL;
			m_MemDataCache.push_back( pMemData );
			m_nRSCCount--;
			return TRUE;
		}
		bHave		= m_pListBucket[Key].next( (LPVOID *) &pMemData );
	}

	return FALSE;
}

//////////////////////////////////////// c스타일 메모리 관리 //////////////////////////////////////////////////

////	기준 메모리
FS_Mem		*R_Mem = NULL;

////	메모리 관리 시작 함수
BOOL  SHHashManager::F_Start_Memory()
{
	//	메모리 생성 확인
	if(R_Mem != NULL)
		return true;
	//	할당.
	R_Mem = (FS_Mem *)malloc(sizeof(FS_Mem));
	//	초기화
	memset(R_Mem,0,sizeof(FS_Mem));
	return true;
}

////	메모리 관리 종료(할당하고 종료되지 않은 메모리 제거)
BOOL  SHHashManager::F_End_Memory(bool	p_Type)
{
	//	기준 데이터 확인.
	if(R_Mem == NULL)
		return true;
	//	임시 리스트 얻기
	FS_Mem	*t_List = R_Mem;
	FS_Mem	*t_Del = NULL;
	//	반복 처리
	int	t_Err = 0;
	while(t_List != NULL)
	{
		//	커운트 증가
		t_Err++;
		//	삭제 부분 받기
		t_Del = t_List;
		//	다음 리스트 지정
		t_List = t_List->m_Next;
		//	삭제
		if(t_Del->m_Buffer != NULL)
		{
			switch(t_Del->m_Type)
			{
			case SH_MALLOC:
				free(t_Del->m_Buffer);
				break;
			case SH_VALLOC:
				VirtualFree(t_Del->m_Buffer,0,MEM_DECOMMIT);
				break;
			default:
				free(t_Del->m_Buffer);
				break;
			}
		}
		//	메모리 정보 출력
		if(t_Err > 1)
		{
			if(p_Type)
			{
#ifdef _DEBUG
				char	Temp_S[260];
				wsprintfA(Temp_S,"FILE : %s LINE :%d ",t_Del->m_Source,t_Del->m_Line);
				PrintDebugStr("\n============================================================================================\n");
				PrintDebugStr("C Memory leak detected!!!  %s\n", Temp_S  );
				PrintDebugStr("============================================================================================\n\n");

 				//wsprintfA(Temp_S,"%d Line, %s",t_Del->m_Line,t_Del->m_Source);
// 				::MessageBoxA(NULL,Temp_S,"확인",MB_OK);
#endif
			}
		}
		//	해당 리스트 삭제
		free(t_Del);
	}
	//	초기화
	R_Mem = NULL;

	//	미삭제 메모리 인경우 오류 확인 필요
	if(t_Err > 1)
	{
		//	메모리 정보 출력
		if(p_Type)
		{

 #ifdef _DEBUG

			PrintDebugStr("\n============================================================================================\n");
			PrintDebugStr("C Memory leak detected!!! remain count : %d\n", t_Err - 1  );
 		//	메모리 정보 출력
 		//wsprintfA(Temp_S,"삭제안된 %d개의 메모리가 있어서 강제 삭제함",t_Err - 1);
 		//::MessageBoxA(NULL,Temp_S,"확인",MB_OK);
		///< 전체 메모리 개수 추가 
		m_nTotalMemCount+= t_Err - 1;
 #endif
		
		}
	}
	return true;
}

////	현재 메모리 버퍼 할당 정보 출력
BOOL  SHHashManager::F_Current_Memory()
{
	//	기준 데이터 확인.
	if(R_Mem == NULL)
		return true;
	//	임시 리스트 얻기
	FS_Mem	*t_List = R_Mem->m_Next;
	int	t_Count = 0;
	DWORD t_Size = 0;
	while(t_List != NULL)
	{
		if(t_List->m_Buffer != NULL)
		{
			t_Count++;
			t_Size = t_Size + t_List->m_Size;
		}
		//	다음 리스트 지정
		t_List = t_List->m_Next;
	}


	return true;
}

////	메모리 생성 함수
BYTE*  SHHashManager::SH_MAlloc_S(int p_Size,int p_MType,const char	*p_Source,int p_Line)
{
	//	전달 인자 확인
	if(p_Size < 1)
		return NULL;

	//	초기화 확인
	F_Start_Memory();

	//	리스트 생성
	FS_Mem	*t_List = (FS_Mem *)malloc(sizeof(FS_Mem));
	if(t_List == NULL)
		return NULL;
	//	리스트 초기화
	memset(t_List,0,sizeof(FS_Mem));

	//	메모리 생성
	t_List->m_Type = p_MType;
	switch(t_List->m_Type)
	{
	case SH_MALLOC:
		t_List->m_Buffer = (BYTE *)malloc(p_Size);
		break;
	case SH_VALLOC:
		t_List->m_Buffer = (BYTE *)VirtualAlloc(NULL,p_Size,MEM_COMMIT | MEM_RESERVE,PAGE_READWRITE);
		break;
	default:
		t_List->m_Buffer = (BYTE *)malloc(p_Size);
		break;
	}
	//	버퍼 오류 확인
	if(t_List->m_Buffer == NULL)
	{
		free(t_List);
		return NULL;
	}
	//	버퍼 초기화
	memset(t_List->m_Buffer,0,p_Size);
	t_List->m_Size = p_Size;
	t_List->m_Next = NULL;
	if(p_Source != NULL)
	{
		t_List->m_Line = p_Line;
		memcpy(t_List->m_Source,p_Source,lstrlenA(p_Source));
	}

	//	리스트에 추가
	FS_Mem	*t_Temp = R_Mem;
	//	마지막 위치 찾기
	while(t_Temp != NULL)
	{
		// 마지막 확인.
		if(t_Temp->m_Next == NULL)
		{
			t_Temp->m_Next = t_List;
			//	결과 반환
			return t_List->m_Buffer;
		}
		else
		{
			//	다음 검사.
			t_Temp = t_Temp->m_Next;
		}
	}
	//	결과 반환
	return t_List->m_Buffer;
}

////	메모리 삭제 함수
BOOL  SHHashManager::F_Free(BYTE *p_Buffer)
{
	//	전달 인자 확인
	if(p_Buffer == NULL)
		return false;
	//	리스트에서 찾기
	FS_Mem	*t_Temp = R_Mem->m_Next;
	FS_Mem	*t_Pre = R_Mem;

	//	마지막 까지 검사
	while(t_Temp != NULL)
	{
		//	삭제 대상인지 확인.
		if(t_Temp->m_Buffer == p_Buffer)
		{
			// 링크 연결 변경.
			t_Pre->m_Next = t_Temp->m_Next;
			// 메모리 삭제
			switch(t_Temp->m_Type)
			{
			case SH_MALLOC:
				free(t_Temp->m_Buffer);
				break;
			case SH_VALLOC:
				VirtualFree(t_Temp->m_Buffer,0,MEM_DECOMMIT);
				break;
			default:
				free(t_Temp->m_Buffer);
				break;
			}
			free(t_Temp);
			return true;
		}
		//	다음 검사
		t_Pre = t_Temp;
		t_Temp = t_Temp->m_Next;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

///<				메모리 누수알림 
 VOID   SHHashManager::OutputMessageBox( VOID )
{
#ifdef _DEBUG
	//	메모리 정보 출력
	if( m_nTotalMemCount != 0 )
	{
		char	Temp_S[256];
		//	메모리 정보 출력
		wsprintfA(Temp_S,"삭제안된 %d개의 메모리가 있어서 강제 삭제함",m_nTotalMemCount);
		::MessageBoxA(NULL,Temp_S,"확인",MB_OK);
	}
#endif
}
