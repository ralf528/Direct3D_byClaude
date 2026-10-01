#include "stdafx.h"
#include "VoidList.h"
#include "Utility.h"

cVoidList::cVoidList( VOID )
{
}

cVoidList::~cVoidList( VOID )
{
	forceClear();
}

 VOID  cVoidList::push_back( LPVOID  pData )
{
	m_cacheList.push_back( pData );
}

 VOID  cVoidList::push_front( LPVOID  pData )
{
	m_cacheList.push_front( pData );
}

LPVOID  cVoidList::pop_front( VOID )
{
	if ( m_cacheList.empty() )
		return NULL;

	VOID_LIST_ITOR	iBegin	= m_cacheList.begin();
	LPVOID 				pData	= ( *iBegin );

	m_cacheList.pop_front();

	return pData;
}

LPVOID  cVoidList::pop_back( VOID )
{
	if ( m_cacheList.empty() )
		return NULL;

	VOID_LIST_RITOR		iBegin	= m_cacheList.rbegin();
	LPVOID 				pData	= ( *iBegin );

	m_cacheList.pop_back();

	return pData;
}

 VOID  cVoidList::forceClear( VOID )
{
	m_cacheList.clear();
}

BOOL cVoidList::first( LPVOID * LPVOID  )
{
	if ( m_cacheList.empty() )
		return FALSE;

	m_iLoop		= m_cacheList.begin();
	*LPVOID 		= ( *m_iLoop );

	return TRUE;
}

bool cVoidList::eraseF(LPVOID * LPVOID  /*= NULL*/)
{
	m_iLoop = m_cacheList.erase( m_iLoop );

	if ( m_iLoop == m_cacheList.end() )
	{
		return false;
	}

	if(LPVOID )
	{
		*LPVOID 		= ( *m_iLoop );
	}
	return true;
}

BOOL cVoidList::next( LPVOID * pointer  )
{
	if ( m_cacheList.empty() )
	{
		return FALSE;
	}

	m_iLoop++;

	if ( m_iLoop == m_cacheList.end() )
	{
		return FALSE;
	}

	*pointer 		= ( *m_iLoop );

	return TRUE;
}

int cVoidList::getSize( VOID )
{
	return ( int ) m_cacheList.size();
}

BOOL cVoidList::empty( VOID )
{
	return m_cacheList.empty();
}

//< 모두지우기
VOID cVoidList::clear( VOID )
{
	m_cacheList.clear();
}

BOOL cVoidList::last( LPVOID * pLPVOID  )				// Back Itor	
{
	if ( m_cacheList.empty() )
		return FALSE;

	m_irLoop		= m_cacheList.rbegin();
	*pLPVOID 		= ( *m_irLoop );

	return TRUE;
}

 VOID  cVoidList::eraseR( VOID )
{
	//	m_cacheList.erase( m_irLoop ++ );
}

BOOL cVoidList::prev( LPVOID * pLPVOID  )
{
	if ( m_cacheList.empty() )
		return FALSE;

	m_irLoop ++;									// 함수 호출후 증가 시킨다.

	if ( m_irLoop == m_cacheList.rend() )
		return FALSE;

	*pLPVOID 		= ( *m_irLoop );

	return TRUE;
}

// 동기화 리스트 ------------------------------------------------------------------

cVoidSectionList::cVoidSectionList( VOID )
{
	SH_NEW_PTR( CRITICAL_SECTION, m_pCriticalSection );
	InitializeCriticalSection( m_pCriticalSection );

}

cVoidSectionList::~cVoidSectionList( VOID )
{
	DeleteCriticalSection( m_pCriticalSection );

	SH_DEL_PTR( m_pCriticalSection );
}

 VOID  cVoidSectionList::push_back( LPVOID  pData )
{
	EnterSection();
	cVoidList::push_back( pData );
	LeaveSection();
}

 VOID  cVoidSectionList::push_front( LPVOID  pData )
{
	EnterSection();
	cVoidList::push_front( pData );
	LeaveSection();
}

LPVOID  cVoidSectionList::pop_front( VOID )
{
	LPVOID  pRetData = NULL;
	EnterSection();
	pRetData = cVoidList::pop_front();
	LeaveSection();
	return pRetData;
}

LPVOID  cVoidSectionList::pop_back( VOID )
{
	LPVOID  pRetData = NULL;
	EnterSection();
	pRetData = cVoidList::pop_back();
	LeaveSection();
	return pRetData;
}

 VOID  cVoidSectionList::forceClear( VOID )
{
	EnterSection();
	cVoidList::forceClear();
	LeaveSection();
}

 VOID  cVoidSectionList::EnterSection( VOID )
{
	EnterCriticalSection( m_pCriticalSection );
}

 VOID  cVoidSectionList::LeaveSection( VOID )
{
	LeaveCriticalSection( m_pCriticalSection );
}

int cVoidSectionList::getSize( VOID )
{
	int retSize = 0;

	EnterSection();
	retSize = cVoidList::getSize();
	LeaveSection();
	return retSize;
}

BOOL cVoidSectionList::empty( VOID )
{
	BOOL bEmpty = FALSE;

	EnterSection();
	bEmpty = cVoidList::empty();
	LeaveSection();
	return bEmpty;
}
