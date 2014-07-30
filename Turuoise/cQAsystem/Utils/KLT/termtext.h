/*=====================================================================
	한 문장 및 문서 전체에 대한 색인어 추출
		- unify에 의한 TF 계산
=====================================================================*/
/* 수식명사(부사성 명사)의 최대수: modinoun.dic */
#define MAX_MODINOUN	1000

/* 분야별 사전 최대 용어 수: domain.dic */
#define MAX_DOMAINNOUN	50000

/* 문장 or 문서에서 색인어 추출에 필요한 최대 용어수 */
#define MAX_TERMS_SENT		 100	/* 한 문장에서 추출되는 최대 용어 개수 */
#define MAX_TERM_BYTES_SENT	2000	/* term string 저장에 필요한 최대 바이트 수 */

#define MAX_TERMS_DOC		 2000	/* 한 문서 파일에서 추출되는 최대 용어 개수 */
#define MAX_TERM_BYTES_DOC	1000000	/* term string 저장에 필요한 최대 바이트 수 */

#define MAX_LOCS_PER_TERM	20/*1024*/	/* 문서당 term의 출현 위치 최대 개수 */

/* 한 문장 or 문서에 대한 term extraction 결과 저장 구조 */
typedef struct TermList {
	long offset;				/* term string 시작 위치 */
	unsigned char pos;			/* term type */
	unsigned char tf;			/* term frequency: max. 255 */
	unsigned short score;		/* term weight */
	unsigned short loc[MAX_LOCS_PER_TERM];	/* term 출현 위치 최대 개수 */
} HAM_TERMLIST, *HAM_PTERMLIST;

/* 한 문서에 대한 term extraction에 필요한 memory */
typedef struct TermMemory {
	long memTermIndex;	/* 'memTermString[]'의 free 부분 index */
	unsigned char memTermString[MAX_TERM_BYTES_DOC];	/* 문서 용어 저장공간 */

	/* 한 문장씩 분석하는데 필요한 memory */
	unsigned char *terms[MAX_TERMS_SENT];			/* 문장에서 추출된 용어 정보 */
	unsigned char termMem[MAX_TERM_BYTES_SENT];		/* 한 문장 용어 저장공간 */
	HAM_MORES hamOut;		/* 한 문장에 대한 형태소 분석 결과 저장공간 */
} HAM_TERMMEMORY, *HAM_PTERMMEMORY;
