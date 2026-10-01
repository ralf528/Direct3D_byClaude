#include "stdafx.h"
//< C / C++ / 

#include "ImageNode.h"
#include "Planet.h"
#include "SH_VertexBuffer.h"
#include "SH_Grid.h"
#include "SH_FreeCam.h"
#include "SH_FpsCam.h"
#include "GameMgr.h"

GameMgr::GameMgr(void)
	: m_playGame(TRUE), m_grid(NULL), m_mesh(NULL), m_renderMesh(NULL),
	m_wireFrameFlag(false), m_sprite(NULL), m_mainCam(NULL)
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

	//< 행성 초기화
	initPlanet();
	
	//< 라이트 끄기
	m_d3dDevice9->SetRenderState( D3DRS_LIGHTING, FALSE );

	SH_NEW_PTR( SH_Grid, m_grid );
	//< 그리드 초기화
	m_grid->init();

	//< 스프라이트 , 텍스쳐
	initSprite();
	loadSpriteTex();

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

	SH3D::g_d3dDevice = m_d3dDevice9;

	return hr;
}

//< 카메라
HRESULT GameMgr::initCamera(void)
{
	//< 카메라 좌표 설정
	D3DXVECTOR3 eye( 0.f, 5.f, -30.f );
	D3DXVECTOR3 lookAt( 0.f, 0.f, 0.f );
	D3DXVECTOR3 up( 0.f, 1.f, 0.f );

	//< 뷰 변환행렬
	/*D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &eye, &lookAt, &up );*/
	
	//< 적용
	/*return m_d3dDevice9->SetTransform( D3DTS_VIEW, &matView );*/

	//< 카메라 모드
	//SH_NEW_PTR( SH_FreeCam, m_mainCam );
	SH_NEW_PTR( SH_FpsCam, m_mainCam );
	m_mainCam->initCam(eye, up, lookAt);

	return S_OK;
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

//< 행성 초기화
HRESULT GameMgr::initPlanet(void)
{
	//< 할당
	SH_NEW_PTR_ARRAY( Planet, m_planet, P_END );

	//< 스피어 메쉬
	D3DXCreateSphere( m_d3dDevice9, 1.0f, 15, 15, &m_mesh, 0 );
	//< uv 메쉬 복사
	m_renderMesh = AutoClonMeshFVF( m_mesh, m_renderMesh );
	//< 텍스처 로드
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/sun.bmp", &m_texture[P_SUN] );
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/mercury.jpg", &m_texture[P_MECURY] );
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/venus.jpg", &m_texture[P_VENUS] );
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/earth.jpg", &m_texture[P_EARTH] );
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/moon.jpg", &m_texture[P_MOON] );
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/mars.png", &m_texture[P_MARS] );
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/jupiter.jpg", &m_texture[P_JUPITER] );
	D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/saturn.jpg", &m_texture[P_SATURN] );

	//< 최 상단 부모행렬
	D3DXMatrixIdentity( &m_matAncient );

	//< 임시 객체
	PLANET_INFO tempPlanet;
	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	
	//< 태양
	tempPlanet.parent	= &m_matAncient;
	tempPlanet.scale	= D3DXVECTOR3( 2.0f, 2.0f, 2.0f );
	tempPlanet.pos		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.0f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	//< 내부에서 깊은복사
	m_planet[P_SUN].initPlanet( &tempPlanet );

	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	//< 수성
	tempPlanet.parent	= m_planet[P_SUN].getMatWorld();
	tempPlanet.scale	= D3DXVECTOR3( 0.1f, 0.1f, 0.1f );
	tempPlanet.pos		= D3DXVECTOR3( 5.8f/2, 0.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.047f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_planet[P_MECURY].initPlanet( &tempPlanet );
	
	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	//< 금성
	tempPlanet.parent	= m_planet[P_SUN].getMatWorld();
	tempPlanet.scale	= D3DXVECTOR3( 0.2f, 0.2f, 0.2f );
	tempPlanet.pos		= D3DXVECTOR3( 10.8f/2, 0.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.035f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_planet[P_VENUS].initPlanet( &tempPlanet );
	
	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	//< 지구
	tempPlanet.parent	= m_planet[P_SUN].getMatWorld();
	tempPlanet.scale	= D3DXVECTOR3( 0.3f, 0.3f, 0.3f );
	tempPlanet.pos		= D3DXVECTOR3( 15.0f/2, 0.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.029f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_planet[P_EARTH].initPlanet( &tempPlanet );
	
	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	//< 달
	tempPlanet.parent	= m_planet[P_MOON-1].getMatWorld();
	tempPlanet.scale	= D3DXVECTOR3( 0.15f, 0.15f, 0.15f );
	tempPlanet.pos		= D3DXVECTOR3( 1.0f, -1.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.01f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 1.0f, 1.0f, 0.0f );
	m_planet[P_MOON].initPlanet( &tempPlanet );

	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	//< 화성
	tempPlanet.parent	= m_planet[P_SUN].getMatWorld();
	tempPlanet.scale	= D3DXVECTOR3( 0.15f, 0.15f, 0.15f );
	tempPlanet.pos		= D3DXVECTOR3( 22.8f/2, 0.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.024f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_planet[P_MARS].initPlanet( &tempPlanet );

	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	//< 목성
	tempPlanet.parent	= m_planet[P_SUN].getMatWorld();
	tempPlanet.scale	= D3DXVECTOR3( 1.1f, 1.1f, 1.1f );
	tempPlanet.pos		= D3DXVECTOR3( 30.0f/2, 0.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.013f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_planet[P_JUPITER].initPlanet( &tempPlanet );

	ZeroMemory( &tempPlanet, sizeof(PLANET_INFO) );
	//< 토성
	tempPlanet.parent	= m_planet[P_SUN].getMatWorld();
	tempPlanet.scale	= D3DXVECTOR3( 0.9f, 0.9f, 0.9f );
	tempPlanet.pos		= D3DXVECTOR3( 40.0f/2, 0.0f, 0.0f );
	tempPlanet.rotAngle	= 0.0f;
	tempPlanet.revAngle	= 0.0f;
	tempPlanet.rotSpeed	= 0.01f;
	tempPlanet.revSpeed	= 0.009f;
	tempPlanet.rotAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	tempPlanet.revAxis	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_planet[P_SATURN].initPlanet( &tempPlanet );

	return S_OK;
}

//< 행성 업데이트
void GameMgr::updatePlanet( float deltaTime )
{
	for(int i = 0 ; i < P_END ; i++ )
	{
		m_planet[i].update(deltaTime);
	}
}

//< 행성 랜더
void GameMgr::renderPlanet(void)
{
	for(int i = 0 ; i < P_END ; i++ )
	{
		//< 트랜스폼 적용
		m_planet[i].render();
		//< 텍스처
		m_d3dDevice9->SetTexture( 0, m_texture[i] );
		m_renderMesh->DrawSubset(0);
	}
}

//< 행성 해제
void GameMgr::releasePlanet(void)
{
	SH_DEL_PTR_ARRAY( m_planet );
}

//< 업데이트
void GameMgr::update(  float deltaTime )
{
	if( m_playGame )
	{
		//< 행성 갱신
		updatePlanet(deltaTime);
	}
	//< 그리드
	if( keyInput::onceKeyDown( '1' ) )
	{
		m_grid->changeRenderFlag();
	}
	//< 와이어 프레임
	if( keyInput::onceKeyDown( '2' ) )
	{
		m_wireFrameFlag = !m_wireFrameFlag;
		if( m_wireFrameFlag )
		{
			m_d3dDevice9->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		}
		else
		{
			m_d3dDevice9->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		}
	}
	//< 메인 카메라 업데이트
	m_mainCam->update(deltaTime);
}
//< 렌더
void GameMgr::render( void )
{
	if( m_playGame && m_d3dDevice9 != NULL )
	{
		m_d3dDevice9->Clear( 0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

		if( SUCCEEDED( m_d3dDevice9->BeginScene() ) )
		{
			//< 그리드랜더
			m_grid->render();

			//< 행성 랜더
			renderPlanet();

			//< 금성 고리 랜더
			renderSaturnRing();

			//< 스프라이트 랜더
			//renderSprite();
			//renderRotateZ();
			//renderRotateCenterZ();

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

//< 스프라이트 객체 생성
HRESULT GameMgr::initSprite(void)
{
	//< 스프라이트 객체 생성
	if( NULL != m_d3dDevice9 )
	{
		return D3DXCreateSprite( m_d3dDevice9, &m_sprite );
	}
	return S_FALSE;
}
//< 스프라이트 텍스쳐 로딩
HRESULT GameMgr::loadSpriteTex(void)
{
	return D3DXCreateTextureFromFile( m_d3dDevice9, "Resources/saturn-ring.png", &m_spriteTexture );
}
//< 스프라이트 랜더
void GameMgr::renderSprite(void)
{
	if( NULL != m_sprite )
	{
		m_sprite->Begin( D3DXSPRITE_ALPHABLEND );
		//< 이 안에서 스프라이트 랜더
		/*
		LPDIRECT3DTEXTURE9 pTexture,	//< 랜더할 텍스처
		CONST RECT *pSrcRect,			//< 랜더할 영역
		CONST D3DXVECTOR3 *pCenter,		//< 중점
		CONST D3DXVECTOR3 *Position,	//< 위치
		D3DCOLOR Color					//< 색상
		*/

		//< 랜더 영역
		//RECT rc;
		//SetRect( &rc, 0, 0, width, height );
		//< 위치
		D3DXVECTOR3 pos(100, 100, 0);

		m_sprite->Draw(
			m_spriteTexture, NULL, NULL, &pos, D3DCOLOR_RGBA(255,255,255,255) );

		m_sprite->End();

		//m_sprite->OnLostDevice();
		//m_sprite->OnResetDevice();
	}
}
//< 스프라이트 일반 회전(Z)
void GameMgr::renderRotateZ(void)
{
	if( NULL != m_sprite )
	{
		m_sprite->Begin( D3DXSPRITE_ALPHABLEND );

		D3DXMATRIX matWorld, matRotZ;

		//< 회전
		static float angle = 0.0f;
		angle += 0.05f;
		D3DXMatrixRotationZ( &matRotZ, angle );

		matWorld = matRotZ;
		//< 적용
		m_sprite->SetTransform( &matWorld );

		m_sprite->Draw(
			m_spriteTexture, NULL, NULL, NULL, D3DCOLOR_RGBA(255,255,255,255) );

		m_sprite->End();
	}
}
//< 스프라이트 중점 회전(Z)
void GameMgr::renderRotateCenterZ(void)
{
	if( NULL != m_sprite )
	{
		m_sprite->Begin( D3DXSPRITE_ALPHABLEND );

		//< 텍스처 정보 얻기
		D3DSURFACE_DESC desc;
		m_spriteTexture->GetLevelDesc( 0, &desc );

		//< 그릴 영역
		RECT rt;
		SetRect( &rt, 0, 0, desc.Width, desc.Height );
		//< 중점
		D3DXVECTOR3 centerPos( desc.Width/2.0f, desc.Height/2.0f, 0 );

		//< 행렬
		D3DXMATRIX matWorld, matRotZ, matTrans;

		//< 회전
		static float angle = 0.0f;
		angle += 0.05f;
		if( D3DX_PI*2 <= angle )
		{
			angle = 0.0f;
		}
		D3DXMatrixRotationZ( &matRotZ, angle );

		//< 이동
		D3DXMatrixTranslation( &matTrans, desc.Width/2.0f, desc.Height/2.0f, 0.0f );

		matWorld = matRotZ * matTrans;

		//< 적용
		m_sprite->SetTransform( &matWorld );

		m_sprite->Draw(
			m_spriteTexture, &rt, &centerPos, NULL, D3DCOLOR_RGBA(255,255,255,255) );

		m_sprite->End();
	}
}

//< 금성 고리 랜더
void GameMgr::renderSaturnRing(void)
{
	if( NULL != m_sprite )
	{
		m_sprite->Begin( D3DXSPRITE_ALPHABLEND );

		//< 텍스처 정보 얻기
		D3DSURFACE_DESC desc;
		m_spriteTexture->GetLevelDesc( 0, &desc );

		//< 그릴 영역
		RECT rt;
		SetRect( &rt, 0, 0, desc.Width, desc.Height );
		//< 중점
		D3DXVECTOR3 centerPos( desc.Width/2.0f, desc.Height/2.0f, 0 );

		//< 행렬
		D3DXMATRIX matWorld, matRotX, matRotZ, matTrans;

		//< 눞히기
		//D3DXMatrixRotationX( &matRotX, 0.1f );

		//< 회전
		static float angle = 0.0f;
		angle += 0.05f;
		if( D3DX_PI*2 <= angle )
		{
			angle = 0.0f;
		}
		D3DXMatrixRotationZ( &matRotZ, angle );

		//D3DXVECTOR3 pos = m_planet[P_SATURN].getPlanetInfo()->pos;
		//< 이동
		D3DXMatrixTranslation( &matTrans, desc.Width/2.0f, desc.Height/2.0f, 0.0f );

		matWorld = matRotZ * matTrans;

		//< 적용
		m_sprite->SetTransform( &matWorld );
		//m_d3dDevice9->SetTransform( D3DTS_WORLD, &matWorld );

		m_sprite->Draw(
			m_spriteTexture, &rt, &centerPos, NULL, D3DCOLOR_RGBA(255,255,255,255) );

		m_sprite->End();
	}
}

//< 버텍스버퍼 설정
HRESULT GameMgr::initVertex(void)
{
	//< 버텍스 정보 설정
	VER_UV verInfo[6] =
	{
		{ 10.f, 10.f, 0.f, 1.f, 0.f, 0.f },
		{ 210.f, 10.f, 0.f, 1.f, 1.f, 0.f },
		{ 10.f, 60.f, 0.f, 1.f, 0.f, 1.f },
		{ 10.f, 60.f, 0.f, 1.f, 0.f, 1.f },
		{ 210.f, 10.f, 0.f, 1.f, 1.f, 0.f },
		{ 210.f, 60.f, 0.f, 1.f, 1.f, 1.f }
	};
	int verCount = 6;
	int size = sizeof(VER_UV) * verCount;
	
	//< 버텍스 버퍼 생성
	HRESULT hr = m_d3dDevice9->CreateVertexBuffer(
		size,
		0,
		VER_UV::MYFVF,
		D3DPOOL_DEFAULT,
		&m_VB_HPBar,
		NULL);
	if( hr != S_OK )
	{
		assert(0);
		return hr;
	}
	//< 버텍스 버퍼에 복사
	void *ver;

	m_VB_HPBar->Lock(0,size,&ver,NULL);

	memmove_s( ver,size,verInfo,NULL );

	m_VB_HPBar->Unlock();

	return hr;
}
//< 버텍스 랜더
void GameMgr::renderVertex(void)
{
	//< 스트림 소스에 올리기
	m_d3dDevice9->SetStreamSource( 0, m_VB_HPBar, 0, sizeof( VER_UV ) );
	//< FVF 설정
	m_d3dDevice9->SetFVF( VER_UV::MYFVF );
	//< 랜더
	m_d3dDevice9->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 6 );
}

//< 해제
void GameMgr::release( void )
{
	KillTimer( g_hWnd, 1 );
	
	SAFE_RELEASE( m_sprite );
	SAFE_RELEASE( m_spriteTexture );

	SAFE_RELEASE( m_mesh );
	SAFE_RELEASE( m_renderMesh );
	for( int i = 0 ; i < P_END ; i++ )
	{
		SAFE_RELEASE( m_texture[i] );
	}

	releasePlanet();
	SH_DEL_PTR( m_grid );
	SH_DEL_PTR( m_mainCam );

	//< 메모리 정리
	GetMemManager()->CloseManager();

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

//< 법선계산함수
D3DXVECTOR3* GameMgr::computeNormal( D3DXVECTOR3 *p0, D3DXVECTOR3 *p1,D3DXVECTOR3 *p2,D3DXVECTOR3 *result )
{
	D3DXVECTOR3	u = *p1 - *p0;
	D3DXVECTOR3	v = *p2 - *p0;

	D3DXVec3Cross( result, &u, &v );
	return D3DXVec3Normalize( result , result );
}

ID3DXMesh* GameMgr::AutoClonMeshFVF(ID3DXMesh* Sphere, ID3DXMesh* SphereMesh)
{

	Sphere->CloneMeshFVF(Sphere->GetOptions(), Sphere->GetFVF() | D3DFVF_TEX1, m_d3dDevice9, &SphereMesh);
	sVER_NOR_TEX* vertice;
	int vercnt = SphereMesh->GetNumVertices();
	SphereMesh->LockVertexBuffer(0, (void**)&vertice);


	// loop through the vertices
	for(int i = 0 ; i < vercnt ; i++)
	{
		//float len = D3DXVec3Length( &vertice[i].vPos );
		//float u, v, z = 0.0f;
		//float temp;

		//if(len>0.0f) 
		//{     
		//	if( 0.0f == vertice[i].vPos.x && 
		//		0.0f == vertice[i].vPos.y ) 
		//	{
		//		u = 0.0f; /* othwise domain error */
		//	}
		//	else
		//	{
		//		u = (1.0f - atan2( vertice[i].vPos.x, vertice[i].vPos.y ) / D3DX_PI ) * 0.5f;
		//	}

		//	z = vertice[i].vPos.z / len;

		//	if( z <= -1.0f) 
		//	{
		//		temp = (float)D3DX_PI;
		//	}
		//	else if( z >=1.0f) 
		//	{
		//		temp = 0.0f;
		//	} 
		//	else 
		//	{
		//		temp = (float)acos(z);
		//	}

		//	v = 1.0f- temp /D3DX_PI;
		//}		

		vertice[i].UV = D3DXVECTOR2( 
			atan2f(vertice[i].vNormal.z, vertice[i].vNormal.x) / (D3DX_PI * 2.0f),
			acosf(vertice[i].vNormal.y) / D3DX_PI 
		);

		//vertice[i].UV = D3DXVECTOR2( u, v );

	}
	SphereMesh->UnlockVertexBuffer();
	m_d3dDevice9->SetRenderState(D3DRS_WRAP0,D3DWRAP_U);
	return SphereMesh;
}