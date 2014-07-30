/*
	File Name: io-defs.h
	Description:
		header file collections for HAM
	Written by:
		Kang, Seung-Shik		04/11/1997
*/

/*                                                          */
/*           Definition of HAM running mode                 */
/*              and Hangul I/O interface.                   */
/*                                                          */
#define TESTING		0x00    /* interactive testing mode */
#define ANALYSIS	0x01    /* morphological analysis   */
#define SPELLING	0x02    /* or spelling check mode   */
#define INDEXING	0x03    /* keyword extraction mode  */
#define PARSING		0x04    /* simple dependency parsing*/
#define SPACING		0x05    /* automatic word spacing   */


/*                                                          */
/*          Various output mode definition for HAM          */
/*                                                          */
#define UNK_NORMAL	0	/* optimal output level */
#define BEST1_ONLY	1	/* printout best 1 result */
#define BEST2_ONLY	2	/* printout best 2 results */
#define UNK_XPND_1	3	/* expand unknown guessing */
#define UNK_XPND_2	4	/* unknown guessing to analed O.K */

/*                                                          */
/*          Various output style definition for HAM         */
/*                                                          */
#define OUTSTYLE_NORMAL	0
#define OUTSTYLE_PAREN	1
#define OUTSTYLE_SLASH	2

/*                                                          */
/*           Various output mode definition for HAM         */
/*                                                          */
#define OUTMODE_ALL	0x00	/* HMA analyzed & temporary candi's */
#define OUTMODE_HAM	0x01	/* HAM analysis results */
#define OUTMODE_TOKEN	0x02	/* token-based for SNU-tag  */
#define OUTMODE_KONAN	0x03	/* token-based for ETRI-tag */
#define OUTMODE_BOTH	0x04	/* HAM & SNU tagset results */

/*
	Following out-modes are not implemented yet
*/
#define OUTMODE_FILE	0x07	/* classify the results */

#define OUTMODE_NOUN	0x11	/* some aux-verb, affixes   */
				/* are isolated from a stem */
#define OUTMODE_NOUNK_1	0x12	/* 'xxxx+noun' pattern is   */
#define OUTMODE_NOUNK_2	0x13	/* 'xxxx+noun' pattern is   */
				/* regarded as a c-noun     */
#define OUTMODE_VERB	0x14	/* 'Ascii+{Hangul}' pattern */
#define OUTMODE_ETC	0x15	/* 'unknown + Josa' pattern */
				/* is allowed for a speller */
#define OUTMODE_SPLAID	0x16	/* spelling aid */


/*
	Generally used values
*/
#define YES		1
#define NO		0

#define TRUE	1
#define FALSE	0


/*
	Hangul code definition
*/
#define KSSM		0
#define KSC5601		1
#define UTF8		2
//#define UNICODE		3

/*
	Josa/Eomi output form
*/
#define JOMI_AOI	0
#define JOMI_BAS	1
#define JOMI_ORG	2

/*
	suffix analysis option for noun
*/
#define COMMON_SFX	0x00    /* general suffixes only    */
#define EXTEND_SFX	0x01    /* extended suffixes added  */


/*                                                               */
/*                                                               */
/*        Definition of values for output return code            */
/*                                                               */
/*                                                               */
#define HAM_RC_OK                'S'   /* ok, success            */
#define HAM_RC_GUESS             'W'   /* ok, but c-noun guessed */
#define HAM_RC_FAIL              'F'   /* analysis failed by HAM */

#define HAM_RC_HFDIC             'H'   /* analysis failed by HAM */

/*---------------  This is the end of io-defs.h -----------------*/
