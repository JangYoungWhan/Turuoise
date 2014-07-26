/*
	File Name: cnoun.h
	Description: output structure for compound noun decomposition.
	Written by: Kang, Seung-Shik		04/11/1997
*/

/*                                                         */
/*     C-noun decomposition result structure               */
/*                                                         */
#define MAXCAND	50	/* 복합명사 분해 후보를 생성하는 최대 개수 */
#define MAXCOMP	16	/* 복합명사를 구성하는 단위명사들의 최대 개수 */

typedef struct ncomp {
    HAM_UCHAR ncmp; /* number of noun components in c-noun */
    HAM_UCHAR flag; /* 8-bit 0/1 for (un)registered compo. */
    HAM_UCHAR nsyl[MAXCOMP];/* num. of syllables for each compo.*/
    HAM_SHORT score;
} HAM_CNOUNSTR, *HAM_PCNOUNSTR;

/*---------------------------------------------------------*/
#define MAXnnn	15	/* see MAXCOMP --> 복합명사를 구성하는 단위명사들의 최대 개수 */

typedef struct hamcnoun {
    char nnn[MAXnnn+1];		/* 복합명사 경계 정보 (ex; 644) -- 바이트 길이 */
    char dic[MAXnnn+1];		/* 단위 명사의 사전 정보 -- S/s/U/D/P/k (우선순위 순서대로)
					S: 불용어 사전에 등록된 불용어
					s: 불용어 사전에 등록된 특수색인어
					U: 사용자 사전에 등록된 단위 명사
					D: 기분석 사전으로 분해된 복합명사의 단위 명사
					P: 사전에 등록된 단위 명사
					K: 사전에 수록되지 않은 미등록어 */
	int score;	/* best 분석결과 선택할 때 사용되는 점수 */
} HAM_CNOUN, *HAM_PCNOUN;
/*-------------------- end of cnoun.h ---------------------*/
