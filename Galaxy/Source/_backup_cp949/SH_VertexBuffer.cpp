#include "stdafx.h"
#include "SH_VertexBuffer.h"

namespace SH3D
{


	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─- 버텍스 버퍼 클래스, Janus. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	SH_VertexBuffer::SH_VertexBuffer()
		:m_vertexInfo(NULL),m_d3dDevice( NULL )
	{
		//< 게임매니져로 부터 디바이스 얻기
		m_d3dDevice = GAME_MGR->getDevice();
	}

	SH_VertexBuffer::~SH_VertexBuffer()
	{
		release();	
	}

	//< 버텍스 버퍼 생성 
	HRESULT		SH_VertexBuffer::initVertex( void *	vertexData, int vertexType, int verCnt )
	{
		//< 정보 설정
		if( m_vertexInfo == NULL )
		{
			SH_NEW_PTR(SH_VertexData( NULL, vertexType, verCnt ),m_vertexInfo);

			//< 메모리 
			setVertexMemory();
			memmove(m_vertexInfo->m_vertexData, vertexData,
				m_vertexInfo->m_verFVFSize * m_vertexInfo->m_verCnt );
		}
		//< 버텍스 FVF 사이즈
		int fvfSize = m_vertexInfo->m_verFVFSize * m_vertexInfo->m_verCnt;

		if( m_vertexInfo->m_vertexBuffer == NULL )
		{
			//< 버텍스 버퍼 생성 
			HRESULT	hr	= m_d3dDevice->CreateVertexBuffer(
				fvfSize,
				0,
				m_vertexInfo->m_verFVF,
				D3DPOOL_DEFAULT,
				&m_vertexInfo->m_vertexBuffer,
				NULL);

			if( hr != S_OK )
			{
				return hr;
			}
		}
		

		//< 락~~언락~~
		void *	pVer = NULL;
		m_vertexInfo->m_vertexBuffer->Lock(0, fvfSize ,	&pVer,0);

		if( pVer != NULL )
		{
			memmove(pVer, vertexData,
				fvfSize );
		}

		m_vertexInfo->m_vertexBuffer->Unlock();

		return S_OK;
	}

	//< 버텍스 버퍼 렌더 
	HRESULT		SH_VertexBuffer::render(  void  )
	{
		///< 디바이스 , 포인터 , 버텍스 버퍼 확인 
		if( m_d3dDevice != NULL &&
			m_vertexInfo != NULL &&
			m_vertexInfo->m_vertexBuffer != NULL )
		{
			m_d3dDevice->SetStreamSource(0, 
				m_vertexInfo->m_vertexBuffer, 0, 
				m_vertexInfo->m_verFVFSize);
			m_d3dDevice->SetFVF(m_vertexInfo->m_verFVF);
			m_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_vertexInfo->m_triangleCnt );

			return S_OK;
		}

		return S_FALSE;
	}

	void		SH_VertexBuffer::release( void )
	{
		SH_DEL_PTR(m_vertexInfo);
	}

	bool		SH_VertexBuffer::setVertexMemory( void )
	{
		switch(m_vertexInfo->m_verType)
		{
		case SH3D::FT_VER_COLOR:
			m_vertexInfo->m_vertexData = new SH3D::tagVerColor[m_vertexInfo->m_verCnt];
			return TRUE;
		case SH3D::FT_VER_RHW_COLOR:
			m_vertexInfo->m_vertexData = new SH3D::tagVerRhwColor[m_vertexInfo->m_verCnt];
			return TRUE;
		case SH3D::FT_MY_VER_COLOR:
			m_vertexInfo->m_vertexData = new SH3D::tagMyVerColor[m_vertexInfo->m_verCnt];
			return TRUE;
		case SH3D::FT_VER_NOR_TEX:
			m_vertexInfo->m_vertexData = new SH3D::tagVerNormalTex[m_vertexInfo->m_verCnt];
			return TRUE;
		}
		return FALSE;
	}

	HRESULT		SH_VertexBuffer::lostDevice(  void  )
	{
		SH_SAFE_RELEASE( m_vertexInfo->m_vertexBuffer );
		return S_OK;
	}

	HRESULT		SH_VertexBuffer::resetDevice(  void  )
	{
		if( m_vertexInfo != NULL )
		{
			return initVertex( m_vertexInfo->m_vertexData,
				m_vertexInfo->m_verFVF
				,m_vertexInfo->m_verCnt);
		}

		return S_FALSE;
	}

	LPDIRECT3DVERTEXBUFFER9	SH_VertexBuffer::getVB( void )
	{
		if( m_vertexInfo != NULL )
		{
			return m_vertexInfo->m_vertexBuffer;
		}

		return NULL;
	}

	SH_VertexData*			SH_VertexBuffer::getVBData( void )
	{
		if( m_vertexInfo != NULL )
		{
			return m_vertexInfo;
		}

		return NULL;
	}

}//< namespace end