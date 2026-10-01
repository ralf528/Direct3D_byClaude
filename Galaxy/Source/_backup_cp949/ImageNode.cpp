#include "stdafx.h"
#include "ImageNode.h"


ImageNode::ImageNode(void)
{
	//< 초기화
	init();
}


ImageNode::~ImageNode(void)
{
	//< 해제
	release();
}

//< 로딩
bool ImageNode::load( const char *imagePath , int cx , int cy )
{
	if( NULL == imagePath )
	{
		//< 이미지 로딩 실패 메세지
		MessageBox( g_hWnd, "imageLoadFail","ImageError",MB_OK );
		return false;
	}

	//< 로딩시작
	//< DC얻기
	HDC hdc = GetDC( g_hWnd );

	//< MemoryDC생성
	m_memDC = CreateCompatibleDC( hdc );

	//< iamgeLoad
	m_bit = (HBITMAP)LoadImage( g_hInst, 
		imagePath, 
		IMAGE_BITMAP, 
		cx,
		cy,
		LR_LOADFROMFILE );

	//< 정보설정
	if( setImageInfo() == false )
	{

		return false;
	}

	//< 로딩실패
	if( NULL == m_bit )
	{
		//< 이미지 로딩 실패 메세지
		MessageBox( g_hWnd, "imageLoadFail","ImageError",MB_OK );
		return false;
	}

	//< 선택
	m_obit = (HBITMAP)SelectObject( m_memDC , m_bit );
		
	ReleaseDC( g_hWnd, hdc );
	return true;
}

//< RC로딩
bool ImageNode::load( const int resourceID )
{
	//< 로딩시작
	//< DC얻기
	HDC hdc = GetDC( g_hWnd );

	//< MemoryDC생성
	m_memDC = CreateCompatibleDC( hdc );

	//< iamgeLoad
	m_bit = LoadBitmap( g_hInst, MAKEINTRESOURCE( resourceID ) );

	//< 정보설정
	if( setImageInfo() == false )
	{

		return false;
	}

	//< 로딩실패
	if( NULL == m_bit )
	{
		//< 이미지 로딩 실패 메세지
		MessageBox( g_hWnd, "imageLoadFail","ImageError",MB_OK );
		return false;
	}

	//< 선택
	m_obit = (HBITMAP)SelectObject( m_memDC , m_bit );

	ReleaseDC( g_hWnd, hdc );
	return true;
}

//< 빈이미지만들기(해당 크기로 빈이미지 만들기 )디폴트로 흰색으로 칠한다.
bool ImageNode::load( int cx , int cy )
{
	//< 로딩시작
	//< DC얻기
	HDC hdc = GetDC( g_hWnd );

	//< MemoryDC생성
	m_memDC = CreateCompatibleDC( hdc );

	//< iamgeLoad
	m_bit = CreateCompatibleBitmap( hdc, cx, cy );

	//< 정보설정
	if( setImageInfo() == false )
	{
		return false;
	}

	//< 로딩실패
	if( NULL == m_bit )
	{
		//< 이미지 로딩 실패 메세지
		MessageBox( g_hWnd, "imageLoadFail","ImageError",MB_OK );
		return false;
	}

	//< 선택
	m_obit = (HBITMAP)SelectObject( m_memDC , m_bit );

	//< 흰색으로
	//< 백버퍼 DC를 초기화시킨다.
	RECT	winRect;
	GetClientRect( g_hWnd, &winRect );
	FillRect( m_memDC, &winRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	ReleaseDC( g_hWnd, hdc );
	return true;
}

//< 렌더
void ImageNode::render( HDC hdc, int x , int y )
{
	BitBlt( hdc, x,y, m_size.cx, m_size.cy, m_memDC,0,0,SRCCOPY );
}
void ImageNode::render( HDC hdc, int x , int y, int destCX , int destCY, 
						  int scrX , int srcY , DWORD mode  )
{
	BitBlt( hdc, x,y, destCX, destCY, m_memDC,scrX,srcY,mode );
}
//< 해제
void ImageNode::release( void )
{
	//< 오브젝트 삭제 및 초기화
	if( NULL != m_bit )
	{
		DeleteObject( SelectObject( m_memDC, m_obit ));
	}
	m_bit = NULL;
	m_obit = NULL;

	//< DC삭제 및 초기화
	if( NULL != m_memDC )
	{
		DeleteDC( m_memDC );
	}
	m_memDC = NULL;
}


//< 초기화
void ImageNode::init( void )
{
	//< 기존이미지 삭제 및 초기화
	release();

	//< 가로세로사이즈
	m_size.cx = 0;
	m_size.cy = 0;

	//< 경로
	//memset( m_path , 0, sizeof( TCHAR) * _MAX_FNAME ) ;
	ZeroMemory( m_path , sizeof( TCHAR) * _MAX_FNAME );
}
//< 정보설정
bool ImageNode::setImageInfo( void )
{
	//< bit확인
	if( NULL == m_bit )
	{
		return false;
	}

	//< 오브젝트 정보 설정
	BITMAP	bt;
	GetObject( m_bit , sizeof(BITMAP), &bt );

	//< 가로세로정보설정
	m_size.cx = bt.bmWidth;
	m_size.cy = bt.bmHeight;

	//< 경로설정

	//< 결과 반환
	return true;
}

