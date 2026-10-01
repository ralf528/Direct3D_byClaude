#pragma once

namespace SH3D
{

class SH_iCamera
{
public:
	enum CAM_KIND
	{
		CAM_FREE = 0,	//< 자유카메라
		CAM_RPG,		//< 알피지 카메라
		CAM_FPS,		//< FPS용카메라
		CAM_TOOL,		//< 툴용카메라
		CAM_END			//< 아무설정없음
	};

public:
	SH_iCamera( CAM_KIND camType );
	virtual ~SH_iCamera(void);

	//< 카메라 설정
	virtual HRESULT			initCam( D3DXVECTOR3 const &eye, 
									 D3DXVECTOR3 const &up,
									 D3DXVECTOR3 const &lookAt);
	//< 정리
	virtual void			release( void ) {}
	//< 업데이트
	virtual void			update( float dt = 0.0f ) {}
	//< 마우스 메세지
	virtual bool			mouseMessage( HWND wnd, UINT message, WPARAM wparam, LPARAM lparam );
	
	//< 카메라 전/후진
	void					moveForBackWard( float fspeed );
	//< 카메라 게걸음(좌/우)
	void					moveLeftRight( float speed );

	//< yaw(Y회전) : 상향(업) 벡터 기준 회전(요)
	void					rotYaw( float angle );
	//< pitch(X회전) : 우향 벡터 기준 회전(피치)
	void					rotPitch( float angle );
	//< roll(Z회전) : 전방(방향) 벡터 기준 회전(롤)
	void					rotRoll( float angle );

	//< 우향벡터
	inline D3DXVECTOR3*		getRight( void )	{ return &m_right;	}
	//< 방향벡터
	inline D3DXVECTOR3*		getDir( void )		{ return &m_dir;	}
	//< 행렬정보 얻기
	inline D3DXMATRIX*		getProj( VOID )		{ return &m_proj;	}
	inline D3DXMATRIX*		getView( VOID )		{ return &m_view;	}
	inline D3DXMATRIX*		getViewProj( VOID ) { return &m_viewProj; }

	//< 카메라 위치
	inline	void			setPos( D3DXVECTOR3 const &pos ) {	m_eye = pos; }
	inline	D3DXVECTOR3		*getPos( void )		{ return &m_eye;	}

	//< 이동 스피드
	inline	void			setMoveSpeed( float speed ) { m_moveSpeed = speed; }

private:
	//< 프로젝션 설정
	void					updateProj( void );
	//< 뷰설정
	void					updateView( void );
	//< 방향벡터 갱신
	void					updateDir( void );
	//< 우향벡터 갱신
	void					updateRight( void );
protected:
	//< 멤버변수
	//< 카메라 벡터정보
	D3DXVECTOR3				m_eye;
	D3DXVECTOR3				m_lookAt;
	D3DXVECTOR3				m_up;

	//< 우향벡터
	D3DXVECTOR3				m_right;
	//< 방향벡터
	D3DXVECTOR3				m_dir;
	//< 회전정보
	float					m_rotX;
	float					m_rotY;
	float					m_rotZ;

	//< 이동 스피드
	float					m_moveSpeed;

	//< 고정 거리 
	float					m_distance;
	//< 디바이스
	LPDIRECT3DDEVICE9		m_d3dDevice9;

	//< 뷰행렬
	D3DXMATRIX				m_view;
	//< 프로젝션행렬(투영)
	D3DXMATRIX				m_proj;
	//< 뷰 * 프로젝션(나중빌보드나 기타 잡것들을 위해)
	D3DXMATRIX				m_viewProj;

	//< 카메라 종류
	int						m_camKind;
};

}//< namespace end
