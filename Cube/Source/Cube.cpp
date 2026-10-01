#include "stdafx.h"
#include "SH_VertexBuffer.h"
#include "Cube.h"

using namespace SH3D;

Cube::Cube(void)
	:m_d3dDevice( NULL ) , m_cubeInfo( NULL )
	,m_vertexBuffer(NULL )
{
	//< 장치 얻기
	m_d3dDevice = GAME_MGR->getDevice();
}


Cube::~Cube(void)
{
	release();
}

//< 플레이어정보 설정
bool Cube::initCube( const LPCUBE_INFO CubeInfo )
{
	if( CubeInfo == NULL )
	{
		//< 로그처리
		return false;
	}

	if( m_cubeInfo != NULL )
	{
		SH_DEL_PTR( m_cubeInfo );
	}

	SH_NEW_PTR( CUBE_INFO, m_cubeInfo );

	memmove_s( m_cubeInfo, sizeof(CUBE_INFO), CubeInfo, sizeof(CUBE_INFO) );

	//< 버텍스버퍼생성
	initVertexBuffer();

	return true;
}

//< 버텍스버퍼설정
HRESULT Cube::initVertexBuffer( void )
{
	if( m_vertexBuffer != NULL )
	{
		SH_DEL_PTR( m_vertexBuffer );
	}

	SH_NEW_PTR( SH_VertexBuffer, m_vertexBuffer );

	//< 정점 개수
	const int vertexSize = 36;

	sVER_COLOR verInfo[vertexSize];
	//< 앞면 (시계방향)
	verInfo[0].vPos = D3DXVECTOR3( -1.0f, -1.0f, -1.0f );
	verInfo[0].dwColor = 0xffff0000;
	verInfo[1].vPos = D3DXVECTOR3( -1.0f,  1.0f, -1.0f );
	verInfo[1].dwColor = 0xffff0000;
	verInfo[2].vPos = D3DXVECTOR3(  1.0f,  1.0f, -1.0f );
	verInfo[2].dwColor = 0xffff0000;
	verInfo[3].vPos = D3DXVECTOR3( -1.0f, -1.0f, -1.0f );
	verInfo[3].dwColor = 0xffff0000;
	verInfo[4].vPos = D3DXVECTOR3(  1.0f,  1.0f, -1.0f );
	verInfo[4].dwColor = 0xffff0000;
	verInfo[5].vPos = D3DXVECTOR3(  1.0f, -1.0f, -1.0f );
	verInfo[5].dwColor = 0xffff0000;
	//< 뒷면 (반시계방향)
	verInfo[6].vPos		= D3DXVECTOR3(  1.0f, -1.0f, 1.0f );
	verInfo[6].dwColor	= 0xff00ff00;
	verInfo[7].vPos		= D3DXVECTOR3(  1.0f,  1.0f, 1.0f );
	verInfo[7].dwColor	= 0xff00ff00;
	verInfo[8].vPos		= D3DXVECTOR3( -1.0f,  1.0f, 1.0f );
	verInfo[8].dwColor	= 0xff00ff00;
	verInfo[9].vPos		= D3DXVECTOR3(  1.0f, -1.0f, 1.0f );
	verInfo[9].dwColor	= 0xff00ff00;
	verInfo[10].vPos	= D3DXVECTOR3( -1.0f,  1.0f, 1.0f );
	verInfo[10].dwColor = 0xff00ff00;
	verInfo[11].vPos	= D3DXVECTOR3( -1.0f, -1.0f, 1.0f );
	verInfo[11].dwColor = 0xff00ff00;
	//< 왼쪽면
	verInfo[12].vPos = D3DXVECTOR3( -1.0f, -1.0f,  1.0f );
	verInfo[12].dwColor = 0xffff00ff;
	verInfo[13].vPos = D3DXVECTOR3( -1.0f,  1.0f,  1.0f );
	verInfo[13].dwColor = 0xffff00ff;
	verInfo[14].vPos = D3DXVECTOR3( -1.0f,  1.0f, -1.0f );
	verInfo[14].dwColor = 0xffff00ff;
	verInfo[15].vPos = D3DXVECTOR3( -1.0f, -1.0f,  1.0f );
	verInfo[15].dwColor = 0xffff00ff;
	verInfo[16].vPos = D3DXVECTOR3( -1.0f,  1.0f, -1.0f );
	verInfo[16].dwColor = 0xffff00ff;
	verInfo[17].vPos = D3DXVECTOR3( -1.0f, -1.0f, -1.0f );
	verInfo[17].dwColor = 0xffff00ff;
	//< 오른쪽면
	verInfo[18].vPos = D3DXVECTOR3( 1.0f, -1.0f, -1.0f );
	verInfo[18].dwColor = 0xff00ffff;
	verInfo[19].vPos = D3DXVECTOR3( 1.0f,  1.0f, -1.0f );
	verInfo[19].dwColor = 0xff00ffff;
	verInfo[20].vPos = D3DXVECTOR3( 1.0f,  1.0f,  1.0f );
	verInfo[20].dwColor = 0xff00ffff;
	verInfo[21].vPos = D3DXVECTOR3( 1.0f, -1.0f, -1.0f );
	verInfo[21].dwColor = 0xff00ffff;
	verInfo[22].vPos = D3DXVECTOR3( 1.0f,  1.0f,  1.0f );
	verInfo[22].dwColor = 0xff00ffff;
	verInfo[23].vPos = D3DXVECTOR3( 1.0f, -1.0f,  1.0f );
	verInfo[23].dwColor = 0xff00ffff;
	//< 윗면
	verInfo[24].vPos = D3DXVECTOR3( -1.0f,  1.0f, -1.0f );
	verInfo[24].dwColor = 0xffffffff;
	verInfo[25].vPos = D3DXVECTOR3( -1.0f,  1.0f,  1.0f );
	verInfo[25].dwColor = 0xffffffff;
	verInfo[26].vPos = D3DXVECTOR3(  1.0f,  1.0f,  1.0f );
	verInfo[26].dwColor = 0xffffffff;
	verInfo[27].vPos = D3DXVECTOR3( -1.0f,  1.0f, -1.0f );
	verInfo[27].dwColor = 0xffffffff;
	verInfo[28].vPos = D3DXVECTOR3(  1.0f,  1.0f,  1.0f );
	verInfo[28].dwColor = 0xffffffff;
	verInfo[29].vPos = D3DXVECTOR3(  1.0f,  1.0f, -1.0f );
	verInfo[29].dwColor = 0xffffffff;
	//< 아랫면
	verInfo[30].vPos = D3DXVECTOR3(  1.0f,  -1.0f, -1.0f );
	verInfo[30].dwColor = 0xff000000;
	verInfo[31].vPos = D3DXVECTOR3(  1.0f,  -1.0f,  1.0f );
	verInfo[31].dwColor = 0xff000000;
	verInfo[32].vPos = D3DXVECTOR3( -1.0f,  -1.0f,  1.0f );
	verInfo[32].dwColor = 0xff000000;
	verInfo[33].vPos = D3DXVECTOR3(  1.0f,  -1.0f, -1.0f );
	verInfo[33].dwColor = 0xff000000;
	verInfo[34].vPos = D3DXVECTOR3( -1.0f,  -1.0f,  1.0f );
	verInfo[34].dwColor = 0xff000000;
	verInfo[35].vPos = D3DXVECTOR3( -1.0f,  -1.0f, -1.0f );
	verInfo[35].dwColor = 0xff000000;

	return m_vertexBuffer->initVertex( verInfo , SH3D::FT_VER_COLOR , vertexSize );
}

