#include "stdafx.h"
#include "SH3D.h"

namespace SH3D
{
	LPDIRECT3D9			g_d3d = NULL;
	LPDIRECT3DDEVICE9	g_d3dDevice = NULL;

	//< 편의함수
	HRESULT	setTransformMatrix( const D3DXMATRIX *world )
	{
		D3DXMATRIX matWorld;
		if( world == NULL )
		{
			D3DXMatrixIdentity( &matWorld );
			return g_d3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		}

		return g_d3dDevice->SetTransform( D3DTS_WORLD , world );
	}
}