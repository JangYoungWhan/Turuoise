#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_


#include <list>
#include <string>
#include "StdRedef.h"

// it is used for ignore ASCII and Korean symbols.
#define _INGORE_SYMBOLS_

enum { ENGLISH, KOREAN, JAPANAESS, HANJA, OTHERS, BLANK };

unsigned const char ASCII_SPACE	= 0x20;
unsigned const char ASCII_TAB	= 0x09;

void splitStringToNgram(std::list<String> *dest, const String &source, const char *delimiter=" ", bool keepEmpty=false);
void replaceAll(String &dest, const String &src, const String &pattern, const String &replace);
void filterSentence(String &dst, const String &src);


inline
int isWhiteSpace(const char &ch)
{
	if(ch==ASCII_SPACE || ch==ASCII_TAB)
		return BLANK;
	else
		return -1;
}

inline
bool isASCIInumber(const char &ch)
{
	if(0x30<=ch && ch<=0x39)
		return true;
	else
		return false;
}

inline
bool isASCIIalphabet(const char &ch)
{
	if((0x41<=ch && ch<=0x5A) || (0x61<=ch && ch<=0x7A))
		return true;
	else
		return false;
}

inline
bool isKoreanSymbols(const unsigned char &ch1, const unsigned char &ch2)
{
	if(((0xA1<=ch1 && 0xA1<=ch2) && (ch1<=0xA1 && ch2<=0xFE)) || ((0xA2<=ch1 && 0xA1<=ch2) && (ch1<=0xA2 && ch2<=0xE7)))
		return true;
	else
		return false;
}

inline
bool isJapaness(const unsigned char &ch1, const unsigned char &ch2)
{
	// KATAKANA and HIRAGANA
	if(((0xAB<=ch1 && 0xA1<=ch2) && (ch1<=0xAB && ch2<=0xF6)) || ((0xAA<=ch1 && 0xA1<=ch2) && (ch1<=0xAA && ch2<=0xF3)))
		return true;
	else
		return false;
}

inline
bool isHanja(const unsigned char &ch1, const unsigned char &ch2)
{
	if((0xCA<=ch1 && 0xA1<=ch2) && (ch1<=0xFD && ch2<=0xFE))
		return true;
	else
		return false;
}

inline
bool isTwoBytes(const unsigned char &ch)
{
	if((ch & 0x80) == 0x80)
		return true;
	else
		return false;
}

inline
int initCurrentLanguage(const unsigned char &ch1, const unsigned char &ch2)
{
	if(!isTwoBytes(ch1))
	{
		if(!(isWhiteSpace(ch1) < 0))
			return BLANK;
		if(isASCIInumber(ch1) || isASCIIalphabet(ch1))
			return ENGLISH;
		else
			return OTHERS;
	}
	else
	{
		if(isJapaness(ch1, ch2))
			return JAPANAESS;
		else if(isHanja(ch1, ch2))
			return  HANJA;
		else
			return KOREAN;
	}
}


#endif