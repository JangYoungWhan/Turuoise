/*
	File name: hamout1.h
	Description:
		Definition of output area for HAM.
		Output unit is a sentence to make room for enhancement.
	Written by: Kang, Seung-Shik	04/11/1997
*/

/*                                                         */
/*       Josa/Eomi/Pomi variant information                */
/*                                                         */
/* See a file 'jomi-var.h' for the value of first 4 items. */
/*                                                         */
typedef struct jomi_variant {
    HAM_UCHAR eomi;             /* Eomi 'A/O' variant type */
    HAM_UCHAR pomi;             /* Pomi 'A/O' variant type */

    HAM_UCHAR xomitype;         /* Eomi before aux. verb   */
                 /* 0: 'Wb/Wf', 1: 'Al', 2: 'Ag', 11: 'Xi' */
    HAM_UCHAR xomi;             /* Eomi 'A/O' variant type */

    HAM_UCHAR josa;             /* Josa variants type      */
    HAM_UCHAR josaAgBg;         /* 'Ag/Bg/hbDtg' info.     */
                      /* 0:WgAg, 1:'Ag', 2:'Bg', 3:'haDtg' */
    HAM_UCHAR zzz;              /* reserved for other use  */
} HAM_JOMIVAR, *HAM_PJOMIVAR;

/*                                                         */
/*       Result-level output structure of HAM              */
/*                                                         */
typedef struct ham_result {     /*   형태소 분석 결과      */
    HAM_SHORT score;            /* score of this result    */

    HAM_UCHAR patn;             /*     word pattern        */
    HAM_UCHAR type;             /* type of input word      */

    HAM_UCHAR stem[STEMSIZE];   /* stem of input word      */
    HAM_UCHAR pos;              /* 3 simplified stem type  */
    HAM_UCHAR pos2;             /* pos attr. for 'pos'     */
    HAM_UCHAR dinf;             /* POS info. in Han-dic    */
    HAM_UCHAR nsfx;             /* index of noun suffix    */

    HAM_UCHAR vsfx;             /* index of verb suffix    */

    HAM_UCHAR josa[JOSASIZE];   /*     Josa string         */
    HAM_PUCHAR jlist;           /*  unit-Josa sequence     */
    HAM_UCHAR eomi[EOMISIZE];   /*     Eomi string         */
    HAM_PUCHAR elist;           /*  unit-Eomi sequence     */
    HAM_UCHAR pomi;             /* encoded prefinal Eomi   */

    HAM_UCHAR xverb[XVERBSIZ];  /*     xverb string        */

    /* morphological attributes */
    HAM_UCHAR vtype;            /* irregular verb type     */
    struct jomi_variant jomi;   /* Josa/Eomi variant info. */
} HAM_RESULT, *HAM_PRESULT;

/*                                                         */
/*       Word-level output structure of HAM                */
/*                                                         */
typedef struct ham_word {
    HAM_UCHAR phon[WORDSIZE];   /* string of input word    */
    HAM_SHORT retcode;          /* HAM return code         */
    HAM_SHORT nresult;          /* number of HAM results   */
    struct ham_result result[MAXRESULT];  /* HAM results   */
    HAM_UCHAR gr[MAXRESULT+1];  /* grade index of 'result' */
        /* gr[0] : recommended number of HAM candidates    */
        /* gr[1], gr[2], ...: index of 1st, 2nd, ... grade */
} HAM_WORD, *HAM_PWORD;

/*                                                         */
/*     Sentence-level output structure of HAM              */
/*                                                         */
typedef struct sentence {
    HAM_UCHAR phon[SENTSIZE];     /* raw string of sentence*/
    HAM_SHORT type;               /* type of the sentence  */
    HAM_SHORT nword;              /* num. of words in sent */
    struct ham_word word[MAXWORDS]; /* array of each word  */
} HAM_MORES, *HAM_PMORES;

/*------------------- end of hamout1.h --------------------*/

/*	<<< 고려사항 >>>
	1. '아/어'를 복원한 것과 그렇지 않은 것 구분
	2. '이' 생략을 복원한 것과 그렇지 않은 것 구분
	3. 보조용언 앞에 어미의 종류 구분: '게/고'
*/
