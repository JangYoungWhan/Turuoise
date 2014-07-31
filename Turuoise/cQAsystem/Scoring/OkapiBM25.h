#ifndef _OKAPI_BM25_H_
#define _OKAPI_BM25_H_


#include "ScoreCalculator.h"


class OkapiBM25 : public ScoreCalculator
{
private:
	const Real BM25_k1;
	const Real BM25_b;

	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInQuestion;
	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInAnswer;

private:
	void calculateQuestionScore();
	void calculateAnswerScore();

	Real calc_idf(Real df);
	Real calc_tf(String q, Integer d, Integer f);
	Real calc_df(String q, Integer f);
	Integer calc_dl(Integer d, Integer f);
	Integer calc_avgdl(Integer f);

public:
	OkapiBM25();
	OkapiBM25(Integer numOfDoc, SqliteConnector* SqlConnector);
	virtual ~OkapiBM25();

public:
	virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result);
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result);
};


#endif