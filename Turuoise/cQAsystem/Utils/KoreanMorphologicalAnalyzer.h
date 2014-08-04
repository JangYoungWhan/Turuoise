#ifndef _KOREAN_MORPHOLOGICAL_ANALYZER_H_
#define _KOREAN_MORPHOLOGICAL_ANALYZER_H_


#include <iostream>
#include <forward_list>
#include <map>
#include <set>
#include "Term.h"
#include "FreqScore.h"

extern "C"
{
#include "KLT/ham-ndx.h"
}

template <typename T_str, typename T_int>
class KoreanMorphologicalAnalyzer
{
private:
	HAM_TERMLIST mTerm[MAX_TERMS_DOC];	/* extracted terms */
	HAM_TERMMEMORY mTM;					/* termtext.h: memories needed for term extraction */
	HAM_RUNMODE mMode;					/* HAM running mode: 'KLT/runmode.h' */

protected:
	std::forward_list<Term<T_str, T_int>>* extractIndex2Flist(char* text);
	std::set<Term<T_str, T_int>>* extractIndex2Set(char* text);
	std::map<T_str, FreqScore<T_int, T_int>>* extractIndex2Map(char* text);
public:
	KoreanMorphologicalAnalyzer();
	virtual ~KoreanMorphologicalAnalyzer();

protected: /*============================= UTILS =============================*/
	unsigned char *load_text(FILE *fp) const;
};

template <typename T_str, typename T_int>
std::forward_list<Term<T_str, T_int>>* KoreanMorphologicalAnalyzer<T_str, T_int>::extractIndex2Flist(char* text)
{
	std::forward_list<Term<T_str, T_int>>* pFlistExtractResult = new std::forward_list<Term<T_str, T_int>>();
	int n;

	/*------------------------------------------------------------------------
	- 다섯번째 인자 -- 0/n/-n(최대로 추출되는 용어수)
	n: 최대 추출 용어수, -n: 빈도 n이상(or n 퍼센트), 0: 모든 용어 추출
	[참고] n개 선택 기준: sortflag(소팅 방식)에 따라 '가중치' or '소팅 옵션'
	- sortflag가 양수 1/2/3/4 -- 가중치 순으로 n개 선택
	- sortflag가 음수 -1/-2/-3/-4 -- 소팅 방식에 따라 소팅 후 n개 선택
	[참고] n <= -10 이면, -n 퍼센트만큼 용어수를 출력함.

	- 여섯번째 인자 -- 0/1/2/3/4(소팅 방식)
	0: 소팅 안함, 1: '가나다'순, 2: 빈도순, 3: 가중치, 4: 어절위치
	[참고] n개 선택 기준을 '소팅 방식'에 따라 선택하려면 음수로 줌

	- 마지막 인자 -- 0/1(어절위치 numbering 방식)
	0: 어절순서로, 1: 문장별로 100부터 시작
	========================================================================*/
	n = get_terms_text(reinterpret_cast<unsigned char*>(text), mTerm, &mTM, &mMode, 0, 2, 1);

	auto iter = pFlistExtractResult->before_begin();
	for(int i=0; i<n; i++)
	{
		Term<T_str, T_int> term;

		std::string currentTerm((const char*)mTM.memTermString+mTerm[i].offset);

		term.setTerm(currentTerm);
		term.setTermFreq(static_cast<T_int>(mTerm[i].tf));
		term.setScore(mTerm[i].score);

		iter = pFlistExtractResult->insert_after(iter, term);
	}

	return pFlistExtractResult;
}

template <typename T_str, typename T_int>
std::set<Term<T_str, T_int>>* KoreanMorphologicalAnalyzer<T_str, T_int>::extractIndex2Set(char* text)
{
	std::set<Term<T_str, T_int>>* pSetExtractResult = new std::set<Term<T_str, T_int>>();
	int n;
	n = get_terms_text(reinterpret_cast<unsigned char*>(text), mTerm, &mTM, &mMode, 0, 2, 1);

	for(int i=0; i<n; i++)
	{
		Term<T_str, T_int> term;

		std::string currentTerm((const char*)mTM.memTermString+mTerm[i].offset);

		term.setTerm(currentTerm);
		term.setTermFreq(static_cast<T_int>(mTerm[i].tf));
		term.setScore(mTerm[i].score);

		pSetExtractResult->insert(term);
	}

	return pSetExtractResult;
}

template <typename T_str, typename T_int>
std::map<T_str, FreqScore<T_int, T_int>>* KoreanMorphologicalAnalyzer<T_str, T_int>::extractIndex2Map(char* text)
{
	std::map<T_str, FreqScore<T_int, T_int>>* pMapExtractResult = new std::map<T_str, FreqScore<T_int, T_int>>();
	int n;
	n = get_terms_text(reinterpret_cast<unsigned char*>(text), mTerm, &mTM, &mMode, 0, 2, 1);

	for(int i=0; i<n; i++)
	{
		

		std::string currentTerm((const char*)mTM.memTermString+mTerm[i].offset);
		FreqScore<T_int, T_int> freq_score(term.setTermFreq(static_cast<T_int>(mTerm[i].tf)), term.setScore(mTerm[i].score));
		std::pair<T_str, FreqScore<T_int, T_int>> term_pair(currentTerm, freq_score);

		pMapExtractResult->insert(term_pair);
	}

	return pMapExtractResult;
}

template <typename T_str, typename T_int>
KoreanMorphologicalAnalyzer<T_str, T_int>::KoreanMorphologicalAnalyzer()
{
	if (!open_HAM_index(&mMode, NULL, "hdic/KLT2000.ini"))
		std::cerr << "Cannot load dictionary files" << std::endl;
}

template <typename T_str, typename T_int>
KoreanMorphologicalAnalyzer<T_str, T_int>::~KoreanMorphologicalAnalyzer()
{
	close_HAM_index(&mMode);	/* HAM is not used any more */
}

template <typename T_str, typename T_int>
unsigned char* KoreanMorphologicalAnalyzer<T_str, T_int>::load_text(FILE *fp) const
{
	long n;
	unsigned char *p;

	fseek(fp, 0L, 2);
	n = ftell(fp);	/* n: byte size of file 'fp' */

	fseek(fp, 0L, 0);
	p = (unsigned char *) malloc(n+1);	/* memory allocation */
	if (p == NULL) return NULL;

	fread(p, sizeof(unsigned char), n, fp);	/* read 'fp' to 'p' */
	*(p+n) = '\0';

	return p;
}


#endif