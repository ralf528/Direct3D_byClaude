#pragma once

#include "BASE_Object.h"

namespace SH3D
{
	class SH_VertexBuffer;
}

class Cube : public BASE_Object
{
public:
	Cube(void);
	~Cube(void);

	//< 플레이어 정보 설정 ( 구조체로 )
	bool initCube( const LPCUBE_INFO CubeInfo );

private:
	//< 버텍스버퍼설정
	HRESULT			initVertexBuffer( void );
	//< 트랜스폼
	HRESULT			setTM( void );

public:
	//< 디바이스 해제
	virtual HRESULT	lostDevice( void );
	//< 디바이스 복구
	virtual HRESULT resetDevice( void );

	//< 오브젝트 이름 얻기
	virtual const TCHAR* toString( void ) { return "Cube"; }

	//< 오브젝트 삭제.
	virtual void release( void );

	//< 오브젝트 갱신
	virtual void update(FLOAT fDT = 0.0f);

	//< 3D오브젝트 렌더
	virtual	HRESULT render( void );

private:
	//< 큐브 정보
	LPCUBE_INFO			m_cubeInfo;
	//< 디바이스 정보
	LPDIRECT3DDEVICE9		m_d3dDevice;
	//< 버텍스 버퍼
	SH3D::SH_VertexBuffer	*m_vertexBuffer;
};