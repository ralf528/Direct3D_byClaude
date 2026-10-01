#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <ctime>
#include <MMSystem.h>	///< DugOutputString을 사용하기 위한 헤더
#include <atlconv.h>	///< BSTR등을 사용하기 위한 헤더 
#include "Utility.h"


extern char * Format(const  char * pFormat, ...)
{
	static char szBuffer[MAX_BUFFER_ROW][MAX_BUFFER_COLUMN];
	static int nIndex = 0;
	// 두개의 버퍼를 만들어 교대로 사용
	char* pBuffer = szBuffer[nIndex++&1];
	// 가변인자값을 버퍼에 저장
	va_list vaList;
	va_start(vaList, pFormat);
	vsprintf_s(pBuffer, MAX_BUFFER_COLUMN, pFormat, vaList);
	va_end(vaList);

	return pBuffer;
}

extern  VOID  PrintDebugStr(const  char * pFormat, ...)
{
	static char szBuffer[MAX_BUFFER_ROW][MAX_BUFFER_COLUMN];
	static int nIndex = 0;
	// 두개의 버퍼를 만들어 교대로 사용
	char* pBuffer = szBuffer[nIndex++&1];
	// 가변인자값을 버퍼에 저장
	va_list vaList;
	va_start(vaList, pFormat);
	vsprintf_s(pBuffer, MAX_BUFFER_COLUMN, pFormat, vaList);
	va_end(vaList);

	OutputDebugString(pBuffer);
}

/**
 * \brief	: 문자열 strSrc의 특정 문자를 다른 문자로 치환해주는 함수 
 * \param strSrc : 원본 문자열( 바뀔문자열)
 * \param strFind : 찾을 문자열
 * \param strDest : 바꿀 문자열
 */
extern  VOID  RePlaceAll(std::string& strSrc, const std::string& strFind, const std::string& strDest)
{   
    size_t location;   
	///< 해당 문자의 위치를 찾는다.
	while ((location = strSrc.find(strFind)) != std::string::npos)   
        strSrc.replace(location, strFind.length(), strDest);	///< 치환한다.
}   
 

/**
* \brief	: UTF8 인코딩된 스트링을 wchar_t 인코딩으로 변환을 한다.
 * \param pUTF8  : UTF8 인코딩된 스트링
 * \param rUnicode : 결과를 받을 wstring
 * \return 
 */
bool Str_UTF8ToUnicodeDynamic(const char* pUTF8, std::wstring& rUnicode)
{
	assert(pUTF8 != NULL);

	int nBufferSize = (static_cast<int>(strlen(pUTF8)) + 1);
	wchar_t * wszUnicode = NULL;

	wszUnicode = new wchar_t[nBufferSize];

	const int nResult = ::MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, wszUnicode, nBufferSize);

	if (nResult == ERROR_NO_UNICODE_TRANSLATION)
	{
		SAFE_DELETE_ARRAY(wszUnicode);
		//ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("Str_UTF8ToUnicode(ERROR_NO_UNICODE_TRANSLATION) failed!\n")), false);
		return false;
	}

	if (nResult <= 0)
	{
		SAFE_DELETE_ARRAY(wszUnicode);
		// not enough buffer?
		return false;
	}

	rUnicode = wszUnicode;
	SAFE_DELETE_ARRAY(wszUnicode);
	return true;
}

