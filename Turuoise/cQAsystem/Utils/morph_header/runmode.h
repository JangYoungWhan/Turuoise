/*
	File name: runmode.h
	Description: HAM options for various application
	Created by: Kang, Seung-Shik		07/20/1997
*/

/*
	Output options for parsing result.
*/
typedef struct parse_mode {
	HAM_UCHAR nnn_to_1;	/* make noun+noun+... nodes to 1 node */

	HAM_UCHAR put_tree;	/* print-out parse tree(1: default) or not(0) */
	HAM_UCHAR put_fail;	/* print parse-failed subtree(1) or not(0: default) */
	HAM_UCHAR put_morp;	/* 1: morp. result at node, 2: mom-sis, 3: Eng-stem */

	HAM_UCHAR put_depn;	/* 1: put dep. tree, 2: tree & relation */
	HAM_UCHAR put_mor2;	/* 0: default, 1: put morp. result before parsing */
	HAM_UCHAR put_node;	/* 0: default, 1: put all parse nodes by 'word-id' */

	HAM_UCHAR rand_inp;	/* 0: default, 1: select random sentence */
} PARSE_MODE, *PPARSE_MODE;

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

	HAM_UCHAR delimiter[30];	/* user-defined word delimiters */
	HAM_UCHAR dicpath[99];	/* dir. path of dictionaries */
	HAM_UCHAR userdic[20];	/* user-defined dictionary */

	HAM_PUCHAR version;	/* HAM version like "HAM/INDEX version 5.0.0" */
	PARSE_MODE parse;	/* parse output mode */
} HAM_RUNMODE, *HAM_PRUNMODE;
/*--------------------- end of runmode.h  ----------------------*/
