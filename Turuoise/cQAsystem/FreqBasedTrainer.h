 code */
	HAM_PUCHAR nlist,	/* list of unit nouns */
	HAM_PSHORT n2,	/* 2nd 후보시작 index */
	HAM_UCHAR noun_delimiter,	/* noun-split mark like '_' or '\0' */
	HAM_PSHORT cnndic);	/* ham-cnn.dic에 있거나(1), 알고리즘으로 분해(0) */

extern PREFIX int genNounCompKSC_alnum(
	HAM_PUCHAR word,	/* c-noun: KSC5601 Hangul string */
	HAM_PUCHAR nlist,	/* list of unit nouns */