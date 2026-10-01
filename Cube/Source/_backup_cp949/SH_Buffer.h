#ifndef __SH_BUFFER_H__
#define __SH_BUFFER_H__

#include "BASE_Object.h"

namespace SH3D
{
	//< 사용자 정의 포맷에 대한 플래그 얻기
	DWORD			GetFVF(int nType);		
	//< 사용자 정의 포맷에 대한 구조체 사이즈 얻기
	int				GetFVFSize(int nType);	//< FVF사이즈

	//< 사용자 인덱스 포맷 얻기
	D3DFORMAT		GetFMT(int nType);
	//< 사용자 인덱스 포맷에 구조체 사이즈 얻기
	int				GetFMTSize(int nType);


	/************************************************************************/
	/* enum                                                                 */
	/************************************************************************/
	enum FVF_TYPE 
	{
		FT_VER_RHW_COLOR = 0,	//< Vertex , Rhw , Color
		FT_VER_COLOR,			//< Vertex , Color 
		FT_MY_VER_COLOR,		//< MY Vertex, Color
		FT_VER_NOR_TEX,			//< Vertex, Nomal, Texture
		FT_INDEX_16BIT,			//< Index 16Bit ( 인덱스 16비트 : 점 65535개)
		FT_INDEX_32BIT,			//< Index 32Bit ( 인덱스 32비트 : 점 약 43억개)
		FT_END
	};


	/************************************************************************/
	/* D3D관련 구조체                                                       */
	/************************************************************************/
	//< 사용자 정의 버텍스포맷 : enum문은 내부에서 참조 
	//< V : X,Y,Z, 
	//< R : RHW동차 좌표 
	//< C : 컬러 DWORD 
	//< N : Normal 
	//< T : 텍스쳐UV

	//< 동차좌표계 버텍스
	typedef struct	tagVerRhwColor
	{
		float					x,y,z,rhw;		//< 동차좌표
		DWORD					dwColor;		//< 색상 
		//< XYZ | 동차좌표계 | 확산광
		enum  { FVF_VER_RHW_COLOR = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE) };
	}sVER_RHW_COLOR,*LPVER_RHW_COLOR;

	//< 좌표 | 확산광 
	typedef struct	tagVerColor
	{
		D3DXVECTOR3			vPos;			// 버텍스좌표
		DWORD				dwColor;		// 색상 

		enum  { FVF_VER_COLOR = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };

		tagVerColor() {};
		tagVerColor(D3DXVECTOR3& _vPos, DWORD _dwColor)
		{
			vPos	= _vPos;
			dwColor	= _dwColor;
		}
	}sVER_COLOR,*LPVER_COLOR;

	//< MY형을 사용하는 구조체
	typedef struct	tagMyVerColor
	{
		D3DXVECTOR3		vPos;			// 버텍스좌표
		DWORD				dwColor;		// 색상 

		enum  { FVF_VER_COLOR = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };

		tagMyVerColor() {};
		tagMyVerColor(D3DXVECTOR3& _vPos, DWORD _dwColor)
		{
			vPos	= _vPos;
			dwColor	= _dwColor;
		}
	}sMYVER_COLOR,*LPMYVER_COLOR;

	//< 좌표 | 법선벡터 | 텍스쳐
	typedef struct	tagVerNormalTex
	{
		D3DXVECTOR3		vPos;		// 버텍스 좌표
		D3DXVECTOR3		vNormal;	// 노말 벡터
		D3DXVECTOR2		UV;			// 텍셀 좌표

		enum  { FVF_VER_NOR_TEX = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };

		//< 디폴트 생성자
		tagVerNormalTex() {};
		//< 인자 있는 생성자
		tagVerNormalTex(D3DXVECTOR3 TvPos)
		{
			vPos = TvPos;
			memset(vNormal, 0, sizeof(D3DXVECTOR3));
			memset(UV, 0, sizeof(D3DXVECTOR2));
		}
		//< 인자 잇는 생성자
		tagVerNormalTex(D3DXVECTOR3 TvPos, D3DXVECTOR3 TvNormal)
		{
			vPos		= TvPos;
			vNormal		= TvNormal;
			memset(UV, 0, sizeof(D3DXVECTOR2));
		}
		//< 인자 있는 생성자
		tagVerNormalTex(D3DXVECTOR3 TvPos, D3DXVECTOR3 TvNormal, D3DXVECTOR2 TUV)
		{
			vPos		= TvPos;
			vNormal	= TvNormal;
			UV			= TUV;
		}
	}sVER_NOR_TEX,*LPVER_NOR_TEX;

	//< 인덱스 
	struct tagIndex16
	{
		WORD w0,w1,w2;

		tagIndex16() {};

		tagIndex16(WORD Tw0, WORD Tw1, WORD Tw2)
		{
			w0 = Tw0; w1 = Tw1; w2 = Tw2;
		}
	};

	struct tagIndex32
	{
		DWORD dw0,dw1,dw2;

		tagIndex32() {};

		tagIndex32(DWORD Tdw0, DWORD Tdw1, DWORD Tdw2)
		{
			dw0 = Tdw0; dw1 = Tdw1; dw2 = Tdw2;
		}
	};

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─- 버퍼 관리 클래스 ( 중간 매개 역활 ), Janus. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	class SH_Buffer : public BASE_Object
	{
	public:
		SH_Buffer(void)			 {}
		virtual ~SH_Buffer(void) {}
	};
}//< namespace SH3D

#endif