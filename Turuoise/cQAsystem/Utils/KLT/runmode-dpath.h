/*
	File name: runmode.h
	Description: HAM options for various application
	Created by: Kang, Seung-Shik		07/20/1997
	Last Modified: 04/18/2005
*/
#define YEAR	2010
#define VERSION_NUMBER	"2.2.0"

#include <limits.h>
#ifdef PATH_MAX
#define DIC_SIZ	PATH_MAX	/* 2005.04.11 °­½Â½Ä: 128 -> 512 */
#else
#define DIC_SIZ	512	/* 2005.04.11 °­½Â½Ä: 128 -> 512 */
#endif

/*
	Keyword extracting options.
*/
typedef struct index_mode {
	HAM_UCHAR alphanum;	/* words with alphanum characters */
	HAM_UCHAR syl_1;	/* include 1-syl. Hangul noun */
	HAM_UCHAR verbs;	/* include verbs as a keyword */
	HAM_UCHAR syl_9;	/* more than 9-syl. Hangul noun */

	HAM_UCHAR gues2;	/* unknown guessed: secondary */
	HAM_UCHAR gues3;	/* unknown guessed for analed */

	HAM_UCHAR del_cnoun_itself;	/* remove c-noun itself(1) or not(0) */
	HAM_UCHAR add_cnoun_comp;	/* 0: no components, 1: default, 2: max. 2 cnoun candidates */
	HAM_UCHAR add_cnoun_comp_more;	/* concatenate adjacent 2 c-noun components */
	HAM_UCHAR cnoun_mark;	/* noun-delimiter char. of c-noun components */

	HAM_UCHAR stopw;	/* don't apply stopword dic. */

	HAM_UCHAR key1;	/* remove some NOUNs */
	HAM_UCHAR key2;	/* remove all NOUNs in dic. */

	HAM_UCHAR del_dup_stem;	/* remove duplicate stems(1) or not(0) */
	HAM_UCHAR ssdp;	/* include(1) OR exclude(0) Same Stem but Diff. Pos */
	HAM_UCHAR add_word_itself;	/* include(1) OR exclude(0) input string itself */
	HAM_UCHAR del_ksc_symbol;	/* remove(1:default) KSC symbols or not(0) */
	HAM_UCHAR verb_ending[5];	/* '´Ù' or 'V' is added at the end of verbs */
	HAM_UCHAR AID_ending[5];	/* a mark is added at the end of AID's */

	HAM_UCHAR query;	/* extract 1 longest stem ONLY */
} INDEX_MODE, *PINDEX_MODE;

/*
	Output style of morph. analysis result
*/
typedef struct out_style {
	HAM_UCHAR form;
		/* OUTSTYLE_NORMAL(0): (p1 "m1") + (p2 "m2") + ... */
		/* OUTSTYLE_PAREN (1): (p1 m1) + (p2 m2) + ... */ 
		/* OUTSTYLE_SLASH (2): m1/p1 + m2/p2 + ... */
	HAM_UCHAR plus;
		/* 0: '+' sign is not inserted bet'n morpheme */
		/* 1: '+' sign is inserted(default) */
	HAM_UCHAR info;
		/* 0: remove analysis information */
		/* 1: add some analysis information(default) */
	HAM_UCHAR zzz;	/* reserved for others: not used yet */
} MORPH_OUTSTYLE, *PMORPH_OUTSTYLE;

/*                                                              */
/*      Definition of the global variables for HAM              */
/*      Please, set values as you want for your application     */
/*                                                              */
typedef struct {
	HAM_UCHAR hcode_in, hcode_out;
		/* 0 or KSSM    : KSSM composite Hangul code */
		/* 1 or KSC5601 : KS C 5601-1987 Hangul code */
		/* 2 or UNICODE : UNICODE 2.0 Hangul code */
	HAM_UCHAR inputmode;	/* 0: default, 1: line-by-line input */
		/* 2: line-by-line for test version */

	HAM_UCHAR exemode;
		/* ANALYSIS: morph analysis mode(default)*/
		/* SPELLING: spelling check mode         */
		/* INDEXING: automatic indexing mode     */
		/* PARSING : simple dependency parser    */

	HAM_UCHAR mbr;
		/* 0: no morph. ambiguity resolution */
		/* 1: morph. ambiguity resolution -- default */

	HAM_UCHAR outmode;  /* print-out mode of HAM */
		/* OUTMODE_HAM  : morph. analysis result */
		/* OUTMODE_TOKEN: SNU-tagset based result*/
		/* OUTMODE_KONAN: KONAN-tag based result */
		/* OUTMODE_BOTH : HAM & SNU-tag result   */
		/* OUTMODE_ALL  : printout all candidates*/

	HAM_UCHAR outlevel; /* unknown-guessing level */
		/* UNK_NORMAL(0): tight conditioned for unknown word */
		/* BEST1_ONLY(1): printout best 1 result */
		/* BEST2_ONLY(2): printout max. best 2 results */
		/* UNK_XPND_1(3): expanded analysis for unknown word */
		/* UNK_XPND_2(4): expand unknown words: analysis O.K */

	MORPH_OUTSTYLE outstyle;

	HAM_UCHAR jomi_var; /* recover Josa/Eomi variant*/
		/* JOMI_AOI(0) : only 'a/o' & 'i' are recovered */
		/* JOMI_BAS(1) : all Josa/Eomi --> base forms   */
		/* JOMI_ORG(2) : all Josa/Eomi --> original form*/
	HAM_UCHAR xpnd_NV;  /* N/P/X/M, V/J/VX are      */
		/* expanded(1) or not(0:default) for token-based result */
	HAM_UCHAR xpnd_asc; /* Hangul+Ascii.. stems are */
		/* expanded(1) or not(0:default) for token-based result */

	HAM_UCHAR sfxlevel; /* suffix analysis level of HAM */

	HAM_UCHAR echosent;  /* echo input sentence or not */
	HAM_UCHAR echoword;  /* echo input word or not */

	HAM_SHORT err_code;  /* error code while running HAM */
	HAM_UCHAR err_word[WORDSIZE];  /* error word */

	HAM_UCHAR anorm;	/* normalize Arabic/Hangul numerals */
	HAM_UCHAR autospac;	/* apply auto. spacing(1) or not(0) */

	HAM_UCHAR delimiter[32];	/* user-defined word delimiters */
	HAM_UCHAR dicpath[DIC_SIZ];	/* dir. path of dictionaries */
	HAM_UCHAR userdic[DIC_SIZ];	/* user-defined dictionary */
	HAM_UCHAR cnndic[DIC_SIZ];	/* ham-cnn.dic */
	HAM_UCHAR stopdic[DIC_SIZ];	/* stopword.dic */

	HAM_UCHAR hf_dic;	/* High-frequency word dic. */

	char *version;	/* VERSION_NUMBER like "6.1.0" */
	INDEX_MODE index;	/* index output mode */
} HAM_RUNMODE, *HAM_PRUNMODE;
/*--------------------- end of runmode.h  ----------------------*/
