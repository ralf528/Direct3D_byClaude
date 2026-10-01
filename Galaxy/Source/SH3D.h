#pragma once

namespace SH3D
{
	extern LPDIRECT3D9			g_d3d;
	extern LPDIRECT3DDEVICE9	g_d3dDevice;

	//< 편의함수
	HRESULT	setTransformMatrix( const D3DXMATRIX *world = NULL );
}