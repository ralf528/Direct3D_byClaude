#ifndef __MYTYPE_H__
#define __MYTYPE_H__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ☆━─ 15-06-26, 전역 구조체 및 전역 타입, SH. ─━☆
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#endif

typedef struct tagVerRhw
{
	float x,y,z,rhw;
	DWORD color;
	enum { MYFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
}VER_RHW, *LPVER_RHW;


typedef struct tagVerColor
{
	float x,y,z;
	DWORD color;
	enum { MYFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
}VER_COLOR, *LPVER_COLOR;

//< 비행체 구조체
typedef struct tagflight
{
	//< 회전
	float			rotAngleZ;
	//< 회전량
	float			rotSpeed;
	//< 이동스피드
	float			moveSpeed;
	//< 월드좌표
	D3DXVECTOR3		m_pos;
	//< 방향벡터
	D3DXVECTOR3		m_dir;
	//< 상향벡터
	D3DXVECTOR3		m_up;
	//< 스케일
	D3DXVECTOR3		m_scale;
}FLIGHT;

//< 플레이어 구조체
typedef struct tagPlayer : public FLIGHT
{
	//< 최대 체력
	float m_maxHP;
	//< 현제 체력
	float m_nowHP;

}PLAYER, *LPPLAYER;

//< 적군 구조체
typedef struct tagEnemy : public FLIGHT
{
	bool existFlag;

	tagEnemy(void)
	{
		existFlag = false;
	}
}ENEMY, *LPENEMY;

//< 총알 구조체
typedef struct tagBullet
{
	//< 회전
	float			rotAngleZ;
	//< 이동스피드
	float			moveSpeed;
	//< 월드좌표
	D3DXVECTOR3		m_pos;
	//< 방향벡터
	D3DXVECTOR3		m_dir;
	//< 상향벡터
	D3DXVECTOR3		m_up;
	//< 스케일
	D3DXVECTOR3		m_scale;
	//< 활성화 여부
	bool			m_existFlag;

	tagBullet(void)
	{
		m_existFlag = false;
		moveSpeed	= 1.f;
		m_up		= D3DXVECTOR3( 0.f, 0.f,-1.f );
		m_scale		= D3DXVECTOR3( 0.5f, 1.0f, 1.0f );
	}
}BULLET,*LPBULLET;