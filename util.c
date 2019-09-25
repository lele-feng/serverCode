#if defined _WIN32 || defined _WIN64
#include <windows.h>
#elif _LINUX_
#include <iconv.h>
#endif

int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
#ifdef _WIN32
	return 0;  // Windows不需要
#endif

#ifndef _WIN32
	int nret = 0;
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
	{
		return -1;
	}
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
	{
		nret = -2;
	}
	iconv_close(cd);
	return nret;
#endif
}


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

char * LRemoveValue(char *src, char *dest, int num, char ch, char cp)
{
	int nct = 0;
	bool bfind = false;
	char *s0 = dest;
	while ((*src != 0) && (*src != '\n') && (nct < num))
	{
		if (*src != ch && *src != ' ')
		{
			if (bfind)
			{
				++nct;
			}
			*s0++ = *src++;
		}
		else
		{
			bfind = true;
			src++;
		}
	}

	while (nct < num)
	{
		*s0++ = cp;
		++nct;
	}

	*s0 = 0;
	return dest;
}


char * ltrim(char *str)
{
	char *s = str;
	while (*s != '\0')
	{
		if ((*s != ' ') && (*s != '\t') && (*s != '\n') && (*s != '\r'))
		{
			break;
		}
		s++;
	}
	if (s != str)memmove(str,s,strlen(s)+1);
	return str;
}

char * rtrim(char *str)
{
	char *s;
	s = str + strlen(str) -1;

	while ( ((s - str) > 0) &&
		    (*s == ' ' || *s == '\n' || *s == '\r'))
	{
		s--;
	}

    s++;
	*s = '\0';
	return str;
}

char * alltrim(char *str)
{
	ltrim(str);
	rtrim(str);
	return str;
}

void Trim(char *strbuf)
{
	if (strbuf == NULL)
	{
		return;
	}
	if (strbuf[strlen(strbuf)] != '\0')
	{
		strbuf[strlen(strbuf)] = '\0';
	}
	char *p1 = strbuf;
	while(*p1 != '\0')
	{
		if ((*p1 != ' ') && (*p1 != '\t') && (*p1 != '\n') && (*p1 != '\r'))
		{
			break;
		}
		p1++;
	}
	char *p2 = p1;
	for (int i = strlen(p1) - 1; i >= 0; i--)
	{
		if ((p2[i] != ' ') && (p2[i] != '\t') && (p2[i] != '\r') && (p2[i] != '\n'))
		{
			p2[i+1] = '\0';
			break;
		}
	}
	strncpy(strbuf,p2,strlen(p2)+1);
	return;
}
