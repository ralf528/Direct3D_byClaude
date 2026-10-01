#pragma once
#include <list>
// 범용적인 리스트 클래스

class cVoidList
{
private:
	typedef std::list < LPVOID  >		 VOID_LIST;
	typedef VOID_LIST::iterator			 VOID_LIST_ITOR;
	typedef VOID_LIST::reverse_iterator	 VOID_LIST_RITOR;

	 VOID_LIST					m_cacheList;
	 VOID_LIST_ITOR				m_iLoop;
	 VOID_LIST_RITOR			m_irLoop;

public:
	cVoidList( VOID );
	virtual ~cVoidList( VOID );

	VOID 						push_back( LPVOID  pData );
	VOID 						push_front( LPVOID  pData );
	LPVOID 						pop_front( VOID );
	LPVOID 						pop_back( VOID );
	VOID 						forceClear( VOID );

	BOOL						first( LPVOID * lpVoid  );
	bool						eraseF(LPVOID * lpVoid  = NULL);
	BOOL						next( LPVOID * lpVoid  );

	BOOL						last( LPVOID * lpVoid  );
	VOID 						eraseR( VOID );
	BOOL						prev( LPVOID * lpVoid  );

	int							getSize( VOID );
	BOOL						empty( VOID );
	//< 모두지우기
	VOID						clear( VOID );
};

// 동기화 리스트 ---------------------------------------------------------------------

class cVoidSectionList : public cVoidList
{
	LPCRITICAL_SECTION				m_pCriticalSection;
public :
	cVoidSectionList( VOID );
	virtual ~cVoidSectionList( VOID );


	virtual VOID 				push_back( LPVOID  pData );
	virtual VOID 				push_front( LPVOID  pData );
	virtual LPVOID 				pop_front( VOID );
	virtual LPVOID 				pop_back( VOID );
	virtual VOID 				forceClear( VOID );

public : 	
	virtual int					getSize( VOID );
	virtual BOOL				empty( VOID );

public :
	VOID 						EnterSection( VOID );
	VOID 						LeaveSection( VOID );
};


