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

typedef struct tagVerUV
{
	float x,y,z,rhw;
	float _u, _v;
	enum { MYFVF = D3DFVF_XYZRHW | D3DFVF_TEX1 };
}VER_UV, *LPVER_UV;

//< 행성
typedef struct tagPlanet
{
	//< 자전
	float			rotAngle;
	//< 회전량
	float			rotSpeed;
	//< 자전 축
	D3DXVECTOR3		rotAxis;

	//< 공전
	float			revAngle;
	//< 회전량
	float			revSpeed;
	//< 공전 축
	D3DXVECTOR3		revAxis;

	//< 월드좌표
	D3DXVECTOR3		pos;
	//< 스케일
	D3DXVECTOR3		scale;
	//< 부모행렬
	D3DXMATRIX		*parent;
	//< 자식에게 물려줄 행렬
	D3DXMATRIX		world;

}PLANET_INFO, *LPPLANET_INFO;

enum PLANET
{
	P_SUN = 0,
	P_MECURY,
	P_VENUS,
	P_EARTH,
	P_MOON,
	P_MARS,
	P_JUPITER,
	P_SATURN,
	P_END
};