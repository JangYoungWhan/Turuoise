/*
	File name: parsek.h
	Description: data structure for Korean parsing
	Written by: Kang, Seung-Shik	1998. 11. 17.
*/
#include "parsed.h"

typedef struct kparword {
	unsigned char *word;	/* input word */
	int wid;	/* i-th 'word' in the sentence */
	int offset;	/* byte-offset of 'word' */

	unsigned char *base;	/* base-form of 'word': with suffix */
		/* '하/되' 등 용언화 접미사가 결합된 형태 */
		/* '화/성/용' 등 사전 엔트리로 포함될 수 있는 것 결합 */
		/* '등/들' 등 사전 엔트리로 포함될 수 없는 것은 결합 안함 */
	unsigned char *stem;	/* stem-form of 'word': no suffixes */
		/* 형태소 분석결과의 stem과 동일 */
	unsigned char *suffix;
	
	unsigned char bpos;	/* P.O.S for 'base' */
	unsigned char spos;	/* P.O.S for 'stem' */

	unsigned char *josa;	/* 조사 */
	unsigned char *eomi;	/* 어말어미 */
	unsigned char *pomi;	/* 선어말어미 */
	unsigned char *xverb;	/* 보조용언 */

	unsigned char rel;	/* dependency relation type? */
				/* 용언 head: 주어, 목적어, 부사어, 부사 */
				/* 체언 head: 관형어, 관형어미, ... */

	int nchild;	/* number of daughters */
	struct kparword *first;	/* first child */
	struct kparword *last;	/* last child */

	struct kparword *mom;	/* pointer to the mother node */
	struct kparword *left;	/* pointer to the left sister */
	struct kparword *right;	/* pointer to the right sister*/
} KPAR_WORD, *KPAR_PWORD;

/*                                                      */
/*       parsing result -- head-daughter structure      */
/*                                                      */
typedef struct kparsent {
	unsigned char *sent;	/* sentence string */
	KPAR_PWORD root;	/* root node */

	DPAR_PSENT dpr;	/* pointer to dep. parsing result */
} KPAR_SENT, *KPAR_PSENT;
/*---------------------- end of parsek.h ------------------------*/
