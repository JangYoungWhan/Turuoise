#ifndef _NAIVE_BEYSIAN_H_
#define _NAIVE_BEYSIAN_H_


#include <iostream>
#include <forward_list>
#include <set>
#include <map>
#include "StdRedef.h"
#include "Term.h"
#include "FreqScore.h"
#include "SqliteConnector.h"
#include "ScoreCalculator.h"


class NaiveBeysian : public ScoreCalculator
{
private:
	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInQuestion;
	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInAnswer;

private:
	void calculateQuestionScore();
	void calculateAnswerScore();
	Integer getSumOfDocFreq(const std::map<String, FreqScore<Integer, Integer>>	*doc_info) const;
	Real NaiveBeysian::prob_w_d(Integer w_freq, Integer d_freq);
	Real NaiveBeysian::applyLaplaceSmoothing(Real real);

public:
	NaiveBeysian();
	NaiveBeysian(Integer numOfDoc, SqliteConnector* SqlConnector);
	virtual ~NaiveBeysian();

public:
	virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result);
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result);
};


#endif