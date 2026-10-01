#ifdef DLLEXPORT
#define MYDLLTYPE __declspec(dllexport)
#else
#define MYDLLTYPE __declspec(dllimport)
#endif

/*
	프로젝트에 공용으로 사용되는 함수
*/
namespace SH_UTIL
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─ 2015.04, 수학관련, Ackashic. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	namespace math
	{		
		//< 두점사이의 거리
		extern "C" MYDLLTYPE float	distancePtToPt( const POINT &pt1, const POINT &pt2 );
		//< 점 이동
		extern "C" MYDLLTYPE bool	moveToPt(POINT *originPos, POINT *destPos,float speed = 3.f);
		//< 각도 계산
		extern "C" MYDLLTYPE float	calcAnglePtToPt(POINT *destPos, POINT *srcPos);
	}//math
	    
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─ 2015.04, 충돌관련, Ackashic. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	namespace collision
	{
		//< 점 사각
		extern "C" MYDLLTYPE bool	 isColPtInRect( const POINT &pt , const RECT &rc );
		//< 사각 사각
		extern "C" MYDLLTYPE bool	 isColRectAndRect( const RECT &rc1, const RECT &rc2 );
		//< 원 점
		extern "C" MYDLLTYPE bool	 isColPtInCircle( const POINT &pt, const RECT &circle );
		//< 원 원
		extern "C" MYDLLTYPE bool	 isColCirAndCir( const RECT &cir1, const RECT &cir2 );
		//< 원 사각
		extern "C" MYDLLTYPE bool	 isColCirAndRect( const RECT &cir, const RECT &rc );
		//선과 원의 충돌 체크
		extern "C" MYDLLTYPE bool	isColLineAndCir( POINT &pos1, POINT &pos2, POINT &center , float r );
	}//collision

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─ 2015.04, 윈도우 관련, Ackashic. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	namespace window
	{
		//< 포커스처리
		extern "C" MYDLLTYPE void			setFocusMainWindow( bool focus );
		//< 메인 윈도우포커스확인
		extern "C" MYDLLTYPE bool			isFocusWindow( void );
	}//window
	
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─ 2015.04, 키입력 관련, Ackashic. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	namespace keyInput
	{
		//< 키모든 플래그 초기화
		extern "C" MYDLLTYPE void		initKey( void );
		//< 키눌림처리
		extern "C" MYDLLTYPE bool		isKeyDown( int keyValue );
		//< 플래그확인
		extern "C" MYDLLTYPE bool		isToggle( int keyValue );
		//< 키 한번 누름처리
		extern "C" MYDLLTYPE bool		onceKeyDown( int keyValue );
		//< 키 한번 땜처리
		extern "C" MYDLLTYPE bool		onceKeyUp( int keyValue );
	}//keyInput

}//< namespace end