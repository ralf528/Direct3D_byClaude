#include "stdafx.h"
#include "BASE_Object.h"

BASE_Object::BASE_Object(void)
{
	//< 포인터를 정수형으로 캐스팅 (해시)
	m_hash = reinterpret_cast<unsigned int>(this);
}

BASE_Object::~BASE_Object(void)
{
}

bool BASE_Object::isEquals( BASE_Object &obj )
{
	return ( m_hash == obj.m_hash );
}