#include "cnoun.h"

extern PREFIX HAM_SHORT genNounCompKSC(
	HAM_PUCHAR word,	/* c-noun: KSC5601 code */
	HAM_PUCHAR nlist,	/* list of unit nouns */
	HAM_PSHORT n2,	/* 2nd 후보시작 index */
	HAM_UCHAR noun_delimiter,	/* noun-split mark like '_' or '\0' */
	HAM_PSHORT cnndic);	/* ham-cnn.dic에 있거나(1), 알고리즘으로 분해(0) */

extern PREFIX int genNounCompKSC_alnum(
	HAM_PUCHAR word,	/* c-noun: KSC5601 Hangul string */
	HAM_PUCHAR nlist,	/* list of unit nouns */
	char noun_delimiter,	/* noun-split mark like '_' or '\0' */
	int *cnndic,	/* ham-cnn.dic에 있거나(1), 알고리즘으로 분해(0) */
	int *puncflag);	/* 문장부호 분리되었는지(1), 아닌지(0) */

extern PREFIX int cnoun_decomposition(char *word, HAM_CNOUN cnn[]);
