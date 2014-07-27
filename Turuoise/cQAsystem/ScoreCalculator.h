#ifndef _SCORE_CALCULATOR_H_
#define _SCORE_CALCULATOR_H_


#include <vector>
#include <forward_list>
#include "StdRedef.h"
#include "Term.h"
#include "SqliteConnector.h"
#include "ProgressBar.h"
#include "DocInfo.h"


class ScoreCalculator
{
protected:
	Integer					mNumOfDocs;
	SqliteConnector*		mSqlConnector;
	ProgressBar<Integer>*	mProgressBar;

public:
	ScoreCalculator()
		:mNumOfDocs(0)
	{ }
	ScoreCalculator(Integer numOfDocs, SqliteConnector* SqlConnector)
		:mNumOfDocs(numOfDocs)
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
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result) = 0;
};


#endif