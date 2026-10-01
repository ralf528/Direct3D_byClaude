#pragma once

#ifdef _MSC_VER
#ifdef _DEBUG
#define __CRTDBG_MAP_ALLOC			//< 메모리 누수 탐지를 위해서 선언해주어야 한다.
#include <crtdbg.h>
#ifndef _CONSOLE
#include <cstdlib>					//< 콘솔 프로그램일 경우 따로 선언
#endif

class MemoryLeak
{
//< 생성자
public:
	MemoryLeak( void )
	{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//< 콘솔 프로그램일 경우
#ifdef _CONSOLE
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );

		/*
			new로 할당된 메모리에 누수가 있을 경우 소스 상의 정확한 위치를 덤프해준다.
			※ _AFXDLL을 사용할 때는 자동으로 되지만, CONSOLE 모드에서 아래처럼 재정의를 해주어야 한다.
		*/
#define DEBUG_NORMALBLOCK new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#ifdef new
#undef new
#endif
#define new DEBUG_NORMALBLOCK

#else
		//< Send all reports to DEBUG window
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );
#endif
		
#ifdef malloc
#undef malloc
#endif
		/*
			malloc으로 할당된 메모리에 누수가 있을 경우 위치를 덤프
			CONSOLE 모드일 경우 crtdbg.h에 malloc이 정의되어 있지만,
			_AXFDLL 모드일 경우 약간 다른 방식으로 덤프하게 된다.
		*/
#define malloc( s ) ( _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ ) )
	}
};

//< 초기화를 생성자를 통해 자동으로 해주기 위해 전역으로 선언
static MemoryLeak memoryleak;

#endif
#endif