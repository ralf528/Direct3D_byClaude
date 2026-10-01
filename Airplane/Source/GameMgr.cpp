#include "stdafx.h"
//< C / C++ / 

#include "ImageNode.h"

#include "GameMgr.h"


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
	//< 버텍스버퍼 생성
	if( initRhw() != S_OK || initVertex() != S_OK )
	{
		return false;
	}
	//< 카메라 적용
	initCamera();
	//< 투영
	initProjection();

	//< 플레이어
	initPlayer();
	//< 적군
	initEnemy();

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

//< 버텍스 버퍼 설정
HRESULT GameMgr::initRhw(void)
{
	//< 버텍스 정보 설정
	VER_RHW verInfo[6] =
	{
		{	10.0f, 30.0f, 0.0f, 1.0f , 0xffff0000 },
		{	10.0f, 10.0f, 0.0f, 1.0f , 0xffff0000 },
		{  310.0f, 30.0f, 0.0f, 1.0f , 0xffff0000 },
		{  310.0f, 30.0f, 0.0f, 1.0f , 0xffff0000 },
		{	10.0f, 10.0f, 0.0f, 1.0f , 0xffff0000 },
		{  310.0f, 10.0f, 0.0f, 1.0f , 0xffff0000 }
	};

	//< 정점 개수
	int verCount = 6;
	//< 크기 계산
	int size = sizeof( VER_RHW ) * verCount; // < 폴리곤을 그릴 메모리 크기

	//< 버텍스 버퍼 생성
	HRESULT hr = m_d3dDevice9->CreateVertexBuffer(
		size,				//< 버텍스 크기
		0,					//< 모드(쓰기,읽기,둘다)
		VER_RHW::MYFVF,		//< 버텍스 포멧
		D3DPOOL_DEFAULT,
		&m_rhwHPVer,
		NULL
		);
	if( hr != S_OK )
	{
		assert(0);
		return hr;
	}

	//< 버텍스버퍼에 정점정보 복사
	void *ver;

	m_rhwHPVer->Lock( 0, size, &ver, NULL );

	//< 정보 복사
	memmove_s( ver, size, verInfo, size );

	m_rhwHPVer->Unlock();

	return hr;
}
//< 버텍스 버퍼
HRESULT GameMgr::initVertex(void)
{
	//< 버텍스 정보 설정
	VER_COLOR verInfo[3] =
	{
		{ -1.f, -1.f, 0.f, 0xffff0000},
		{  0.f,  1.f, 0.f, 0xff00ff00},
		{  1.f, -1.f, 0.f, 0xffff00ff}
	};
	
	//< 크기 계산
	int verCount = 3; // < 정점개수
	int size = sizeof( VER_COLOR ) * verCount;

	HRESULT hr = m_d3dDevice9->CreateVertexBuffer(
		size,
		0,
		VER_COLOR::MYFVF,
		D3DPOOL_DEFAULT,
		&m_playerVertex,
		NULL
		);

	if( hr != S_OK )
	{
		assert(0);
		return hr;
	}
	
	//< 버텍스버퍼에 정점정보 복사
	void *ver;

	m_playerVertex->Lock( 0, size, &ver, NULL );

	//< 정보복사
	memmove_s( ver, size, verInfo, size );

	m_playerVertex->Unlock();

	return S_OK;
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

//< 플레이어 초기화
void GameMgr::initPlayer(void)
{
	m_player = new PLAYER;

	m_player->m_dir		= D3DXVECTOR3( 0.f, 1.f, 0.f );
	m_player->m_up		= D3DXVECTOR3( 0.f, 0.f,-1.f );
	m_player->m_scale	= D3DXVECTOR3( 1.f, 1.5f, 1.f );
	m_player->m_pos		= D3DXVECTOR3( 0.f, 0.f, 0.f );

	m_player->moveSpeed = 0.4f;
	m_player->rotAngleZ = 0.0f;
	m_player->rotSpeed  = 0.1f;

	m_player->m_nowHP = 100.f;
	m_player->m_maxHP = 100.f;
}
//< 적군 초기화

//< 플레이어 업데이트
void GameMgr::updatePlayer(void)
{
	//< 이동
	if( keyInput::isKeyDown( VK_UP ) )
	{
		m_player->m_pos += m_player->m_dir * m_player->moveSpeed;
	}

	if( keyInput::isKeyDown( VK_DOWN ) )
	{
		m_player->m_pos -= m_player->m_dir * m_player->moveSpeed;
	}

	//< 방향
	if( keyInput::isKeyDown( VK_LEFT ) )
	{
		m_player->rotAngleZ += m_player->rotSpeed;
		updatePlayerDir();
	}
	if( keyInput::isKeyDown( VK_RIGHT ) )
	{
		m_player->rotAngleZ -= m_player->rotSpeed;
		updatePlayerDir();
	}

	//< 총알 발사
	if( keyInput::isKeyDown( VK_SPACE ) )
	{
		shootBullet();
	}
}
//< 플레이어 방향
void GameMgr::updatePlayerDir(void)
{
	D3DXVECTOR3		axis( 0.f, 1.f, 0.f );
	D3DXMATRIXA16	matRotZ;

	//< 회전
	D3DXMatrixRotationZ( &matRotZ, m_player->rotAngleZ );
	//< 변환 적용
	D3DXVec3TransformCoord( &m_player->m_dir, &axis, &matRotZ );
	//< 정규화
	D3DXVec3Normalize( &m_player->m_dir, &m_player->m_dir );
	//
}
//< 플레이어 랜더
void GameMgr::renderPlayer(void)
{
	//< 스트림소스에 올리기
	m_d3dDevice9->SetStreamSource( 0, m_playerVertex, 0, sizeof(VER_COLOR) );
	//< FVF 설정
	m_d3dDevice9->SetFVF( VER_COLOR::MYFVF );
	//< 그리기
	m_d3dDevice9->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
}
//< 플레이어 TM
void GameMgr::setPlayerTM(void)
{
	//< W = S * R * T
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRotZ;
	D3DXMATRIXA16 matTrans;
	D3DXMATRIXA16 matWorld;

	//< 크기
	D3DXMatrixScaling( &matScale, 
		m_player->m_scale.x,
		m_player->m_scale.y,
		m_player->m_scale.z );
	
	//< 회전
	D3DXMatrixRotationZ( &matRotZ, m_player->rotAngleZ );
	
	//< 이동
	D3DXMatrixTranslation( &matTrans, 
		m_player->m_pos.x,
		m_player->m_pos.y,
		m_player->m_pos.z );

	//< 조합
	matWorld = matScale * matRotZ * matTrans;
	
	//< 적용
	m_d3dDevice9->SetTransform( D3DTS_WORLD, &matWorld );
}

//< 플레이어 삭제
void GameMgr::releasePlayer(void)
{
	SAFE_DELETE( m_player );
}

//< 적군 초기화
void GameMgr::initEnemy(void)
{
	m_enemy = new ENEMY[ENEMY_NUM];

	for(int i = 0 ; i<ENEMY_NUM ; i++ )
	{
		m_enemy[i].m_up		= D3DXVECTOR3( 0.f, 0.f,-1.f );
		m_enemy[i].m_scale	= D3DXVECTOR3( 1.f, 1.f, 1.f );

		m_enemy[i].moveSpeed = 0.5f;
		m_enemy[i].rotAngleZ = 0.0f;
		m_enemy[i].rotSpeed  = 0.1f;
	}
}

//< 적군 업데이트
void GameMgr::updateEnemy(void)
{
	for(int i = 0 ; i<ENEMY_NUM ; i++ )
	{
		if( m_enemy[i].existFlag == true )
		{
			//< 캐릭터와 부딫히면 비활성화			
			D3DXVECTOR3 distToPlayer = m_player->m_pos - m_enemy[i].m_pos;
			float distance = D3DXVec3Length( &distToPlayer );
			if( distance < 3.f ) 
			{
				//< 플레이어 HP 감소
				m_player->m_nowHP-= 10;
				m_enemy[i].existFlag = false;
				continue;
			}
			//< 멀어지면 비활성화
			distance = D3DXVec3Length( &m_enemy[i].m_pos );
			if( distance > 100.f )
			{
				m_enemy[i].existFlag = false;
			}
			//< 총알에 닿으면 비활성화
			if( collisionBullet( m_enemy[i].m_pos ) )
			{
				m_enemy[i].existFlag=false;
			}
		}
		else
		{
			//< 위치 랜덤 생성
			float posX,posY;
			posX = static_cast<float>( rand() % 200 - 100 );
			posY = static_cast<float>( rand() % 200 - 100 );

			m_enemy[i].m_pos		= D3DXVECTOR3( posX, posY, 0.f );
			//< 너무 가까우면 생성하지 않음
			float distance = D3DXVec3Length( &m_enemy[i].m_pos );
			if( distance < 100.f )
			{
				continue;
			}
			m_enemy[i].m_dir		= m_player->m_pos - m_enemy[i].m_pos;
			//< 방향벡터 정규화
			D3DXVec3Normalize( &m_enemy[i].m_dir, &m_enemy[i].m_dir );
			
			//< 방향
			D3DXVECTOR3		axis( 0.f, 1.f, 0.f );
			D3DXMATRIXA16	matRotZ;

			//< 방향 계산
			D3DXVECTOR3 right( -1.f, 0.f, 0.f);
			//< 회전할 각도
			float angle = acosf( D3DXVec3Dot( &axis, &m_enemy[i].m_dir ) );
			//< 우향벡터와의 각도
			float rotAngle = acosf( D3DXVec3Dot( &right, &m_enemy[i].m_dir ) );
			//< 각도 계산
			if( rotAngle < D3DX_PI/2 )
			{
				//< 예각일때
				m_enemy[i].rotAngleZ = angle;
			}
			else
			{
				//< 둔각일때
				m_enemy[i].rotAngleZ = -angle;
			}
			//< 회전
			D3DXMatrixRotationZ( &matRotZ, m_enemy[i].rotAngleZ );
			//< 변환 적용
			D3DXVec3TransformCoord( &m_enemy[i].m_dir, &axis, &matRotZ );
			//< 정규화
			D3DXVec3Normalize( &m_enemy[i].m_dir, &m_enemy[i].m_dir );
			
			//< 활성화
			m_enemy[i].existFlag = true;
		}
	}
}

//< 적군 TM
void GameMgr::setEnemyTM(int i)
{
	//< 회전
	D3DXMATRIXA16 matRotZ;
	D3DXMatrixRotationZ( &matRotZ, m_enemy[i].rotAngleZ );//+ D3DX_PI/2 );

	//< 이동
	m_enemy[i].m_pos += m_enemy[i].m_dir * m_enemy[i].moveSpeed;

	//< 이동 변환
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation( &matTrans, m_enemy[i].m_pos.x, m_enemy[i].m_pos.y, m_enemy[i].m_pos.z );

	D3DXMATRIXA16 matWorld;

	matWorld = matRotZ * matTrans;

	m_d3dDevice9->SetTransform( D3DTS_WORLD, &matWorld );
}

//< 적군 삭제
void GameMgr::releaseEnemy(void)
{
	SAFE_DELETE_ARR(m_enemy);
}

//< 총알 발사
void GameMgr::shootBullet(void)
{
	for( int i = 0 ; i < BULLET_NUM ; i++ )
	{
		if( m_bullet[i].m_existFlag == false )
		{
			//< 플레이어의 위치
			m_bullet[i].m_pos		= D3DXVECTOR3( m_player->m_pos.x, m_player->m_pos.y, 0.f );
			//< 플레이어의 방향
			m_bullet[i].m_dir		= m_player->m_dir;
			//< 방향벡터 정규화
			D3DXVec3Normalize( &m_bullet[i].m_dir, &m_bullet[i].m_dir );

			m_bullet[i].rotAngleZ = m_player->rotAngleZ;

			//< 활성화
			m_bullet[i].m_existFlag = true;

			break;
		}
	}
}

//< 총알 갱신
void GameMgr::updateBullet(void)
{
	for( int i = 0 ; i < BULLET_NUM ; i++ )
	{
		if( m_bullet[i].m_existFlag == true )
		{
			//< W = S * R * T
			D3DXMATRIXA16 matScale;
			D3DXMATRIXA16 matRotZ;
			D3DXMATRIXA16 matTrans;
			D3DXMATRIXA16 matWorld;

			//< 크기 
			D3DXMatrixScaling( &matScale, 
				m_bullet[i].m_scale.x, 
				m_bullet[i].m_scale.y,
				m_bullet[i].m_scale.z );

			//< 회전
			D3DXMatrixRotationZ( &matRotZ, m_bullet[i].rotAngleZ );

			//< 이동
			m_bullet[i].m_pos += m_bullet[i].m_dir * m_bullet[i].moveSpeed;
			
			//< 멀어지면 비활성화
			float distance = D3DXVec3Length( &m_bullet[i].m_pos );
			if( distance > 100.f )
			{
 				m_bullet[i].m_existFlag = false;
			}

			//< 이동 변환
			D3DXMatrixTranslation( &matTrans, m_bullet[i].m_pos.x, m_bullet[i].m_pos.y, m_bullet[i].m_pos.z );

			matWorld = matScale * matRotZ * matTrans;

			m_d3dDevice9->SetTransform( D3DTS_WORLD, &matWorld );
			//랜더
			renderPlayer();
		}
	}
}
//< 총알 충돌체크
bool GameMgr::collisionBullet( D3DXVECTOR3 &dest )
{
	//< 적군과 부딫히면 비활성화			
	for( unsigned int i = 0 ; i < BULLET_NUM ; i++ )
	{
		if( m_bullet[i].m_existFlag == true )
		{
			D3DXVECTOR3 distVec = dest - m_bullet[i].m_pos;
			float distance = D3DXVec3Length( &distVec );
			if( distance < 3.f ) 
			{
				//< 총알 비활성화
				m_bullet[i].m_existFlag = false;
				return true;
			}
		}
	}
	return false;
}
//< HP 갱신
void GameMgr::updateHP(void)
{
	float hpPos = 10.f + 300.f*(m_player->m_nowHP/m_player->m_maxHP);
	
	//< 버텍스 정보 설정
	VER_RHW verInfo[6] =
	{
		{	10.0f, 30.0f, 0.0f, 1.0f , 0xffff0000 },
		{	10.0f, 10.0f, 0.0f, 1.0f , 0xffff0000 },
		{   hpPos, 30.0f, 0.0f, 1.0f , 0xffff0000 },
		{   hpPos, 30.0f, 0.0f, 1.0f , 0xffff0000 },
		{	10.0f, 10.0f, 0.0f, 1.0f , 0xffff0000 },
		{   hpPos, 10.0f, 0.0f, 1.0f , 0xffff0000 }
	};

	//< 정점 개수
	int verCount = 6;
	//< 크기 계산
	int size = sizeof( VER_RHW ) * verCount; // < 폴리곤을 그릴 메모리 크기
	
	//< 버텍스버퍼에 복사
	void *ver;

	m_rhwHPVer->Lock( 0, size, &ver, NULL );

	//< 정보 복사
	memmove_s( ver, size, verInfo, size );
	m_rhwHPVer->Unlock();
}

//< 정점 랜더
void GameMgr::renderHP(void)
{
	//< 스트림 소스에 올리기
	m_d3dDevice9->SetStreamSource( 0, m_rhwHPVer, 0, sizeof( VER_RHW ) );
	//< FVF 설정
	m_d3dDevice9->SetFVF( VER_RHW::MYFVF );
	//< 그리기
	m_d3dDevice9->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 3 );
}

//< 업데이트
void GameMgr::update(  float deltaTime )
{
	if( m_playGame )
	{
		//< 플레이어 갱신
		updatePlayer();
		//< 적군 갱신
		updateEnemy();
		//< HP바 갱신
		updateHP();

#ifndef _DEBUG
		if( m_player->m_nowHP <= 0 )
		{
			m_playGame = FALSE;
			if( IDOK == MessageBox( g_hWnd, "죽었습니다.", "DIE!", MB_OK ) )
			{
				SendMessage( g_hWnd, WM_DESTROY, 0, 0 );
			}
		}
#endif
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
			//< 적군 랜더
			for( int i = 0 ; i < ENEMY_NUM ; i ++ )
			{
				setEnemyTM(i);
				if( m_enemy[i].existFlag )
				{
					renderPlayer();
				}
			}
			//< hp 랜더
			renderHP();
			//< 플레이어 TM
			setPlayerTM();
			//< 플레이어 랜더
			renderPlayer();
			//< 총알 갱신
			updateBullet();

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

	releasePlayer();
	releaseEnemy();

	SAFE_RELEASE( m_rhwHPVer );

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