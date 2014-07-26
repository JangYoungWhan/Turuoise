/*
	File Name: sizedefs.h
	Description:
		Constant definitions for HAM and KLP services.
		Max. size of sent, word, HAM results, ...
	Written by: Kang, Seung-Shik	04/11/1997
*/

#define LINESIZE	512

#define SENTSIZE	2048
	/* max. byte-size of Korean sentence  */
#define MAXWORDS	 100
	/* max. number of words in a sentence */

#define MAXRESULT	 10
	/* max. number of str-based HAM results */
#define MAXRESULT2	 7
	/* max. number of token-based HAM results */
#define MAXCANDI	 10
	/* max. number of verb candidates     */

#define WORDSIZE	 55
	/* maximum number of bytes for words  */
#define IWORDSIZ	 28
	/* max. no. of syllables in a word    */
#define STEMSIZE	 55	/* 40 -> 55: 2007.07.05 강승식 for "그러나얼마가지않아일제의식민지가되고말았습니다" */
	/* maximum number of bytes for stems  */

#define JOSASIZE	 25
	/* maximum number of bytes for Josa   */
#define IJOSASIZ	 13
	/* max. number of syllables for Josa  */
#define EOMISIZE	 15
	/* max. number of syllables for Eomi  */
#define IEOMISIZ	  8
	/* max. number of syllables for Eomi  */

#define SUFFSIZE	  5
	/* maximum number of bytes for suffix */
#define CMPDSIZE	 27
	/* max. byte-size of compound word    */
#define XVERBSIZ	  5
	/* max. number of bytes for c-noun    */

#define MAXTOKEN	15
	/* maximum number of tokens in a word */

/*---------------------- end of sizedefs.h -----------------------*/