//< 트랜스폼
HRESULT Cube::setTM( void )
{
	//< W = S * R * T
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRotX;
	D3DXMATRIXA16 matRotY;
	D3DXMATRIXA16 matRotZ;
	D3DXMATRIXA16 matTrans;
	D3DXMATRIXA16 matWorld;

	//< 크기
	D3DXMatrixScaling( &matScale, m_cubeInfo->scale.x ,
		m_cubeInfo->scale.y,
		m_cubeInfo->scale.z );

	//< 회전
	D3DXMatrixRotationX( &matRotX, m_cubeInfo->rotAngleX );
	D3DXMatrixRotationY( &matRotY, m_cubeInfo->rotAngleY );
	D3DXMatrixRotationZ( &matRotZ, m_cubeInfo->rotAngleZ );

	//< 이동
	D3DXMatrixTranslation( &matTrans, m_cubeInfo->pos.x, 
		m_cubeInfo->pos.y, 
		m_cubeInfo->pos.z );

	//< 조합
	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;

	return m_d3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

//< 디바이스 해제
HRESULT Cube::lostDevice( void )
{
	return S_OK;
}
//< 디바이스 복구
HRESULT Cube::resetDevice( void )
{
	return S_OK;
}

//< 오브젝트 삭제.
void Cube::release( void )
{
	//< 버텍스버퍼삭제
	SH_DEL_PTR( m_vertexBuffer );
	//< 플레이어 정보 삭제
	SH_DEL_PTR( m_cubeInfo );

}

//< 오브젝트 갱신
void Cube::update(FLOAT fDT)
{
	if( keyInput::isKeyDown( 'W' ) )
	{
		m_cubeInfo->rotAngleX += m_cubeInfo->rotSpeed;
	}

	if( keyInput::isKeyDown( 'S' ) )
	{
		m_cubeInfo->rotAngleX -= m_cubeInfo->rotSpeed;
	}

	if( keyInput::isKeyDown( 'A' ) )
	{
		m_cubeInfo->rotAngleY += m_cubeInfo->rotSpeed;
	}

	if( keyInput::isKeyDown( 'D' ) )
	{
		m_cubeInfo->rotAngleY -= m_cubeInfo->rotSpeed;
	}

	if( keyInput::isKeyDown( 'Q' ) )
	{
		m_cubeInfo->rotAngleZ += m_cubeInfo->rotSpeed;
	}

	if( keyInput::isKeyDown( 'E' ) )
	{
		m_cubeInfo->rotAngleZ -= m_cubeInfo->rotSpeed;
	}
}

//< 3D오브젝트 렌더
HRESULT Cube::render( void )
{
	//< TM적용
	setTM();
	//< 렌더
	if( m_vertexBuffer != NULL )
	{
		m_vertexBuffer->render();
	}

	return S_OK;
}