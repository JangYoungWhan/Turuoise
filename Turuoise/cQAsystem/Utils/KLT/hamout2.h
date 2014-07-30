/*
    File name: hamout2.h
    Description:
        API definition of HAM result for KLP application S/W.
		POS tag is based on 'tag-snu.h'.
    Written by: Kang, Seung-Shik    04/11/1997
*/

#include "tag-snu.h"

/*                                                             */
/*  Token-level result structure of HAM interface.             */
/*                                                             */
/*  Item 'pos' may have one of the following character.        */
/*    head --- 'N' 'V' 'B'(adverB) 'D'(Det) 'X'(eXcl) 'A'(SC)  */
/*    tail --- 'j' 'e' 's'(n-sfx) 't'(v-sfx) 'c'(opula)        */
/*             'f'(prefinal) 'x'(aux. verb)                    */
/*                                                             */
typedef struct hamresult {
    HAM_UCHAR     ntoken;         /* number of tokens in word  */
    HAM_UCHAR     pos[MAXTOKEN+1];/* part of speech of a token */
    HAM_PUCHAR    token[MAXTOKEN];/* char-ptr of token itself  */
    HAM_USHORT    ind;            /* dic. information of head  */
    HAM_UCHAR     patn;           /*     word pattern          */
    HAM_PJOMIVAR  jomi;           /* Josa/Eomi variant inf. ptr*/
    double        score;          /* score field for tagging   */
} HAM_RESULT2, HAM_FAR *HAM_PRESULT2;

/*                                                             */
/*  Word-level result structure of HAM output.                 */
/*                                                             */
/*  Maximum number of HAM analysys result --- MAXRESULT        */
/*                                                             */
typedef struct hamword {
    HAM_PUCHAR    phon;             /* phon. of the word       */
    HAM_SHORT     retcode;          /* HAM return code         */
    HAM_SHORT     nresult;          /* num. of results possible*/
    struct hamresult result[MAXRESULT2]; /* results of HAM     */
} HAM_WORD2, HAM_FAR *HAM_PWORD2;

/*                                                             */
/*  Sentence-level result structure of HAM output.             */
/*                                                             */
/*  This structure is a definition of the output area          */
/*    for the result of HAM. Output area is designed           */
/*    to process a 'sentence', not a word because it           */
/*    is helpful for us to analyze a text by sentence          */
/*    unit. Currently, sentence structure isn't used yet.      */
/*                                                             */
/*  This is a standard result structure of HAM for the         */
/*    application program interface.                           */
/*                                                             */
typedef struct hamsent {
    HAM_PUCHAR    phon;           /* raw string of input sent. */
    HAM_SHORT     type;           /* the type of a sentence    */
    HAM_SHORT     nword;          /* num. of words in the sent */
    struct hamword word[MAXWORDS];/* HAM result of each word   */
} HAM_MORES2, HAM_FAR *HAM_PMORES2;

/*-------------------- end of hamout2.h -----------------------*/
