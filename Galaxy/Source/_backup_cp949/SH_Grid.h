#pragma once

#include <iostream>
#include <vector>
#include "SH_Buffer.h"

using namespace std;

const static FLOAT GRID_XYZ_LENGTH		= 11.0f;

namespace SH3D
{
	//< 그리드를쌍으로 관리할 리스트 목록( 삽입 삭제 보다는 참조가 많은것으로 판단)
class SH_Grid : public BASE_Object
{
public:
	//< 그리드 관리
	typedef	std::vector<tagVerColor*>	VEC_Grid;
	typedef	VEC_Grid::iterator			VEC_Grid_Iter;

public:
	SH_Grid( void );
	~SH_Grid( void );
	
	//< 초기화	
	virtual	 void 			init( void );

	//< 그리드 렌더 
	//< 라이트를 기본적으로 끄고 렌더를 시작하고
	//< 렌더링이 끝나면 라이트를 켜야한다.
	//< 만약 렌더링에 라이트가 필요하지 않다면 그냥 렌더
	virtual	HRESULT			render( void );
	
	//< 화살표 렌더 여부
	void					setRenderArrow( bool show ) { m_showArrow = show; }
	//< 그리드 렌더 여부
	void					setRenderGrid( bool show )	{ m_showGrid = show;  }

	//< 그리드 월드 추가 
	virtual bool			setWorld(const D3DXMATRIXA16* pWorld);

	//< 랜더 여부 변경
	void changeRenderFlag(void){ m_renderFlag = !m_renderFlag; }

private:
	//< 그리드 전체 삭제
	virtual  void 			deleteAll( void );
	//< 그리드 추가 
	virtual	bool			addGrid(D3DXVECTOR3	vStart, D3DXVECTOR3 vEnd, DWORD dwColor);

	//< 화살표 추가 
	virtual	bool			setArrow(bool bSet,FLOAT fDist);
private:
	//< 그리드 목록을 관리할 벡터 리스트
	VEC_Grid				m_gridList;
	bool					m_showArrow;
	bool					m_showGrid;
	//< 화살표 리스트
	tagVerColor*			m_vertexList;
	D3DXMATRIXA16			m_worldMatrix;
	LPDIRECT3DDEVICE9		m_d3dDevice9;

	//< 그리드 랜더 여부
	bool					m_renderFlag;
};

}//< namespace end
