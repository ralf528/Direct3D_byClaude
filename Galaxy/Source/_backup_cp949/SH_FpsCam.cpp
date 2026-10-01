#include "StdAfx.h"
#include "SH_FpsCam.h"

namespace SH3D
{
//< 마우스 회전 감도 
const float MOUSE_SENSITIVITY = 800.0f;

SH_FpsCam::SH_FpsCam(void)
:SH_iCamera( CAM_FPS )
{
}

SH_FpsCam::~SH_FpsCam(void)
{
}

//< 카메라 설정
HRESULT	SH_FpsCam::initCam( D3DXVECTOR3 const &eye, 
										D3DXVECTOR3 const &up,
										D3DXVECTOR3 const &lookAt)
{
	//< 기본설정
	if( SH_iCamera::initCam( eye, up, lookAt ) != S_OK )
	{
		return S_FALSE;
	}

	//< FPS설정
	setFPS();

	return S_OK;
}

void	SH_FpsCam::update( float dt )
{
	//< 전후진
	if( keyInput::isKeyDown( 'W' ) == true )
	{
		moveForBackWard( m_moveSpeed );
	}

	if( keyInput::isKeyDown( 'S' ) == true )
	{
		moveForBackWard( -m_moveSpeed );
	}

	//< 게걸음
	if( keyInput::isKeyDown( 'A' ) == true )
	{
		moveLeftRight( -m_moveSpeed );
	}

	if( keyInput::isKeyDown( 'D' ) == true )
	{
		moveLeftRight( m_moveSpeed );
	}

	//if( keyInput::isKeyDown( MK_RBUTTON) == true )
	//{
		checkRot();
	//}
}

void	SH_FpsCam::setFPS( void )
{
	//< 마우스 좌표얻고
	RECT	rcWin;
	//< 윈도우 크기 얻기
	GetClientRect( g_hWnd,&rcWin);
	
	//< 중앙좌표설정
	m_winCenterPos.x = (rcWin.right + rcWin.left)/2;
	m_winCenterPos.y = (rcWin.bottom + rcWin.top)/2;

	//< 스크린 좌표 
	ClientToScreen( g_hWnd ,&m_winCenterPos);

	//< 커서숨기기 
	ShowCursor(FALSE);

	//< 마우스 좌표 이동 
	SetCursorPos(m_winCenterPos.x, m_winCenterPos.y);
}

//< 커서정보 설정
void	SH_FpsCam::checkRot( void )
{
	POINT curMouse;

	//< 현재 마우스 얻기
	GetCursorPos(&curMouse);

	int nX = curMouse.x - m_winCenterPos.x;
	int nY = curMouse.y - m_winCenterPos.y;

	rotYaw(	nX / MOUSE_SENSITIVITY );
	rotPitch(	nY / MOUSE_SENSITIVITY );

	//< 정위치로 
	SetCursorPos(m_winCenterPos.x, m_winCenterPos.y);	
}

bool	SH_FpsCam::mouseMessage( HWND wnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	switch(message)
	{
	case WM_MOUSEWHEEL:
		{
			return SH_iCamera::mouseMessage(wnd,message,wparam,lparam);
		}
		
	//case WM_MOUSEMOVE:
	//	{
	//		//POINT curMouse;

	//		////< 현재 마우스 얻기
	//		//GetCursorPos(&curMouse);

	//		//int nX = curMouse.x - m_winCenterPos.x;
	//		//int nY = curMouse.y - m_winCenterPos.y;

	//		//rotYaw(	nX / MOUSE_SENSITIVITY );
	//		//rotPitch(	nY / MOUSE_SENSITIVITY );

	//		////< 정위치로 
	//		//SetCursorPos(m_winCenterPos.x, m_winCenterPos.y);		
	//	}
	//	return true;
	}

	return false;
}

}//< namespace SH3D