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

int GetDateString(int ndate, char *szdatestring, int nlen)
{
	int year = ndate / 10000;
	int month = ndate % 10000 / 100;
	int day = ndate % 10000 % 100;

	sprintf(szdatestring, "%d年%d月%d日", year, month, day);

	return 0;
}

int GetTimeString(int ntime, char *sztimestring, int nlen)
{
	int day = ntime / (60 * 24);
	int hour = (ntime - day * 60 * 24) / 60;
	int minute = ntime - day * 60 * 24 - hour * 60;

	sprintf(sztimestring, "%d天%d小时%d分", day, hour, minute);

	return 0;
}

char * stringtoupper(char *s)
{
   register int i;

   if(s) {
     for(i=((int)strlen(s) - 1); i >= 0; i--)
       s[i] = toupper(s[i]);
   }

   return s;
}

char * GetValue(char *src,char *dest ,int num,char ch)
{
	char *s0=dest;
	dest[0]=0;
	while (*src!=0 && *src!='\n' && num!=1)
	{
		if (*src==ch)
		  num--;
		src++;
	}
	while (*src!=0 && *src!='\n' && *src!=ch)
	*s0++=*src++;
	*s0=0;
	return dest;
}

