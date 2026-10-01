#pragma once

#include "BASE_Object.h"

class Planet : public BASE_Object
{
public:
	Planet(void);
	~Planet(void);

	//< 플레이어 정보 설정 ( 구조체로 )
	bool initPlanet( const LPPLANET_INFO PlanetInfo );

private:
	//< 트랜스폼
	HRESULT			setTM( void );

public:
	//< 디바이스 해제
	virtual HRESULT	lostDevice( void );
	//< 디바이스 복구
	virtual HRESULT resetDevice( void );

	//< 오브젝트 이름 얻기
	virtual const TCHAR* toString( void ) { return "Planet"; }

	//< 오브젝트 삭제.
	virtual void release( void );

	//< 오브젝트 갱신
	virtual void update(FLOAT fDT = 0.0f);

	//< 3D오브젝트 렌더
	virtual	HRESULT render( void );

	//< 행렬 물려주기
	virtual D3DXMATRIX* getMatWorld(void);

	//< 행성 정보 참조
	inline LPPLANET_INFO getPlanetInfo(void){ return m_PlanetInfo; }

private:
	//< 행성 정보
	LPPLANET_INFO				m_PlanetInfo;
	//< 디바이스 정보
	LPDIRECT3DDEVICE9			m_d3dDevice;
};