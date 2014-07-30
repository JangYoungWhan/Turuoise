/*
	File Name: keyword.h
	Description:
		Keyword identifiers for automatic indexing.
		It is used only for automatic indexing.
	Written by: Kang, Seung-Shik		1997. 10. 2.
*/

#define MAXKEYWORDS	100
#define MAXKEYBYTES	3000

/* max. number of longwords in 1 sentence */
#define MAX_LWORDS	10

/*
	HAM-internal structure for keyword extraction
*/
typedef struct term_info {
	HAM_SHORT wid;	/* word-id */
	HAM_UCHAR tag;	/* type of term */
	HAM_PUCHAR key;	/* ptr. to term string */

	HAM_UCHAR freq; /* 출현 빈도수 */
	HAM_UCHAR tail; /* tail유형: 주격/목적격/소유격/하다/되다 등 */
	HAM_SHORT score; /* weight value for term ranking */
} HAM_TERMINFO, *HAM_PTERMINFO;

/*----------------------------------------------------------------
	Type of keywords are classified as follows.
----------------------------------------------------------------*/

/*
	1. Keyword types for Hangul noun

		1-syl. noun or 1-char alphanum : MARK_1SYL_NOUN
		nouns in dic. : MARK_NOUN
		compound noun : MARK_CNOUN
			c-noun component : MARK_NOUN_CMP
		unknown guess : MARK_UNKNOWN, MARK_UNKNOWN2, MARK_UNKNOWN3

		too long word : MARK_LONGWORD

		special words defined at 'stopword.dic' : MARK_SPECIAL

	2. Keyword types for alphanumerics : MARK_ALPHANUM

*/

/* nouns registered in dic : 2 or more syllables */
#define MARK_NOUN	'N'

/* 1-syllable Hangul noun or 1-char alphanumerics */
#define MARK_1SYL_NOUN	'1'

/* compound nouns: each registered in dic */
#define MARK_CNOUN	'C'

/* keyword: guessed as a noun or c-noun */
#define MARK_UNKNOWN	'K'

/* secondary guessed noun: almost garbages */
#define MARK_UNKNOWN2	'2'

/* secondary guessed noun: almost garbages */
#define MARK_UNKNOWN3	'3'

/* guessed nouns: more than 10 syllables */
#define MARK_LONGWORD	'L'

/* noun component for c-noun or guessed */
#define MARK_NOUN_CMP	'P'
#define MARK_NOUN_CMp	'p'
#define MARK_NOUN_CMP_cnndic	'D'

/* noun component for c-noun or guessed */
#define MARK_NOUN_CMP2	'Q'

/* special keywords of including alphanum at 'stopword.dic' */
#define MARK_SPECIAL	'S'

/*----------------------------------------------------------------
	한글과 영문자 or 숫자가 혼합된 단어
----------------------------------------------------------------*/

/* 영문자(A-Za-z)로 *시작*되는 것: English, vitamin-A, CD-ROM, LG-EDS 등 */
/* <주의> 'A', 'B'와 같이 1 문자로 된 것은 MARK_1SYL_NOUN임 */
#define MARK_ABC	'A'

/* 2문자이상_영문자 + 1음절이상_한글 : SK텔레콤, LG정보통신, CNN뉴스 */
/* 2음절이상_한글 + alphanum : 삼성SDS, 한국IBM, 비타민A, 한겨레21, 윈도95 */
#define MARK_HANGUL_ABC	'*'

/* 0-9까지 숫자로 *시작*되고 한글 없음 : 1st, 2nd, 123, 12.34 */
/* <주의> '1', '2'와 같이 1 문자로 된 것은 MARK_1SYL_NOUN임 */
#define MARK_123	'#'
/* 0-9까지 숫자로 *시작*되고 한글 있음 : 3개, 5원, 10만원, 21세기, 98년3월, 6시30분, 24회 등 */
#define MARK_123_HANGUL	'%'

/* 기타 : A그룹, B회사 등   --> 알파벳_1문자 + 한글 */
/*        제3(의), 제5회 등 --> 1음절_한글 + alphanumeric */
#define MARK_ALPHANUM	'@'

/*----------------------------------------------------------------
	MARK_123 중에서 한글수사 변환에 의하여 tag 수정
----------------------------------------------------------------*/
/* MARK_123 중에서 모두 숫자로 변환된 것 */
/* 백이십삼, 2백3십4, 삼천 이백 오십육 등 */
/* 123처럼 변환이 안된 것은 MARK_123으로 */
#define MARK_123_NUM1	'+'

/* MARK_123 중에서 prefix가 숫자로 변환된 것 */
/* 123원, 오천원(5000원), 5만2천달러(52000달러), 오천미터(5000미터) 등 */
#define MARK_123_NUM2	'$'

/*----------------------------------------------------------------
	Extended stems for verbs at. al.
----------------------------------------------------------------*/

/* verb, adjective, xverb */
#define MARK_VJXV	'V'
#define MARK_VMJ	'v'	/* 2008.05.15 강승식 추가 for '먹기가', '먹었음은' */

/* adverb, determiner, imperative */
#define MARK_AID	'W'	/* A/I/D로 구분을 못할 때 */
#define MARK_ADV	'a'
#define MARK_DET	'd'
#define MARK_IMP	'i'

/*----------------------------------------------------------------
	Ignore this special tag --- HAM internal-use only
----------------------------------------------------------------*/

/* stopwords: excluded by stopword list */
#define MARK_STOPWORD	'X'

/*----------------------------------------------------------------
	FOLLOWING TAGS ARE NOT USED YET !!!
----------------------------------------------------------------*/

/* input word itself */
#define MARK_INPUTWORD	'I'

/* KSC 5601 Hanja noun */
#define MARK_HANJA	'H'

#define MARK_PUNC	'.'
#define MARK_SYMBOL	'`'
/*---------------------- end of keyword.h ----------------------*/
