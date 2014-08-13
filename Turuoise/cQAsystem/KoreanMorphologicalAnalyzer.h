   */
#define HAM_RC_OK                'S'   /* ok, success            */
#define HAM_RC_GUESS             'W'   /* ok, but c-noun guessed */
#define HAM_RC_FAIL              'F'   /* analysis failed by HAM */

#define HAM_RC_HFDIC             'H'   /* analysis failed by HAM */

/*---------------  This is the end of io-defs.h -----------------*/
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
/*