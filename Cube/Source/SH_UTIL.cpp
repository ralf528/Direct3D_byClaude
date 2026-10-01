#include "stdafx.h"
#include <math.h>

//──────────────────────────────────────────────────────────────
// SH_UTIL 정적 구현부
//
// 원본 SH_UTIL.dll 은 Visual Studio 2012 의 "디버그" CRT(MSVCR110D.dll)에
// 링크되어 배포되었다. 디버그 CRT 는 재배포가 금지된 모듈이라 VS2012 가
// 설치되지 않은 PC 에서는 프로세스가 아예 로드되지 않는다(0xC0000135).
// DLL 소스가 남아있지 않으므로 SH_UTIL.h 의 선언을 그대로 지켜 정적으로
// 다시 구현한다. 이렇게 하면 SH_UTIL.dll / SH_UTIL.lib 가 더 이상 필요 없다.
//──────────────────────────────────────────────────────────────

namespace SH_UTIL
{
	namespace math
	{
		//< 점과 점 사이의 거리
		MYDLLTYPE float distancePtToPt( const POINT &pt1, const POINT &pt2 )
		{
			float dx = static_cast<float>( pt2.x - pt1.x );
			float dy = static_cast<float>( pt2.y - pt1.y );

			return sqrtf( dx * dx + dy * dy );
		}

		//< 점 이동. 목적지에 도달하면 true
		MYDLLTYPE bool moveToPt( POINT *originPos, POINT *destPos, float speed )
		{
			if( NULL == originPos || NULL == destPos )
			{
				return false;
			}

			float dx = static_cast<float>( destPos->x - originPos->x );
			float dy = static_cast<float>( destPos->y - originPos->y );
			float distance = sqrtf( dx * dx + dy * dy );

			//< 한 걸음 안에 들어오면 목적지로 붙이고 종료
			if( distance <= speed || distance <= 0.0001f )
			{
				*originPos = *destPos;
				return true;
			}

			originPos->x += static_cast<LONG>( dx / distance * speed );
			originPos->y += static_cast<LONG>( dy / distance * speed );

			return false;
		}

		//< 각도 계산(라디안, 0 ~ 2PI). 화면좌표계라 y 는 뒤집어서 계산한다.
		MYDLLTYPE float calcAnglePtToPt( POINT *destPos, POINT *srcPos )
		{
			if( NULL == destPos || NULL == srcPos )
			{
				return 0.f;
			}

			float dx = static_cast<float>( destPos->x - srcPos->x );
			float dy = static_cast<float>( srcPos->y - destPos->y );

			float angle = atan2f( dy, dx );

			if( angle < 0.f )
			{
				angle += 2.f * 3.14159265358979f;
			}

			return angle;
		}
	}//math

	namespace collision
	{
		//< 원을 감싸는 RECT 에서 중심/반지름을 뽑아내는 내부 도우미
		static void getCircleInfo( const RECT &circle, float *cx, float *cy, float *radius )
		{
			*cx     = ( circle.left + circle.right  ) * 0.5f;
			*cy     = ( circle.top  + circle.bottom ) * 0.5f;
			*radius = ( circle.right - circle.left  ) * 0.5f;
		}

		//< 점 - 사각
		MYDLLTYPE bool isColPtInRect( const POINT &pt, const RECT &rc )
		{
			return ( pt.x >= rc.left && pt.x <= rc.right &&
					 pt.y >= rc.top  && pt.y <= rc.bottom );
		}

		//< 사각 - 사각
		MYDLLTYPE bool isColRectAndRect( const RECT &rc1, const RECT &rc2 )
		{
			return !( rc1.right  < rc2.left || rc1.left > rc2.right ||
					  rc1.bottom < rc2.top  || rc1.top  > rc2.bottom );
		}

		//< 점 - 원
		MYDLLTYPE bool isColPtInCircle( const POINT &pt, const RECT &circle )
		{
			float cx, cy, radius;
			getCircleInfo( circle, &cx, &cy, &radius );

			float dx = pt.x - cx;
			float dy = pt.y - cy;

			return ( dx * dx + dy * dy ) <= ( radius * radius );
		}

		//< 원 - 원
		MYDLLTYPE bool isColCirAndCir( const RECT &cir1, const RECT &cir2 )
		{
			float cx1, cy1, r1;
			float cx2, cy2, r2;
			getCircleInfo( cir1, &cx1, &cy1, &r1 );
			getCircleInfo( cir2, &cx2, &cy2, &r2 );

			float dx = cx2 - cx1;
			float dy = cy2 - cy1;
			float sumR = r1 + r2;

			return ( dx * dx + dy * dy ) <= ( sumR * sumR );
		}

