/*
	File Name: tag-snu.h
	Description: Hangul POS tags defined for S.N.U. K-J M.T. system.
		It is only used for token-based output result.
	Written by: Kang, Seung-Shik	04/11/1997
*/
/* N(체언): NPUM을 포괄, N/P/U/M --> '체언'에 대한 세부 태그 */
#define HTAG_NOUN	'N'
#define HTAG_PNOUN	'P'
#define HTAG_XNOUN	'U'
#define HTAG_NUMER	'M'

#define HTAG_ASC	'A'

/* V(용언): VJ을 포괄, V/J --> '용언'에 대한 세부 태그 */
#define HTAG_VERB	'V'
#define HTAG_XVERB	'W'
#define HTAG_ADJ	'J'
#define HTAG_XADJ	'K'	/* NOT USED YET */

#define HTAG_ADV	'B'
#define HTAG_DET	'D'
#define HTAG_EXCL	'L'

#define HTAG_JOSA	'j'
#define HTAG_CORP	'c'
#define HTAG_EOMI	'e'
#define HTAG_POMI	'f'

#define HTAG_NSFX	's'
#define HTAG_VSFX	't'

#define HTAG_PUNC	'q'
#define HTAG_SYMB	'Q'
/*------------------- end of tag-snu.h --------------------*/
/*	예전 태그를 아래와 같이 수정했음: 2001.07.27 강승식
	'x' --> 'W' 
	'C' --> 'q'
*/
