#ifndef _OKAPI_BM25_H_
#define _OKAPI_BM25_H_


#include "ScoreCalculator.h"


class OkapiBM25 : public ScoreCalculator
{
private:
	const Real BM25_k1;
	const Real BM25_b;

	std::vector<Term<String, Integer>>	mVectorDocInfoInQuestion;
	std::vector<Term<String, Integer>>	mVectorDocInfoInAnswer;

	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInQuestion;
	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInAnswer;

private:
	void calculateQuestionScore();
	void calculateAnswerScore();

	Real cal_idf(Real df) const;
	Real cal_tf(String q, Integer d, int flag) const;
	Real cal_df(String q, int flag) const;
	Integer cal_dl(Integer d, int flag) const;
	Real cal_avgdl(int flag) const;
	Real calBM25(Real tf, Real idf, Integer dl, Real avgdl) const;

public:
	OkapiBM25();
	OkapiBM25(Integer numOfDoc, SqliteConnector* SqlConnector);
	OkapiBM25(Real question_ratio, Real answer_ratio, Integer numOfDoc, SqliteConnector* SqlConnector);
	virtual ~OkapiBM25();

public:
	//virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result);
	//virtual void beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result);
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, double synonym = 0.0, double levenshtein = 0.0);
};


#endif