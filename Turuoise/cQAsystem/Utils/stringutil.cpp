#include "stringutil.h"


void splitStringToNgram(std::list<String> *dest, const String &source, const char *delimiter, bool keepEmpty)
{
	size_t prev = 0;
	size_t next = 0;

	while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
	{
		if (keepEmpty || (next - prev != 0))
		{
			dest->push_back(source.substr(prev, next - prev));
		}
		prev = next + 1;
	}

	if (prev < source.size())
		dest->push_back(source.substr(prev));
}

void replaceAll(String &dest, const String &src, const String &pattern, const String &replace)
{
	dest = src;
	std::string::size_type pos = 0;
	std::string::size_type offset = 0;

	while((pos=dest.find(pattern, offset)) != std::string::npos)
	{
		dest.replace(dest.begin()+pos, dest.begin()+pos+pattern.size(), replace);
		offset = pos + replace.size();
	}
}

void filterSentence(String &dst, const String &src)
{
	int prev_lang = initCurrentLanguage(*(src.begin()), *(src.begin()+1));
	int curr_lang = prev_lang;
	for(auto iter=src.begin(); iter!=src.end(); iter++)
	{
		if(!isTwoBytes(*iter))
		{
			// Is a space or tab
			if(!(isWhiteSpace(*iter) < 0))
			{
				curr_lang = BLANK;
				if(prev_lang == curr_lang)
					continue;
			}
			// ASCII letters and numbers
			else if(isASCIInumber(*iter) || isASCIIalphabet(*iter))
			{
				curr_lang = ENGLISH;

				if(prev_lang != curr_lang)
					dst.push_back(ASCII_SPACE);

				dst.push_back(*iter);				
			}
			else
			{
				curr_lang = OTHERS;

				#ifndef _INGORE_SYMBOLS_
				dst.push_back(ASCII_SPACE);
				dst.push_back(*iter);
				#endif
			}
			prev_lang = curr_lang;
		}
		else
		{
			auto c1 = *iter;
			auto c2 = *(iter+1);

			if(!isKoreanSymbols(c1, c2)) // Korean, Janpaness, Hanja
			{
				if(isJapaness(c1, c2))
					curr_lang = JAPANAESS;
				else if(isHanja(c1, c2))
					curr_lang = HANJA;
				else
					curr_lang = KOREAN;

				if(prev_lang != curr_lang)
					dst.push_back(ASCII_SPACE);

				dst.push_back(c1);
				dst.push_back(c2);
			}			
			else // Korean symbols
			{
				curr_lang = OTHERS;

				#ifndef _INGORE_SYMBOLS_
				dst.push_back(ASCII_SPACE);
				dst.push_back(c1);
				dst.push_back(c2);
				#endif
			}
			iter++;
			prev_lang = curr_lang;
		}
	}
	if(isWhiteSpace(*(dst.begin())) == BLANK)
		dst.erase(dst.begin());
}

std::string convertFromUTF8ToANSI( const char *pszCode)
{
	BSTR    bstrWide;
	char*   pszAnsi;
	int     nLength;
	
	nLength = MultiByteToWideChar( CP_UTF8, 0, pszCode, strlen( pszCode) + 1, NULL, NULL);
	nLength = MultiByteToWideChar( CP_UTF8, 0, pszCode, strlen( pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen( NULL, nLength);

	MultiByteToWideChar( CP_UTF8, 0, pszCode, strlen( pszCode) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte( CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[ nLength];

	WideCharToMultiByte( CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString( bstrWide);

	std::string return_str = pszAnsi;
	delete[] pszAnsi;
	return return_str;
}

std::string convertFromANSIToUTF8( const char * pszCode)
{
	int	nLength, nLength2;
	BSTR bstrCode; 
	char *pszUTFCode = NULL;

	nLength = MultiByteToWideChar( CP_ACP, 0, pszCode, strlen( pszCode), NULL, NULL); 
	bstrCode = SysAllocStringLen( NULL, nLength); 
	MultiByteToWideChar( CP_ACP, 0, pszCode, strlen (pszCode), bstrCode, nLength);


	nLength2 = WideCharToMultiByte( CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL); 
	pszUTFCode = (char*)malloc( nLength2+1); 
	WideCharToMultiByte( CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2, NULL, NULL); 

	std::string return_str = pszUTFCode;
	free( pszUTFCode);
	return return_str;
}