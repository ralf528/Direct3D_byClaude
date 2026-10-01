#pragma once

#include "Singleton.h"

//< 전방선언
class ImageNode;
class Planet;

namespace SH3D
{
	class SH_Grid;
	class SH_iCamera;
}

class GameMgr
{
private:
	GameMgr(void);
	~GameMgr(void);

	//< 싱글톤으로 만들자
	SINGLETON( GameMgr );

public:
	//////////////////////////////////////////////////////////////////////////
	//<외부 인터페이스
	//////////////////////////////////////////////////////////////////////////
	//< 초기화
	bool	init(  int width, int height, bool winMode  = true  );
	//< 업데이트
	void	update( float deltaTime = 0.0f );
	//< 렌더
	void	render( void );
	//< 해제
	void	release( void );
	
	//< 메시지루프
	WPARAM	msgLoop( void );
	//< 프로시져
	LRESULT WndProc(HWND wnd ,UINT msg ,WPARAM wparam, LPARAM lparam );

	//< 장치 얻기
	LPDIRECT3DDEVICE9 getDevice(void) { return m_d3dDevice9; }

private:
	//< 3D설정
	HRESULT	init3D( int width, int height, bool winMode  = true );

	//< 카메라
	HRESULT initCamera(void);
	//< 투영
	HRESULT initProjection(void);

	//< 행성 초기화
	HRESULT initPlanet(void);
	//< 행성 업데이트
	void updatePlanet( float deltaTime );
	//< 행성 랜더
	void renderPlanet(void);
	//< 행성 해제
	void releasePlanet(void);

	//< 법선계산함수
	D3DXVECTOR3* computeNormal( D3DXVECTOR3 *p0, D3DXVECTOR3 *p1,D3DXVECTOR3 *p2,D3DXVECTOR3 *result );
	//< 메쉬 정보 받아와서 UV 설정( 구 )
	ID3DXMesh* AutoClonMeshFVF( ID3DXMesh* Sphere, ID3DXMesh* SphereMesh );

	//< 스프라이트 객체 생성
	HRESULT initSprite(void);
	//< 스프라이트 텍스쳐 로딩
	HRESULT loadSpriteTex(void);
	//< 스프라이트 랜더
	void renderSprite(void);
	//< 스프라이트 일반 회전(Z)
	void renderRotateZ(void);
	//< 스프라이트 중점 회전(Z)
	void renderRotateCenterZ(void);

	//< 금성 고리 랜더
	void renderSaturnRing(void);

	//< 버텍스버퍼 설정
	HRESULT initVertex(void);
	//< 버텍스 랜더
	void renderVertex(void);

private:
	//////////////////////////////////////////////////////////////////////////
	//< 멤버변수
	LPDIRECT3D9					m_d3d9;
	LPDIRECT3DDEVICE9			m_d3dDevice9;

	//< 카메라
	SH_iCamera					*m_mainCam;
	//< 카메라 위치
	D3DXVECTOR3					m_cameraPos;

	//< 게임 중
	BOOL						m_playGame;
	//< 와이어 프레임 랜더 모드
	BOOL						m_wireFrameFlag;	

	//< 행성 객체
	Planet						*m_planet;
	//< 스피어 메쉬
	LPD3DXMESH					m_mesh;
	//< 랜더할 메쉬(uv)
	LPD3DXMESH					m_renderMesh;
	//< 텍스쳐 설정
	LPDIRECT3DTEXTURE9			m_texture[ P_END ];

	//< 최상위 부모 행렬
	D3DXMATRIX					m_matAncient;

	//< 그리드
	SH3D::SH_Grid				*m_grid;

	//< 스프라이트
	LPD3DXSPRITE				m_sprite;
	//< 스프라이트 텍스처
	LPDIRECT3DTEXTURE9			m_spriteTexture;

	//< 버텍스 버퍼
	LPDIRECT3DVERTEXBUFFER9		m_VB_HPBar;	

	//////////////////////////////////////////////////////////////////////////
	//< 실행경로
	char	m_path[ _MAX_FNAME ];
};