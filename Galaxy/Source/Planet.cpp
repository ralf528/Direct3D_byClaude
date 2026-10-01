#include "stdafx.h"
#include "SH_VertexBuffer.h"
#include "Planet.h"

using namespace SH3D;

Planet::Planet(void)
	:m_d3dDevice( NULL ) , m_PlanetInfo( NULL )
{
	//< 장치 얻기
	m_d3dDevice = GAME_MGR->getDevice();
}


Planet::~Planet(void)
{
	release();
}

//< 플레이어정보 설정
bool Planet::initPlanet( const LPPLANET_INFO PlanetInfo )
{
	if( PlanetInfo == NULL )
	{
		//< 로그처리
		return false;
	}

	if( m_PlanetInfo != NULL )
	{
		SH_DEL_PTR( m_PlanetInfo );
	}

	SH_NEW_PTR( PLANET_INFO, m_PlanetInfo );

	memmove_s( m_PlanetInfo, sizeof(PLANET_INFO), PlanetInfo, sizeof(PLANET_INFO) );

	return true;
}

//< 트랜스폼
HRESULT Planet::setTM( void )
{
	//< W = S * R * T
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRot;
	D3DXMATRIXA16 matRev;
	D3DXMATRIXA16 matTrans;

	//< 크기
	D3DXMatrixScaling( &matScale, m_PlanetInfo->scale.x ,
		m_PlanetInfo->scale.y,
		m_PlanetInfo->scale.z );

	//< 자전
	D3DXMatrixRotationAxis( &matRot, &m_PlanetInfo->rotAxis, m_PlanetInfo->rotAngle );

	//< 이동
	D3DXMatrixTranslation( &matTrans, m_PlanetInfo->pos.x, 
		m_PlanetInfo->pos.y, 
		m_PlanetInfo->pos.z );

	//< 공전
	D3DXMatrixRotationAxis( &matRev, &m_PlanetInfo->revAxis, m_PlanetInfo->revAngle );

	//< 자식에게 물려줄 월드
	m_PlanetInfo->world = matRot * matTrans * matRev * (*m_PlanetInfo->parent);
	//< 월드 조합
	matWorld = matScale * m_PlanetInfo->world;

	return m_d3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

//< 디바이스 해제
HRESULT Planet::lostDevice( void )
{
	return S_OK;
}
//< 디바이스 복구
HRESULT Planet::resetDevice( void )
{
	return S_OK;
}

//< 오브젝트 삭제.
void Planet::release( void )
{
	//< 플레이어 정보 삭제
	SH_DEL_PTR( m_PlanetInfo );
}

//< 오브젝트 갱신
void Planet::update(FLOAT fDT)
{
	//< 자전
	m_PlanetInfo->rotAngle += m_PlanetInfo->rotSpeed;
	//< 공전
	m_PlanetInfo->revAngle += m_PlanetInfo->revSpeed;
}

//< 3D오브젝트 렌더
HRESULT Planet::render( void )
{
	//< TM적용
	setTM();
	//< 메쉬 랜더는 밖에서;;
	return S_OK;
}

//< 행렬 물려주기
D3DXMATRIX* Planet::getMatWorld(void)
{
	if( m_PlanetInfo != NULL )
	{
		return &m_PlanetInfo->world;
	}
	return NULL;
}