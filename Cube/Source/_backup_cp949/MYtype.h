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

//< 큐브
typedef struct tagCubeInfo
{
	//< 회전
	float			rotAngleX;
	float			rotAngleY;
	float			rotAngleZ;
	//< 회전량
	float			rotSpeed;
	
	//< 월드좌표
	D3DXVECTOR3		pos;
	//< 상향벡터
	D3DXVECTOR3		up;
	//< 스케일
	D3DXVECTOR3		scale;
}CUBE_INFO, *LPCUBE_INFO;