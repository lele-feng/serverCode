#if defined _WIN32 || defined _WIN64
#include <windows.h>
#elif _LINUX_
#include <iconv.h>
#endif

int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen)
{
#ifdef _WIN32	
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if (!lpGBKStr)
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, NULL, NULL);
	lpUnicodeStr = new WCHAR[nRetLen + 1];
	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, lpUnicodeStr, nRetLen);
	if (!nRetLen)
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL);

	if (!lpUTF8Str)
	{
		if (lpUnicodeStr)
			delete[]lpUnicodeStr;
		return nRetLen;
	}

	if (nUTF8StrLen < nRetLen)
	{
		if (lpUnicodeStr)
			delete[]lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, (char *)lpUTF8Str, nUTF8StrLen, NULL, NULL);

	if (lpUnicodeStr)
		delete[]lpUnicodeStr;

	return nRetLen;
#else
	int nRetLen = 0;
	code_convert((char *)"gb2312", (char *)"utf-8", (char *)lpGBKStr, strlen((const char *)lpGBKStr), (char *)lpUTF8Str, nUTF8StrLen);
	nRetLen = nUTF8StrLen;
	return nRetLen;
#endif	
}

int UTF8ToGBK(unsigned char * lpszUtf8, unsigned char *lpszGbk, int nGbkLen)
{
#ifdef _WIN32
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)lpszUtf8, -1, NULL, 0);
	wchar_t * wszGBK = new wchar_t[len];
	memset(wszGBK, 0, len);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)lpszUtf8, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	strncpy((char *)lpszGbk, szGBK, nGbkLen);
	delete[] szGBK;
	delete[] wszGBK;

	return len;
#else
	int nRetLen = 0;
	code_convert((char *)"utf-8", (char *)"gb2312", (char *)lpszUtf8, strlen((const char *)lpszUtf8), (char *)lpszGbk, nGbkLen);
	nRetLen = nGbkLen;
	return nRetLen;

#endif
}


