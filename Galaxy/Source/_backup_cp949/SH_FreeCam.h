#pragma once

#include "SH_iCamera.h"

namespace SH3D
{

class SH_FreeCam : public SH_iCamera
{
public:
	SH_FreeCam(void);
	virtual ~SH_FreeCam(void);

	//< 업데이트
	virtual void			update( float dt = 0.0f );
};

}