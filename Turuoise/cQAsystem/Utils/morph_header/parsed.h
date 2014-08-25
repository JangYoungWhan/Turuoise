/*
	File name: parsed.h
	Description: data structure for dependency parsing.
	Written by: Kang, Seung-Shik	1997. 11. 14.
*/
#include "dep-rel.h"	/* govnor-dependent relationship */

#define MAX_GOV	10	/* max. govnors of a node */

typedef struct predlist {
	HAM_SHORT vtp;	/* predicate type */
	HAM_SHORT vid;	/* predicate id.  */
	HAM_SHORT sbj;	/* subject of pred*/
	HAM_SHORT obj;	/* object of pred.*/
} DPAR_PREDLIST, *DPAR_PPREDLIST;

typedef struct nounlist {
	HAM_SHORT ntp;	/* noun node type */
	HAM_SHORT nid;	/* noun node id.  */
	HAM_SHORT pid;	/* predi. of noun */
} DPAR_NOUNLIST, *DPAR_PNOUNLIST;

typedef struct dparword {
	HAM_SHORT ngov;	/* number of governors */
	HAM_SHORT gov[MAX_GOV];	/* parent node: index of governor */
	HAM_SHORT score[MAX_GOV];	/* score[i] --> gov[i]의 가능성 */

	HAM_SHORT nch;	/* number of children */
	HAM_SHORT nod;	/* number of all children */
		/* num. of children & grand children */

	HAM_SHORT first;	/* link to the first child */
	HAM_SHORT last;	/* link to the last child */

	HAM_SHORT left;	/* link to left sibling */
	HAM_SHORT right;	/* link to right sibling */

	HAM_UCHAR rel;	/* relation type */
	HAM_UCHAR fixed;	/* relation is fixed? */
	HAM_SHORT depth;	/* depth of parse tree */

	HAM_SHORT govType;	/* 지배소 유형 */
		/* 복합명사의 non-last noun, 의존명사, 보조용언 */

	HAM_PWORD pw;	/* morph. analysis results */
	HAM_PRESULT pr;	/* the best result */
} DPAR_WORD, *DPAR_PWORD;

#define MAXPREDS	30
#define MAXNOUNS	30

/*                                                      */
/*       Korean dependency parser: data structure       */
/*                                                      */
typedef struct dparsent {
	HAM_PMORES pmr;	/* ptr to morph analysis result */

/*------------- dependency parsing results -------------*/
	HAM_SHORT root;	/* down link to 1st child of root */
	HAM_SHORT nch0;	/* number of children for root */

	HAM_SHORT nword;	/* num. of tokens in sentence */
//	HAM_PWORD token[MAXWORDS];	/* morph-analed stems */
	struct dparword node[MAXWORDS];	/* parsing result */
/*------------------------------------------------------*/

	/*    'nounlist', 'predlist' -- NOT USED YET     */
	HAM_SHORT nnp;	/* num. of nouns -- NOT USED YET */
	struct nounlist nps[MAXNOUNS];	/* nouns in sent */

	HAM_SHORT nvj;	/* num. of preds -- NOT USED YET */
	struct predlist vjs[MAXPREDS];	/* pred. in sent */
} DPAR_SENT, *DPAR_PSENT;
/*----------------- end of parsed.h -------------------*/
