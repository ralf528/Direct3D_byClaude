#pragma once

class ImageNode
{
public:
	ImageNode(void);
	~ImageNode(void);

	//< 로딩
	bool		load( const char *imagePath , int cx = 0, int cy = 0 );
	//< RC로딩
	bool		load( const int resourceID );
	//< 빈이미지만들기(해당 크기로 빈이미지 만들기 )디폴트로 흰색으로 칠한다.
	bool		load( int cx , int cy );

	//< 렌더
	void		render( HDC hdc, int x , int y );
	void		render( HDC hdc, int x , int y, int destCX , int destCY, 
						int scrX = 0, int srcY = 0, DWORD mode = SRCCOPY );
	//< 해제
	void		release( void );

	//< 외부에서 MEMDC얻기
	inline HDC	getMemDC( void )	{ return m_memDC;	}
	//< 사이즈얻기
	inline SIZE getSize( void )		{ return m_size;	}

private:
	//< 초기화
	void		init( void );
	//< 정보설정
	bool		setImageInfo( void );

private:
	//< 메모리DC
	HDC			m_memDC;
	//< 비트맵핸들
	HBITMAP		m_bit,m_obit;
	//< 가로세로사이즈
	SIZE		m_size;
	//< 경로
	TCHAR		m_path[ _MAX_FNAME ];
};

