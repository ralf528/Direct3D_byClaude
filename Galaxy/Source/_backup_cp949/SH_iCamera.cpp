#include "StdAfx.h"
#include "SH_iCamera.h"

namespace SH3D
{

const float CAM_MOVESPEED = 0.2f;

SH_iCamera::SH_iCamera( CAM_KIND camType )
:m_camKind( camType )
{
	m_moveSpeed = CAM_MOVESPEED;
}

SH_iCamera::~SH_iCamera(void)
{
}

//< 카메라 설정
HRESULT		SH_iCamera::initCam(D3DXVECTOR3 const &eye, 
								D3DXVECTOR3 const &up,
								D3DXVECTOR3 const &lookAt)
{
	//< 디바이스 설정
	m_d3dDevice9 = GAME_MGR->getDevice();

	//< 기본정보설정
	m_lookAt	= lookAt;
	m_eye		= eye;
	m_up		= up;

	//< 방향벡터
	updateDir();
	//< 우향벡터
	updateRight();

	//< 뷰행렬업데이트
	updateView();
	//< 프로젝션 설정
	updateProj();
	return S_OK;
}

////< 업데이트
//void		SH_iCamera::update( float dt )
//{
//	//< x회전테스트
//	if( SHUTIL::isKeyDown( 'W' ) == true )
//	{
//		rotPitch( -0.01f );
//	}
//
//	if( SHUTIL::isKeyDown( 'S' ) == true )
//	{
//		rotPitch( 0.01f );
//	}
//	
//	//< y회전 테스트
//	if( SHUTIL::isKeyDown( 'A' ) == true )
//	{
//		rotYaw( -0.01f );
//	}
//
//	if( SHUTIL::isKeyDown( 'D' ) == true )
//	{
//		rotYaw( 0.01f );
//	}
//
//	//< 전후진
//	if( SHUTIL::isKeyDown( 'Q' ) == true )
//	{
//		moveLeft();
//	}
//
//	if( SHUTIL::isKeyDown( 'E' ) == true )
//	{
//		moveRight();
//	}
//
//
//	//< 전후진
//	if( SHUTIL::isKeyDown( VK_UP ) == true )
//	{
//		moveForWard();
//	}
//
//	if( SHUTIL::isKeyDown( VK_DOWN ) == true )
//	{
//		moveBackWard();
//	}
//}

//< 카메라 전/후진
void		SH_iCamera::moveForBackWard( float moveSpeed  )
{
	//< 눈의 위치 이동
	m_eye += m_dir * moveSpeed;
	//< 시점이동
	m_lookAt +=m_dir * moveSpeed;
	//< 뷰갱신
	updateView();
}
// void		SH_iCamera::moveBackWard( void )
// {
// 	//< 눈의 위치 이동
// 	m_eye -= m_dir * m_moveSpeed;
// 	//< 시점이동
// 	m_lookAt -=m_dir * m_moveSpeed;
// 	//< 뷰갱신
// 	updateView();
// }
//< 카메라 게걸음(좌/우)
void		SH_iCamera::moveLeftRight( float moveSpeed )
{
	//< 눈의 위치 이동
	m_eye += m_right * moveSpeed;
	//< 시점이동
	m_lookAt +=m_right * moveSpeed;
	//< 뷰갱신
	updateView();
}

//< yaw(Y회전) : 상향(업) 벡터 기준 회전(요)
void		SH_iCamera::rotYaw( float angle )
{
	//< 회전 결과 행렬
	D3DXMATRIX	  yaw;
	//< 방향벡터
	updateDir();
	
	//< 상향(업)벡터를 기준으로 회전한다.
	D3DXMatrixRotationAxis(&yaw, &m_up, angle);//
	D3DXVec3TransformCoord(&m_dir, &m_dir, &yaw);

	m_lookAt =   m_eye + m_dir; 
	
	//< 뷰적용
	updateView();
}
//< pitch(X회전) : 우향 벡터 기준 회전(피치)
void		SH_iCamera::rotPitch( float angle )
{
	//< 업벡터와 방향벡터를 내적해서 짐벌락막기
	float dotAngle = D3DXVec3Dot( &m_dir, &m_up );

	//< 제한각도는 지정해서 사용
	if( ( dotAngle < -0.95  && 0 < angle ) || 
		( 0.95 < dotAngle &&  angle < 0 ) )
	{
		return;
	}

	D3DXMATRIX	  pitch;

	//< 방향벡터
	updateDir();
	//< 우향벡터
	updateRight();

	//< 우향벡터를 기준으로 회전한다.
	D3DXMatrixRotationAxis( &pitch, &m_right, angle);
	//< 회전한 값을 방향벡터에 반영한다.
	D3DXVec3TransformCoord( &m_dir, &m_dir, &pitch);
	
	//< 시점이동
	m_lookAt =   m_eye + m_dir; 
	
	//< 뷰갱신
	updateView();
}
//< roll(Z회전) : 전방(방향) 벡터 기준 회전(롤)
void		SH_iCamera::rotRoll( float angle )
{
	//< 방향벡터
	updateDir();
	//< 우향벡터
	updateRight();

	D3DXMATRIX	  roll;

	//< 방향벡터를 기준으로 회전한다.
	D3DXMatrixRotationAxis( &roll, &m_dir, angle);
	//< 회전한 값을 방향벡터에 반영한다.
	D3DXVec3TransformCoord( &m_up, &m_up, &roll);

	//< 시점이동
	//m_lookAt =   m_eye + m_dir; 

	//< 뷰갱신
	updateView();
}


//< 프로젝션 설정(나중에 뷰포트설정하게 되면 그때는 알아서하셈)
void		SH_iCamera::updateProj( void )
{
	if( NULL != m_d3dDevice9 )
	{
		//< 프로젝션 설정
		D3DXMatrixPerspectiveFovLH( &m_proj, 
			D3DX_PI / 4 ,			//< 시야각
			WINSIZE_X / WINSIZE_Y,	//< 종횡비(윈도우 가로세로사이즈)
			//< 생성된 뷰포트의 종횡비를 뜻한다.(기본으로는 윈도우생성크기)
			1,						//< 카메라 최소거리
			1000 );					//< 카메라 최대거리
		m_d3dDevice9->SetTransform( D3DTS_PROJECTION , &m_proj );
	}
}
//< 뷰설정
void		SH_iCamera::updateView( void )
{
	if( NULL != m_d3dDevice9 )
	{
		//< 방향벡터 업데이트
		updateDir();
		//< 우향벡터 업데이트
		updateRight();

		//< 카메라 행렬
		D3DXMatrixLookAtLH( &m_view , &m_eye, &m_lookAt ,&m_up );
		//< 적용
		m_d3dDevice9->SetTransform( D3DTS_VIEW , &m_view );
	}
}
//< 방향벡터 갱신
void		SH_iCamera::updateDir( void )
{
	//< 방향벡터( 바라보는 시점에서 나의 위치 빼기 )
	m_dir = m_lookAt - m_eye;
	//< 단위벡터
	D3DXVec3Normalize( &m_dir , &m_dir );
}
//< 우향벡터 갱신
void		SH_iCamera::updateRight( void )
{
	//< 업벡터 와 방향 벡터를 외적
	D3DXVec3Cross( &m_right, &m_up, &m_dir );
	//< 단위벡터
	D3DXVec3Normalize( &m_right , &m_right );
}

//< 마우스 메세지
bool	SH_iCamera::mouseMessage( HWND wnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	switch(message)
	{
	case WM_MOUSEWHEEL:
		{
			if( GetKeyState( VK_CONTROL) & 0x8000 )
			{
				//< 컨트롤을 누르고 휠을 돌리면 빨리 이동
				moveForBackWard( 1.0f * ( GET_WHEEL_DELTA_WPARAM(wparam) / 20.0f));
			}
			else
			{
				//< 그렇지 않으면 적당히 이동
				moveForBackWard( 1.0f * ( GET_WHEEL_DELTA_WPARAM(wparam) / 200.0f));
			}

		}
		return true;
	}

	return false;
}


}//< namespace end