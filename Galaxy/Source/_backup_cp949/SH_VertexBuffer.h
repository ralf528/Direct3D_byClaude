#ifndef __SH_VERTEX_BUFFER_H__
#define __SH_VERTEX_BUFFER_H__

#include "SH_Buffer.h"

namespace SH3D
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─ 버텍스 데이터 클래스, SH. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	class SH_VertexData
	{
	public:
		VOID *							m_vertexData;		// Vertex 정보
		int								m_verType;			// Vertex Type
		int								m_verCnt;			// Vertex Count
		int								m_triangleCnt;		// Vertex Triangle num
		DWORD							m_verFVF;			// Vertex FVF
		int								m_verFVFSize;			// Vertex FVF Size;
		LPDIRECT3DVERTEXBUFFER9			m_vertexBuffer;				// VertexBuffer

	public:
		SH_VertexData()
		{
			m_vertexData		= NULL;
			m_verType			= SH3D::FT_VER_NOR_TEX;
			m_verCnt			= 0;
			m_triangleCnt		= 0;
			m_verFVF			= 0;
			m_verFVFSize		= 0;
			m_vertexBuffer		= NULL;
		}

		SH_VertexData( VOID *	VertexData, int nType, int nVerCnt)
		{
			m_vertexData		= VertexData;
			m_verType			= nType;
			m_verCnt			= nVerCnt;
			m_triangleCnt		= nVerCnt / 3;
			m_verFVF			= SH3D::GetFVF(nType);
			m_verFVFSize		= SH3D::GetFVFSize(nType);
			m_vertexBuffer		= NULL;
		}

		~SH_VertexData( VOID )
		{
			//< 버텍스 버퍼 생성 정보 삭제
			SH_DEL_PTR_ARRAY(m_vertexData);
			//< 버텍스 버퍼 인터페이스 해제
			SH_SAFE_RELEASE(m_vertexBuffer);
		}
	};
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// ☆━─ 버텍스 버퍼 클래스, SH. ─━☆
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	class SH_VertexBuffer : public SH_Buffer
	{
	public:
		SH_VertexBuffer(  VOID  );
		virtual ~SH_VertexBuffer(  VOID  );

		virtual		HRESULT		lostDevice(  void  );
		virtual		HRESULT		resetDevice(  void  );

		HRESULT					initVertex( void *	VertexData, int nType, int nVerCnt);
	
		virtual		void		update(  FLOAT fDT = 0.0f  ) {}
		virtual		HRESULT		render(  void  );
		virtual		void		release( void );

		LPDIRECT3DVERTEXBUFFER9	getVB( void );
		SH_VertexData*			getVBData(	void );

	private:
		///<	버텍스 메모리 
		bool					setVertexMemory(  void  );


	private:
		//< 버텍스정보
		SH_VertexData*			m_vertexInfo;
		//< 디바이스
		LPDIRECT3DDEVICE9		m_d3dDevice;
	};
}

#endif