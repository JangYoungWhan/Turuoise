#ifndef _NAIVE_BEYSIAN_H_
#define _NAIVE_BEYSIAN_H_


#include "../Scoring/ScoreCalculator.h"


class NaiveBeysian : public ScoreCalculator
{
private:
	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInQuestion;
	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInAnswer;

private:
	void calculateQuestionScore();
	void calculateAnswerScore();
	Integer getSumOfDocFreq(const std::map<String, FreqScore<Integer, Integer>>	*doc_info) const;
	Real prob_w_d(Integer w_freq, Integer d_freq);
	Real applyLaplaceSmoothing(Real real);

public:
	NaiveBeysian();
	NaiveBeysian(Integer numOfDoc, SqliteConnector* SqlConnector);
	virtual ~NaiveBeysian();

public:
	//virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result);
	//virtual void beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result);
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, double synonym = 0.0, double levenshtein = 0.0);
};


#endif