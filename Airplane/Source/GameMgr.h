#pragma once

#include "Singleton.h"

//< 전방선언
class ImageNode;

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

private:
	//< 3D설정
	HRESULT	init3D( int width, int height, bool winMode  = true );

	//< 버텍스 버퍼 설정
	HRESULT initRhw(void);
	//< 버텍스 버퍼
	HRESULT initVertex(void);
	//< 카메라
	HRESULT initCamera(void);
	//< 투영
	HRESULT initProjection(void);

	//< 플레이어 초기화
	void initPlayer(void);
	//< 플레이어 업데이트
	void updatePlayer(void);
	//< 플레이어 방향
	void updatePlayerDir(void);
	//< 플레이어 랜더
	void renderPlayer(void);
	//< 플레이어 TM
	void setPlayerTM(void);
	//< 플레이어 삭제
	void releasePlayer(void);	

	//< 적군 초기화
	void initEnemy(void);
	//< 적군 업데이트
	void updateEnemy(void);
	//< 적군 TM
	void setEnemyTM(int i);
	//< 적군 삭제
	void releaseEnemy(void);

	//< 총알 발사
	void shootBullet(void);
	//< 총알 갱신
	void updateBullet(void);
	//< 총알 충돌체크
	bool collisionBullet( D3DXVECTOR3 &dest );

	//< HP 갱신
	void updateHP(void);
	//< 정점 랜더
	void renderHP(void);

private:
	//////////////////////////////////////////////////////////////////////////
	//< 멤버변수
	
	//< 게임 중
	BOOL						m_playGame;

	LPDIRECT3D9					m_d3d9;
	LPDIRECT3DDEVICE9			m_d3dDevice9;

	//< RHW 버텍스 버퍼
	LPDIRECT3DVERTEXBUFFER9		m_rhwHPVer;

	//< 비행기 버텍스 버퍼
	LPDIRECT3DVERTEXBUFFER9		m_playerVertex;
	//< 플레이어 객체
	LPPLAYER					m_player;

	//< 총알
	BULLET						m_bullet[BULLET_NUM];

	//< 적군
	LPENEMY						m_enemy;

	//////////////////////////////////////////////////////////////////////////
	//< 실행경로
	char	m_path[ _MAX_FNAME ];
};