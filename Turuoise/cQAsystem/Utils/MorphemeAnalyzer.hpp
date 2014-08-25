#ifndef _MORPHEME_ANALYZER_HPP_
#define _MORPHEME_ANALYZER_HPP_

#include <iostream>
#include <vector>

namespace MA {
extern "C"
{
#include "KLT/sizedefs.h"	/* size definitions */
#include "morph_header/ham-par.h"
}

class Morpheme {
	public:
		std::string mor;
		char rel;

		Morpheme( char _rel, std::string _mor) {
			rel = _rel;
			mor = _mor;
		}
};


class VECMorpheme {
	public:
		std::string word;
		unsigned char rel;
		std::vector< Morpheme> vec;

		VECMorpheme( char _rel, std::string _word) {
			rel = _rel;
			word = _word;
		}
};

class MorphemeAnalyzer
{
private:
	HAM_RUNMODE mode;   /* HAM running mode: 'header/runmode.h' */
	unsigned char sent[SENTSIZE];	/* input sentence */
	KPAR_PSENT kpr;	/* parse tree: header/parsek.h */
	HAM_SHORT vline_flag[30];

public:
	MorphemeAnalyzer();
	~MorphemeAnalyzer();
	std::vector< VECMorpheme> Extract( std::string str);
	void parsing_out( FILE *fp, KPAR_PSENT kpr, std::vector< VECMorpheme>& vec_morphemes);
	void MorphemeAnalyzer::put_tree( KPAR_PWORD node, int depth, std::vector< VECMorpheme>& vec_morphemes);
	void put_dep_tree( FILE *fp, DPAR_PSENT dpr);
	void put_dep_relation( FILE *fp, DPAR_PSENT dpr);
	void put_dep_subtree( FILE *fp, HAM_SHORT i, HAM_SHORT d, HAM_SHORT n, DPAR_PSENT dpr);
};
}

#endif