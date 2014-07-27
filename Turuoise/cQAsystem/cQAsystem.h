#ifndef _CQA_SYSTEM_H_
#define _CQA_SYSTEM_H_


#include <forward_list>
#include <algorithm>
#include "QAsystem.h"
#include "SqliteConnector.h"
#include "BaseTrainer.h"
#include "FreqBasedTrainer.h"
#include "QueryAnalyzer.h"
#include "QryAnalCosSim.h"
#include "ScoreCalculator.h"
#include "CosineSimilarity.h"
#include "NaiveBeysian.h"
#include "DocInfo.h"


class CQAsystem : public QAsystem
{
private:
	String mDbName;

private:
	std::map<Integer, String>					mDocId2DocPath;
	std::forward_list<Term<String, Integer>>	*mFlstQueryResult;
	std::set<Term<String, Integer>>				*mSetQueryResult;
	std::vector<DocInfo>						mScoreResult; // It needs partial sorting.

private:
	SqliteConnector*	mSqliteConnector;
	BaseTrainer*		mTrainer;
	QueryAnalyzer*		mQueryAnalyzer;
	ScoreCalculator*	mScoreCalculator;
	void*				mOutputPrinter; // it does not defined yet

private:
	const String& getXmlPathFromDocID(Integer doc_id) const;

public:
	CQAsystem();
	CQAsystem(String& dbName);
	virtual ~CQAsystem();

public:
	virtual void beginTraning(String& srcDir);
	virtual void analyzeQuery(String& query);
	virtual void calculateScore();
	virtual void dispalyResult(const Integer show_limit);

};


#endif