#include "stdafx.h"
#include "SH_FreeCam.h"

SH_FreeCam::SH_FreeCam(void)
:SH_iCamera(CAM_FREE)
{
}

SH_FreeCam::~SH_FreeCam(void)
{
}


//< 업데이트
void	SH_FreeCam::update( float dt )
{
	//< x회전테스트
	if( keyInput::isKeyDown( 'W' ) == true )
	{
		rotPitch( -0.01f );
	}

	if( keyInput::isKeyDown( 'S' ) == true )
	{
		rotPitch( 0.01f );
	}
	
	//< y회전 테스트
	if( keyInput::isKeyDown( 'A' ) == true )
	{
		rotYaw( -0.01f );
	}

	if( keyInput::isKeyDown( 'D' ) == true )
	{
		rotYaw( 0.01f );
	}

	//< y회전 테스트
	if( keyInput::isKeyDown( 'Z' ) == true )
	{
		rotRoll(-0.01f);
	}

	if( keyInput::isKeyDown( 'C' ) == true )
	{
		rotRoll(0.01f);
	}

	//< 전후진
	if( keyInput::isKeyDown( 'Q' ) == true )
	{
		moveLeftRight( -m_moveSpeed );
	}

	if( keyInput::isKeyDown( 'E' ) == true )
	{
		moveLeftRight( m_moveSpeed );
	}


	//< 전후진
	if( keyInput::isKeyDown( VK_UP ) == true )
	{
		moveForBackWard( m_moveSpeed );
	}

	if( keyInput::isKeyDown( VK_DOWN ) == true )
	{
		moveForBackWard( -m_moveSpeed );
	}
}