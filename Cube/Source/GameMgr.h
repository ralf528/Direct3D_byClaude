#pragma once

#include "Singleton.h"

//< 전방선언
class ImageNode;
class Cube;

namespace SH3D
{
	class SH_VertexBuffer;
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

	//< 큐브 초기화
	void initCube(void);

private:
	//////////////////////////////////////////////////////////////////////////
	//< 멤버변수
	
	//< 게임 중
	BOOL						m_playGame;

	LPDIRECT3D9					m_d3d9;
	LPDIRECT3DDEVICE9			m_d3dDevice9;

	//< 큐브 객체
	Cube						*m_cube;

	//////////////////////////////////////////////////////////////////////////
	//< 실행경로
	char	m_path[ _MAX_FNAME ];
};