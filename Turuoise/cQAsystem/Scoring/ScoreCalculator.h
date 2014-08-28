#ifndef _SCORE_CALCULATOR_H_
#define _SCORE_CALCULATOR_H_


#include <vector>
#include <forward_list>
#include <list>
#include "../Utils/StdRedef.h"
#include "../Utils/Term.h"
#include "../Utils/SqliteConnector.h"
#include "../Utils/ProgressBar.h"
#include "../Utils/DocInfo.h"


class ScoreCalculator
{
protected:
	Real QUESTION_RATIO;
	Real ANSWER_RATIO;

protected:
	Integer					mNumOfDocs;
	SqliteConnector*		mSqlConnector;
	ProgressBar<Integer>*	mProgressBar;

public:
	ScoreCalculator()
		:QUESTION_RATIO(0.5), ANSWER_RATIO(0.5), mNumOfDocs(0)
	{ }
	ScoreCalculator(Integer numOfDocs, SqliteConnector* SqlConnector)
		:QUESTION_RATIO(0.5), ANSWER_RATIO(0.5), mNumOfDocs(numOfDocs)
	{
		this->mProgressBar	= new ProgressBar<Integer>();
		this->mSqlConnector = SqlConnector;
	}
	ScoreCalculator(Real question_ratio, Real answer_ratio, Integer numOfDocs, SqliteConnector* SqlConnector)
		:QUESTION_RATIO(question_ratio), ANSWER_RATIO(answer_ratio), mNumOfDocs(numOfDocs)
	{
		this->mProgressBar	= new ProgressBar<Integer>();
		this->mSqlConnector = SqlConnector;
	}

	virtual ~ScoreCalculator()
	{
		delete this->mProgressBar;
	}

public:
	//virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result, double synonym = 0.0, double levenshtein = 0.0) = 0;
	//virtual void beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result, double synonym = 0.0, double levenshtein = 0.0) = 0;
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, double synonym = 0.0, double levenshtein = 0.0) = 0;
};


#endif