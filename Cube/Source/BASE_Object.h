#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

class BASE_Object
{
public:
	BASE_Object(void);
	virtual ~BASE_Object(void);

	//< 오브젝트 이름 (타입)
	virtual const TCHAR *toString(void) { return "BASE_Object"; }

	//< 오브젝트 고유 hash 번호
	inline unsigned int getHashCode(void) { return m_hash; }

	//< 오브젝트 비교
	virtual bool isEquals( BASE_Object& obj );

	//< 오브젝트 삭제
	virtual void release(void)				{}
	//< 오브젝트 갱신
	virtual void update(FLOAT fDT = 0.0f)	{}
	//< 오브젝트 랜더
	virtual void render(HDC hdc)			{}
	
	//< 3D 오브젝트 랜더
	virtual HRESULT render(void)			{ return S_OK; }

	//< 디바이스 해제
	virtual HRESULT lostDevice(void)		{ return S_OK; }
	//< 디바이스 복구
	virtual HRESULT resetDevice(void)		{ return S_OK; }


private:
	//< 고유 hash 번호
	unsigned int m_hash;
};

#endif