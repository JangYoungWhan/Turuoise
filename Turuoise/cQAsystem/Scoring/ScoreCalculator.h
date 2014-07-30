#ifndef _SCORE_CALCULATOR_H_
#define _SCORE_CALCULATOR_H_


#include <vector>
#include <forward_list>
#include "../Utils/StdRedef.h"
#include "../Utils/Term.h"
#include "../Utils/SqliteConnector.h"
#include "../Utils/ProgressBar.h"
#include "../Utils/DocInfo.h"


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