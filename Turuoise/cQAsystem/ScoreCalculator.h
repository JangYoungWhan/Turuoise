#ifndef _SCORE_CALCULATOR_H_
#define _SCORE_CALCULATOR_H_


#include "SqliteConnector.h"
#include "ProgressBar.h"


class ScoreCalculator
{
protected:
	SqliteConnector*		mSqlConnector;
	ProgressBar<Integer>*	mProgressBar;

public:
	ScoreCalculator() { }
	ScoreCalculator(SqliteConnector* SqlConnector)
	{
		this->mProgressBar	= new ProgressBar<Integer>();
		this->mSqlConnector = SqlConnector;
	}

	virtual ~ScoreCalculator()
	{
		delete this->mProgressBar;
	}

public:
	virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result) = 0;
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result) = 0;
};


#endif