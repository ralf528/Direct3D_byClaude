#include "stdafx.h"
//< C / C++ / 

#include "ImageNode.h"
#include "Cube.h"
#include "SH_VertexBuffer.h"
#include "GameMgr.h"

using namespace SH3D;

GameMgr::GameMgr(void)
	: m_playGame(TRUE)
{
}


GameMgr::~GameMgr(void)
{
	release();
}

//< 초기화
bool GameMgr::init(  int width, int height, bool winMode  )
{
	//< 랜덤 시드
	srand( GetTickCount() );
	//< 경로얻기
	GetCurrentDirectory( _MAX_FNAME, m_path );
	SetTimer( g_hWnd, 1, 10, NULL );
	//< 키 초기화
	keyInput::initKey();

	//< 3d 설정
	if( init3D( width, height, winMode ) != S_OK )
	{
		return false;
	}

	//< 카메라 적용
	initCamera();
	//< 투영
	initProjection();

	//< 큐브 초기화
	initCube();

	//< 라이트 끄기
	m_d3dDevice9->SetRenderState( D3DRS_LIGHTING, FALSE );

	return true;
}

HRESULT  GameMgr::init3D(  int width, int height, bool winMode  )
{
	//< 크리에이트
	m_d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	assert( m_d3d9 );

	//< 캡스
	D3DCAPS9 caps;
	m_d3d9->GetDeviceCaps( D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL, &caps );

	//< 버텍스 프로세싱
	int vp=0;
	D3DDEVTYPE type;

	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		type = D3DDEVTYPE_HAL;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		type = D3DDEVTYPE_SW;
	}
	
	//< d3dpp
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	d3dpp.Windowed = winMode;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	//< 디바이스 생성
	HRESULT hr = m_d3d9->CreateDevice( D3DADAPTER_DEFAULT,
		type, g_hWnd, vp, &d3dpp, &m_d3dDevice9 );

	return hr;
}

//< 카메라
HRESULT GameMgr::initCamera(void)
{
	//< 카메라 좌표 설정
	D3DXVECTOR3 eye( 0.f, 0.f, -100.f );
	D3DXVECTOR3 lookAt( 0.f, 0.f, 0.f );
	D3DXVECTOR3 up( 0.f, 1.f, 0.f );

	//< 뷰 변환행렬
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &eye, &lookAt, &up );
	
	//< 적용
	return m_d3dDevice9->SetTransform( D3DTS_VIEW, &matView );
}
//< 투영
HRESULT GameMgr::initProjection(void)
{
	D3DXMATRIXA16 matProj;

	D3DXMatrixPerspectiveFovLH( 
		&matProj,
		D3DX_PI / 4.f,
		WINSIZE_X / WINSIZE_Y,
		1.f,
		1000.f
		);

	//< 적용
	return m_d3dDevice9->SetTransform( D3DTS_PROJECTION, &matProj );
}

//< 큐브 초기화
void GameMgr::initCube(void)
{
	//< 구조체 생성
	CUBE_INFO cubeInfo;

	cubeInfo.up		= D3DXVECTOR3( 0.f, 0.f,-1.f );
	cubeInfo.scale	= D3DXVECTOR3( 10.f, 10.f, 10.f );
	cubeInfo.pos	= D3DXVECTOR3( 0.f, 0.f, 0.f );

	cubeInfo.rotAngleX = 0.0f;
	cubeInfo.rotAngleY = 0.0f;
	cubeInfo.rotAngleZ = 0.0f;
	cubeInfo.rotSpeed  = 0.1f;

	//< 플레이어 생성
	SH_NEW_PTR( Cube, m_cube );

	if( m_cube->initCube( &cubeInfo ) != S_OK )
	{
		//< 에러처리
	}
}

//< 업데이트
void GameMgr::update(  float deltaTime )
{
	if( m_playGame )
	{
		//< 플레이어 갱신
		if( m_cube != NULL )
		{
			m_cube->update(deltaTime);
		}
	}
}
//< 렌더
void GameMgr::render( void )
{
	if( m_playGame && m_d3dDevice9 != NULL )
	{
		m_d3dDevice9->Clear( 0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0,0,255), 1.0f, 0);

		if( SUCCEEDED( m_d3dDevice9->BeginScene() ) )
		{
			//< 큐브 랜더
			m_cube->render();

			HRESULT hr = m_d3dDevice9->EndScene();

			m_d3dDevice9->Present( NULL, NULL, NULL, NULL );

			if( hr != S_OK )
			{
				Sleep(500);
			}
		}
		else
		{

		}
	}
}
//< 해제
void GameMgr::release( void )
{
	KillTimer( g_hWnd, 1 );

	SH_DEL_PTR( m_cube );

	SAFE_RELEASE( m_d3dDevice9 );
	SAFE_RELEASE( m_d3d9 );
}


//< 메시지루프
WPARAM GameMgr::msgLoop( void )
{
	MSG			Message;		//메세지 구조체 선언	
	//<GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterJanus, UINT wMsgFilterMAx)메시지큐에서 읽어들인  메세지가 WM_QUIT면 false 리턴 나머지는 true 리턴
	//<lpMsg : 메시지의 주소
	//<hWnd : 윈도우의 핸들 0이면 모든 윈도우의 메시지를 가져오고, 핸들 값을 지정하면 그핸들값에 포함된 메시지만 가져온다.
	//<wMsgFilterJanus, wMsgFilterMAx : 메시지를 읽어들일 범위 최소값 최대값(사용할경우 시스템이 무한루프에 빠질수 있다.)
	//< 4 메세지 루프(사용자로부터의 메시지를 처리한다 !메시지 구동시스템)
	while(GetMessage(&Message,0,0,0))
	{
		TranslateMessage(&Message);// 키보드 입력 메시지를 관리한다(a라는  키가 눌렸을때 a라는 키가 눌렸다는 메세지를 만들어낸다.)
		DispatchMessage(&Message); // 메시지를 가지고 프로시져함수를 호출하는 역활 
	}

	return Message.wParam;
}

//< 프로시져
LRESULT GameMgr::WndProc(HWND wnd ,UINT msg ,WPARAM wparam, LPARAM lparam )
{
	switch(msg)
	{
	case WM_SETFOCUS:
		window::setFocusMainWindow( true );
		break;
	case WM_KILLFOCUS:
		window::setFocusMainWindow( false );
		break;
	case WM_TIMER:
		{
			update();

			render();
		}
		
		break;

	case WM_LBUTTONDOWN:


		break;

	case WM_RBUTTONDOWN:

		break;

	case WM_KEYDOWN:
		switch(wparam)
		{
			//(esc key)
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	//< wndProc에서 처리 되지 않은 나머지 메시지들을 처리해준다.
	//< 윈도우크기 변경이나, 이동 등를 처리해준다.
	return(DefWindowProc(wnd,msg,wparam,lparam));
}