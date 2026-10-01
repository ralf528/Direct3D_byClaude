#include "StdAfx.h"
#include "SH_Grid.h"

const static float ARROW_SCALE = 0.3f;

namespace SH3D
{

SH_Grid::SH_Grid( void )
:m_vertexList(NULL), m_renderFlag(false)
{

}

SH_Grid::~SH_Grid( void )
{
	deleteAll();
}

//< 초기화	
void 		SH_Grid::init( void )
{
	//< 디바이스 얻기
	m_d3dDevice9 = GAME_MGR->getDevice();

	//< 리스트 초기화
	m_gridList.clear();

	m_showArrow = true;
	m_showGrid = true;

	D3DXMatrixIdentity(&m_worldMatrix);

	//< pivot(방향축)
	addGrid(D3DXVECTOR3( -GRID_XYZ_LENGTH, 0.02f, 0 ), D3DXVECTOR3(GRID_XYZ_LENGTH,0.02f,0), 0x00ff0000);
	addGrid(D3DXVECTOR3( 0, -GRID_XYZ_LENGTH,0 ), D3DXVECTOR3(0,GRID_XYZ_LENGTH,0), 0x0000ff00);
	addGrid(D3DXVECTOR3( 0, 0.02f, -GRID_XYZ_LENGTH ), D3DXVECTOR3(0,0.02f,GRID_XYZ_LENGTH), 0xff0000ff);
	setArrow( true ,GRID_XYZ_LENGTH );

	//< grid추가
	//x축
	for(int i = 0; i < 42; i+= 2)
	{
		//< X
		addGrid(D3DXVECTOR3(-10.0f, 0.0f, (-10.0f + i/2 )), D3DXVECTOR3(10.0f, 0.0f, (-10.0f + i/2)),0x00ffffff);	
		//< Z
		addGrid(D3DXVECTOR3((-10.0f + i/2), 0.0f, -10.0f), D3DXVECTOR3((-10.0f + i/2), 0.0f,  10.0f),0x00ffffff);	
	}
	//< 랜더 여부
	m_renderFlag = false;
}

//< 그리드 렌더 
HRESULT		SH_Grid::render( void )
{
	//< 디바이스 검사
	if( m_d3dDevice9 != NULL && m_renderFlag )
	{
		//< 포그 끄기
		m_d3dDevice9->SetRenderState(D3DRS_FOGENABLE, FALSE);
		//< 알파 끄기
		m_d3dDevice9->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		//< 텍스쳐 선택 없음 
		m_d3dDevice9->SetTexture(0,NULL);
		//< 적용버텍스포맷
		m_d3dDevice9->SetFVF(tagVerColor::FVF_VER_COLOR);
		
		//< 라이트끄기
		//m_d3dDevice9->SetRenderState( D3DRS_LIGHTING, FALSE );
		//< 기본 월드 적용
		SH3D::setTransformMatrix(&m_worldMatrix);
		//< 그리드 렌더
		if( true == m_showGrid )
		{
			//< 그리드 리스트 렌더
			VEC_Grid_Iter iter = m_gridList.begin();

			for( ; iter != m_gridList.end() ; ++iter )
			{
				tagVerColor*	pGridInfo = (*iter);
				if( pGridInfo != NULL )
				{
					if(FAILED(m_d3dDevice9->DrawPrimitiveUP(D3DPT_LINELIST,	1, pGridInfo, sizeof(SH3D::tagVerColor))))
					{
						return E_FAIL;
					}
				}
			}
		}
		
		//< 피봇(화살표)렌더
		if( true == m_showArrow )
		{
			//화살표
			m_d3dDevice9->DrawPrimitiveUP(D3DPT_LINELIST, 
				8, 
				m_vertexList, 
				sizeof(SH3D::tagVerColor));
		}

		//< 라이트끄기
		//m_d3dDevice9->SetRenderState( D3DRS_LIGHTING, TRUE );

		return SH3D::setTransformMatrix();
	}

	return E_FAIL;
}

//< 그리드 추가 
bool		SH_Grid::addGrid(D3DXVECTOR3	vStart, D3DXVECTOR3 vEnd, DWORD dwColor )
{
	//< 그리드 생성 
	tagVerColor* pGridVerLine;
	SH_NEW_PTR_ARRAY(tagVerColor,pGridVerLine,2);

	pGridVerLine[0] = tagVerColor(vStart, dwColor);
	pGridVerLine[1] = tagVerColor(vEnd, dwColor);

	m_gridList.push_back(pGridVerLine);
	return TRUE;
}

//< 화살표 추가 
bool		SH_Grid::setArrow(bool bSet,FLOAT fDist)
{
	if( m_vertexList == NULL )
	{
		SH_NEW_PTR_ARRAY(tagVerColor,m_vertexList,24);
		//x축 화살표
		m_vertexList[0] = tagVerColor(D3DXVECTOR3(fDist,				0.02f,			0.0f), 0xffff0000 );
		m_vertexList[1] = tagVerColor(D3DXVECTOR3(fDist-ARROW_SCALE, ARROW_SCALE,	0.0f), 0xffff0000 );
		m_vertexList[2] = tagVerColor(D3DXVECTOR3(fDist,			    0.02f,			0.0f), 0xffff0000 );
		m_vertexList[3] = tagVerColor(D3DXVECTOR3(fDist-ARROW_SCALE, -ARROW_SCALE,    0.0f), 0xffff0000 );

		////x축 화살표
		//m_vertexList[4] = tagVerColor(D3DXVECTOR3(fDist,			0.0f,  0.0f), 0xffff0000 );
		//m_vertexList[5] = tagVerColor(D3DXVECTOR3(fDist-0.5f,    0.0f,  0.5f), 0xffff0000 );
		//m_vertexList[6] = tagVerColor(D3DXVECTOR3(fDist,			0.0f,  0.0f), 0xffff0000 );
		//m_vertexList[7] = tagVerColor(D3DXVECTOR3(fDist-0.5f,    0.0f,  -0.5f), 0xffff0000 );


		//y축 화살표
		m_vertexList[8] = tagVerColor(D3DXVECTOR3( 0.0f,			   fDist,			    0.0f), 0xff00ff00);
		m_vertexList[9] = tagVerColor(D3DXVECTOR3(-ARROW_SCALE,     fDist -ARROW_SCALE,  0.0f), 0xff00ff00);
		m_vertexList[10] = tagVerColor(D3DXVECTOR3( 0.0f,		   fDist,			    0.0f), 0xff00ff00);
		m_vertexList[11] = tagVerColor(D3DXVECTOR3( ARROW_SCALE,    fDist-ARROW_SCALE,  0.0f), 0xff00ff00);

		//z축 화살표
		m_vertexList[12]  = tagVerColor(D3DXVECTOR3(  0.0f,			 0.02f,				fDist		), 0xff0000ff);
		m_vertexList[13]  = tagVerColor(D3DXVECTOR3(  0.0f,			-ARROW_SCALE,		fDist-ARROW_SCALE	), 0xff0000ff);
		m_vertexList[14] = tagVerColor(D3DXVECTOR3(  0.0f,			 0.02f,				fDist		), 0xff0000ff);
		m_vertexList[15] = tagVerColor(D3DXVECTOR3(  0.0f,			 ARROW_SCALE,		fDist-ARROW_SCALE	), 0xff0000ff);
	}


	m_showArrow = bSet;
	return TRUE;
}

//< 그리드 월드 추가 
bool		SH_Grid::setWorld(const D3DXMATRIXA16* pWorld)
{
	if( pWorld == NULL )
	{
		return FALSE;
	}

	memcpy(&m_worldMatrix,pWorld,sizeof(D3DXMATRIXA16));

	return TRUE;
}

//< 그리드 전체 삭제
 void 		SH_Grid::deleteAll( void )
{
	VEC_Grid_Iter	iter= m_gridList.begin();

	for( ; iter != m_gridList.end() ; )
	{
		if( *iter != NULL )
		{
			tagVerColor	*pVer = (*iter);
			SH_DEL_PTR_ARRAY(pVer);
		}
		iter = m_gridList.erase(iter);
	}

	//< 화살표 정리
	SH_DEL_PTR_ARRAY(m_vertexList);
}

}//<namespace end