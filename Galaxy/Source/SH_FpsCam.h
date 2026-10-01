#pragma once

#include "SH_iCamera.h"

namespace SH3D
{

class SH_FpsCam : public SH_iCamera
{
public:
	SH_FpsCam(void);
	virtual ~SH_FpsCam(void);

	//< 카메라 설정
	virtual HRESULT		initCam( D3DXVECTOR3 const &eye, 
						D3DXVECTOR3 const &up,
						D3DXVECTOR3 const &lookAt);

	//< 업데이트
	virtual void		update( float dt = 0.0f );

	//< 마우스 메세지
	virtual bool		mouseMessage( HWND wnd, UINT message, WPARAM wparam, LPARAM lparam );

private:
	//< FPS설정
	void				setFPS( void );
	//< 커서정보 설정
	void				checkRot( void );

private:
	POINT				m_winCenterPos;	//< 마우스 좌표설정 
};

}//< namespace SH3D