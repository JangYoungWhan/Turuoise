#include "MorphemeAnalyzer.hpp"

using namespace MA;

MorphemeAnalyzer::MorphemeAnalyzer()
{
	/*=========================== Initialization ===========================*/
	if (open_HAM_parse(&mode, NULL, "Utils\\hdic\\ham2000.ini")) {
		fprintf(stderr, "Error: dictionary-file path <%s>!", mode.dicpath);
		fprintf(stderr, " See <header/err-code.h> for code %d\n", mode.err_code);
	}
}

MorphemeAnalyzer::~MorphemeAnalyzer()
{
	if (mode.autospac)
		free_bigram_data();	/* 자동 띄어쓰기 */
	close_HAM_parse();	/* HAM is not used any more */
}


std::vector< VECMorpheme> MorphemeAnalyzer::Extract( std::string str)
{
	if (mode.autospac) load_bigram_data("hdic/bigram.dic");	/* 자동 띄어쓰기 */
	
	strcpy_s( (char*)sent, str.length() + 1, str.c_str());
	if (mode.autospac) adjust_word_spacing( sent, 1);	/* 자동 띄어쓰기 */
	kpr = parsing( sent, &mode);

	std::vector< VECMorpheme> vec_morphemes;
	parsing_out( stdout, kpr, vec_morphemes);	/* head-daughter parse tree */
	std::reverse( vec_morphemes.begin(), vec_morphemes.end());
	return vec_morphemes;
}


void MorphemeAnalyzer::parsing_out( FILE *fp, KPAR_PSENT kpr, std::vector< VECMorpheme>& vec_morphemes)
{

	/* output test for dependency relation */
	/*
	if (mode.parse.put_depn == 1)
		put_dep_relation(fp, kpr->dpr);
	if (mode.parse.put_depn == 2)
		put_dep_tree(fp, kpr->dpr);
*/
	if (mode.parse.put_tree) {
		if (mode.echosent)	/* input sentence */
	//		fprintf(fp, "INPUT: %s\n", kpr->sent);
		put_tree( kpr->root, 0, vec_morphemes);
	}
}



/*
	Recursive print-out head-daughter parsing output.
*/
void MorphemeAnalyzer::put_tree( KPAR_PWORD node, int depth, std::vector< VECMorpheme>& vec_morphemes)	
{
	int i, j;
	int opt = mode.parse.put_morp;
	KPAR_PWORD p;

	if (node == NULL) return;

	//for (j = 0; j < depth; j++) printf( "    ");	/* indentation */
	//printf( "%c %s", node->rel, node->word);

	VECMorpheme vecmorpheme( (char)node->rel, (char*)node->word);

	//printf( " %c:%s", node->bpos, node->base);
	vecmorpheme.vec.push_back( Morpheme( (char)node->bpos, (char*)node->base));
	if (node->pomi != NULL) {
		//printf( " P:%s", node->pomi);
		vecmorpheme.vec.push_back( Morpheme( 'P', (char*)node->pomi));
	}
	if (node->eomi != NULL) {
		//printf( " E:%s", node->eomi);
		vecmorpheme.vec.push_back( Morpheme( 'E', (char*)node->eomi));
	}
	if (node->josa != NULL) {
		//printf( " J:%s", node->josa);
		vecmorpheme.vec.push_back( Morpheme( 'J', (char*)node->josa));
	}

	//printf( "\n");
	vec_morphemes.push_back( vecmorpheme);

	if (node->nchild) {	/* daughters */
		p = node->last;
		for (i = node->nchild-1; i >= 0; i--) {
			put_tree( p, depth+1, vec_morphemes);
			p = p->left;
		}
	}
}

/*
void MorphemeAnalyzer::put_dep_tree( FILE *fp, DPAR_PSENT dpr)
{
	HAM_SHORT i;

	for (i = 0; i < 30; i++) vline_flag[i] = 0;

	put_dep_subtree(fp, dpr->root, 0, dpr->nch0, dpr);
	fprintf(fp, "\n");
}

void MorphemeAnalyzer::put_dep_relation( FILE *fp, DPAR_PSENT dpr)
{
	HAM_SHORT i;
	DPAR_PWORD node;

	fprintf(fp, "WID  WORD       GOV <NCHILD FST LST> <LEFT RIGHT> DEPTH\n");
	for (i = 0; i < dpr->nword; i++) {
		node = dpr->node + i;
		fprintf(fp, "[%2d] %-10s ", i, node->pw->phon);
		fprintf(fp, " %2d <%2d(%2d)  %2d %2d > ", node->gov[0],
			node->nch, node->nod, node->first, node->last);
		fprintf(fp, "  <%2d %2d> %6d\n", node->left, node->right, node->depth);
		
	}
}


void MorphemeAnalyzer::put_dep_subtree( FILE *fp, HAM_SHORT i, HAM_SHORT d, HAM_SHORT n, DPAR_PSENT dpr)
{
	HAM_SHORT j;
	DPAR_PWORD node = dpr->node + i;

	if (i < 0) return;

	for (j = 0; j < d; j++) {	
		if (vline_flag[j]) fprintf(fp, "|   ");
		else fprintf(fp, "    ");
	}

	fprintf(fp, "+-- %s[%d]%c", node->pw->phon, i, node->rel);


		fprintf(fp, "\n");

	if (node->last != -1) {
		if (n > 1) vline_flag[d] = 1;	
		put_dep_subtree(fp, node->last, d+1, node->nch, dpr);
		vline_flag[d] = 0;
	}

	for (j = 0; j < n-1; j++) {	
		i = dpr->node[i].left;
		if (n > 1 && j < n-2) vline_flag[d] = 1;	
		put_dep_subtree(fp, i, d, 1, dpr);
		vline_flag[d] = 0;
	}
}
*/