		//< 원 - 사각. 사각형에서 원 중심과 가장 가까운 점까지의 거리로 판정한다.
		MYDLLTYPE bool isColCirAndRect( const RECT &cir, const RECT &rc )
		{
			float cx, cy, radius;
			getCircleInfo( cir, &cx, &cy, &radius );

			float closestX = cx;
			float closestY = cy;

			if( closestX < rc.left   ) closestX = static_cast<float>( rc.left   );
			if( closestX > rc.right  ) closestX = static_cast<float>( rc.right  );
			if( closestY < rc.top    ) closestY = static_cast<float>( rc.top    );
			if( closestY > rc.bottom ) closestY = static_cast<float>( rc.bottom );

			float dx = cx - closestX;
			float dy = cy - closestY;

			return ( dx * dx + dy * dy ) <= ( radius * radius );
		}

		//< 선분 - 원. 선분 위에서 중심과 가장 가까운 점까지의 거리로 판정한다.
		MYDLLTYPE bool isColLineAndCir( POINT &pos1, POINT &pos2, POINT &center, float r )
		{
			float ax = static_cast<float>( pos1.x );
			float ay = static_cast<float>( pos1.y );
			float bx = static_cast<float>( pos2.x );
			float by = static_cast<float>( pos2.y );
			float px = static_cast<float>( center.x );
			float py = static_cast<float>( center.y );

			float abx = bx - ax;
			float aby = by - ay;
			float lenSq = abx * abx + aby * aby;

			float closestX = ax;
			float closestY = ay;

			//< 길이가 0 이 아니면 선분에 투영한 뒤 [0,1] 로 자른다.
			if( lenSq > 0.0001f )
			{
				float t = ( ( px - ax ) * abx + ( py - ay ) * aby ) / lenSq;

				if( t < 0.f ) t = 0.f;
				if( t > 1.f ) t = 1.f;

				closestX = ax + abx * t;
				closestY = ay + aby * t;
			}

			float dx = px - closestX;
			float dy = py - closestY;

			return ( dx * dx + dy * dy ) <= ( r * r );
		}
	}//collision

	namespace window
	{
		//< 메인 윈도우가 포커스를 가지고 있는지 여부
		static bool s_focus = true;

		//< 포커스 처리
		MYDLLTYPE void setFocusMainWindow( bool focus )
		{
			s_focus = focus;
		}

		//< 현재 포커스 확인
		MYDLLTYPE bool isFocusWindow( void )
		{
			return s_focus;
		}
	}//window

	namespace keyInput
	{
		//< 가상키 코드 최대 개수
		enum { KEY_MAX = 256 };

		//< onceKeyDown / onceKeyUp 용 이전 프레임 눌림 상태
		static bool s_keyDown[ KEY_MAX ];
		//< isToggle 용 on/off 상태
		static bool s_keyToggle[ KEY_MAX ];

		//< 현재 물리적으로 눌려 있는가(포커스가 없으면 무시)
		static bool isPhysicallyDown( int keyValue )
		{
			if( keyValue < 0 || keyValue >= KEY_MAX )
			{
				return false;
			}
			if( false == window::isFocusWindow() )
			{
				return false;
			}

			return ( GetAsyncKeyState( keyValue ) & 0x8000 ) != 0;
		}

		//< 키상태 플래그 초기화
		MYDLLTYPE void initKey( void )
		{
			ZeroMemory( s_keyDown,   sizeof( s_keyDown   ) );
			ZeroMemory( s_keyToggle, sizeof( s_keyToggle ) );
		}

		//< 키눌림 처리
		MYDLLTYPE bool isKeyDown( int keyValue )
		{
			return isPhysicallyDown( keyValue );
		}

		//< 플래그 확인. 한 번 누를 때마다 on/off 가 뒤집힌다.
		MYDLLTYPE bool isToggle( int keyValue )
		{
			if( keyValue < 0 || keyValue >= KEY_MAX )
			{
				return false;
			}

			if( onceKeyDown( keyValue ) )
			{
				s_keyToggle[ keyValue ] = !s_keyToggle[ keyValue ];
			}

			return s_keyToggle[ keyValue ];
		}

		//< 키 한번 눌림 처리(안눌림 -> 눌림 전이에서만 true)
		MYDLLTYPE bool onceKeyDown( int keyValue )
		{
			if( keyValue < 0 || keyValue >= KEY_MAX )
			{
				return false;
			}

			bool down = isPhysicallyDown( keyValue );

			if( down && false == s_keyDown[ keyValue ] )
			{
				s_keyDown[ keyValue ] = true;
				return true;
			}
			if( false == down )
			{
				s_keyDown[ keyValue ] = false;
			}

			return false;
		}

		//< 키 한번 뗌 처리(눌림 -> 안눌림 전이에서만 true)
		MYDLLTYPE bool onceKeyUp( int keyValue )
		{
			if( keyValue < 0 || keyValue >= KEY_MAX )
			{
				return false;
			}

			bool down = isPhysicallyDown( keyValue );

			if( down )
			{
				s_keyDown[ keyValue ] = true;
				return false;
			}
			if( s_keyDown[ keyValue ] )
			{
				s_keyDown[ keyValue ] = false;
				return true;
			}

			return false;
		}
	}//keyInput

}//< namespace end
