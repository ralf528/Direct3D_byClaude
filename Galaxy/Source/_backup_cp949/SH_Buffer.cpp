#include "stdafx.h"
#include "SH_Buffer.h"

namespace SH3D
{
	//< 사용자 정의 포맷에 대한 플래그 얻기
	DWORD			GetFVF(int nType)
	{
		switch(nType)
		{
		case FT_VER_RHW_COLOR:
			return tagVerRhwColor::FVF_VER_RHW_COLOR;
		case FT_VER_COLOR:
			return tagVerColor::FVF_VER_COLOR;
		case FT_MY_VER_COLOR:
			return tagMyVerColor::FVF_VER_COLOR;
		case FT_VER_NOR_TEX:
			return tagVerNormalTex::FVF_VER_NOR_TEX;
		default:
			return -1;
		}
	}
	//< 사용자 정의 포맷에 대한 구조체 사이즈 얻기
	int				GetFVFSize(int nType)	//< FVF사이즈
	{
		switch(nType)
		{
		case FT_VER_RHW_COLOR:
			return sizeof(tagVerRhwColor);
		case FT_VER_COLOR:
			return sizeof(tagVerColor);
		case FT_MY_VER_COLOR:
			return sizeof(tagMyVerColor);
		case FT_VER_NOR_TEX:
			return sizeof(tagVerNormalTex);
		case FT_INDEX_16BIT:
			return sizeof(tagIndex16);
		case FT_INDEX_32BIT:
			return sizeof(tagIndex32);
		default:
			return -1;
		}
	}

	//< 사용자 인덱스 포맷 얻기
	D3DFORMAT		GetFMT(int nType)
	{
		switch(nType)
		{
		case FT_INDEX_16BIT:
			return D3DFMT_INDEX16;
		case FT_INDEX_32BIT:
			return D3DFMT_INDEX32;
		default:
			return D3DFMT_INDEX32;
		}
	}
	//< 사용자 인덱스 포맷에 구조체 사이즈 얻기
	int				GetFMTSize(int nType)
	{
		switch(nType)
		{
		case FT_INDEX_16BIT:
			return sizeof(tagIndex16);
		case FT_INDEX_32BIT:
			return sizeof(tagIndex32);
		default:
			return -1;
		}
	}
}//namespace SH3D