bool Str_UnicodeToCharDynamic(const wchar_t* pUni, std::string& rACP)
{
	assert(pUni != NULL);

	int nBufferSize = (static_cast<int>(wcslen(pUni)) + 1);
	char * szACP = NULL;

	szACP = new char[nBufferSize];
	int nACP = GetACP();

	const int nResult = ::WideCharToMultiByte(nACP, 0, pUni, -1, szACP, nBufferSize, NULL, NULL);

	//if (nResult == ERROR_NO_UNICODE_TRANSLATION)
	//{
	//	SAFE_DELETE_ARRAY(wszUnicode);
	//	//ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("Str_UTF8ToUnicode(ERROR_NO_UNICODE_TRANSLATION) failed!\n")), false);
	//	return false;
	//}

	//if (nResult <= 0)
	//{
	//	SAFE_DELETE_ARRAY(szACP);
	//	// not enough buffer?
	//	return false;
	//}

	rACP = szACP;
	SAFE_DELETE_ARRAY(szACP);
	return true;
}


 VOID  str_UTF8ToAnsi(char* pszCode , std::string& strOut)
{
	BSTR bstrWide;
	char* pszAnsi;
	int nLength;

	nLength = MultiByteToWideChar(CP_UTF8,0,pszCode,lstrlen(pszCode)+1,NULL,NULL);

	bstrWide = SysAllocStringLen(NULL,nLength);
	MultiByteToWideChar(CP_UTF8,0,pszCode,lstrlen(pszCode)+1,bstrWide,nLength);

	nLength = WideCharToMultiByte(CP_ACP,0,bstrWide,-1,NULL,0,NULL,NULL);

	pszAnsi = new char[nLength];

	WideCharToMultiByte(CP_ACP,0,bstrWide , -1, pszAnsi , nLength , NULL ,NULL);

	SysFreeString(bstrWide);

	strOut = pszAnsi;

	delete [] pszAnsi;
	//return pszAnsi;
}

/**
 * \brief	: Ansi 에서 wstring으로 변환
 * \param ascstring : 변환할 ansistring
 */
std::wstring wstrFromAsc( const std::string& ascstring  )
{
	return  wstrFromCodePage( CP_ACP, ascstring ) ;
}

std::string wstrToAsc(const std::wstring& widestring)
{  
	return strToCodePage( CP_ACP, widestring );
}


std::string strToCodePage(unsigned int codepage, const std::wstring& widestring)
{
	int out_size = ::WideCharToMultiByte( codepage, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL );
	if (out_size == 0)
	{
		//		throw GCN_EXCEPTION("Error in conversion.");
		return NULL;
	}

	std::vector<char> resultstring(out_size);

	int convresult = ::WideCharToMultiByte(codepage, 0, widestring.c_str(), -1, &resultstring[0], out_size, NULL, NULL);

	if (convresult != out_size)
	{
		//		throw GCN_EXCEPTION("Utf16 - Codepage Conversion error!");
		return NULL;
	}

	return std::string(&resultstring[0]);
}




/**
 * \brief	: 코드페이지에 따라서 wstring으로 변환 
 * \param codpage : 코드 페이지 ( ansi , utf8)
 * \param utf8string : 변환할 문자열 
 */
std::wstring wstrFromCodePage( unsigned int codpage , const std::string& utf8string )
{

	int widesize = ::MultiByteToWideChar( codpage, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		printf("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		printf("Error in conversion.");
	}

	std::vector<wchar_t> resultstring(widesize+1);

	int convresult = ::MultiByteToWideChar(codpage, 0, utf8string.c_str(), -1, &resultstring[0], widesize+1);

	if (convresult != widesize)
	{
		printf("Utf8 - Utf16 Conversion error!");
	}

	return std::wstring(&resultstring[0]);
}

int CalcTotalDay(int nYear, int nMonth, int nDay)
{
	int nTotalDays = 0;

	const int Month[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
	nTotalDays = (nYear-1)*365;	// 년도에 따른 일수 계산
	nTotalDays += ((nYear-1)/4 - (nYear-1)/100 + (nYear-1)/400);	// 윤년을 계산하여 일수 가감
	nTotalDays += (Month[nMonth-1] + nDay);	// 입력 월일의 일수 계산
	//
	if (!(nYear%4) && nMonth>2)		// 입력된 년도가 윤년이고 2월이 지났다면
	{
		nTotalDays++;
		if (!(nYear%100)) // 만약, 100의 배수인 해이면
		{
			nTotalDays--;
		}
		if (!(nYear%400)) // 만약, 400의 배수인 해이면
		{
			nTotalDays++;
		}
	} 

	return nTotalDays;
}

int GetD_Day(int nTargetYear, int nTargetMonth, int nTargetDay)
{
	time_t curTime;
	curTime  = time(NULL);
	struct tm * pCurDateTime = NULL;
	localtime_s(pCurDateTime,&curTime);
	int nCurTotal = CalcTotalDay(pCurDateTime->tm_year + 1900, pCurDateTime->tm_mon + 1, pCurDateTime->tm_mday);
	int nExpTotal = CalcTotalDay(nTargetYear, nTargetMonth, nTargetDay);
	return nExpTotal - nCurTotal;
}