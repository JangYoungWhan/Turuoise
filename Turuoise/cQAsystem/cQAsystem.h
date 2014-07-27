#ifndef _CQA_SYSTEM_H_
#define _CQA_SYSTEM_H_


#include <forward_list>
#include "QAsystem.h"
#include "SqliteConnector.h"
#include "BaseTrainer.h"
#include "FreqBasedTrainer.h"
#include "QueryAnalyzer.h"
#include "QryAnalCosSim.h"
#include "ScoreCalculator.h"
#include "CosineSimilarity.h"
#include "NaiveBeysian.h"


struct DocInfo
{
	Integer docID; // Q-A paired XML file name. It have to be integer value.
	Real	score; // Score of the file satisfied that query.
};


class CQAsystem : public QAsystem
{
private:
	String mDbName;

private:
	std::forward_list<Term<String, Integer>>	*mQueryResult;
	std::forward_list<DocInfo>					*mScoreResult; // It needs partial sorting.

private:
	SqliteConnector*	mSqliteConnector;
	BaseTrainer*		mTrainer;
	QueryAnalyzer*		mQueryAnalyzer;
	ScoreCalculator*	mScoreCalculator;
	void*				mOutputPrinter; // it does not defined yet

public:
	CQAsystem();
	CQAsystem(String& dbName);
	virtual ~CQAsystem();

public:
	virtual void beginTraning(String& srcDir);
	virtual void analyzeQuery(String& query);
	virtual void calculateScore();
	virtual void dispalyResult();

};


#endif