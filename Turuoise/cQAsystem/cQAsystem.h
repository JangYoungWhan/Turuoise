#ifndef _CQA_SYSTEM_H_
#define _CQA_SYSTEM_H_


#include <forward_list>
#include <algorithm>
#include "QAsystem.h"
#include "Utils/SqliteConnector.h"
#include "MachineLearning/BaseTrainer.h"
#include "MachineLearning/FreqBasedTrainer.h"
#include "QueryAnalysis/QueryAnalyzer.h"
#include "QueryAnalysis/QryAnalCosSim.h"
#include "Scoring/ScoreCalculator.h"
#include "Scoring/CosineSimilarity.h"
#include "Scoring/NaiveBeysian.h"
#include "Utils/DocInfo.h"

#ifdef _WIN32
#include "Utils/winDirent.h"
#else
#include <dirent.h>
#endif


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
	bool loadTraningDir(const char* training_dir);

public:
	CQAsystem();
	CQAsystem(String& dbName);
	virtual ~CQAsystem();	

public:
	virtual void beginTraning(String& srcDir, bool isTrained);
	virtual void analyzeQuery(String& query);
	virtual void calculateScore();
	virtual void dispalyResult(const Integer show_limit);
};


#endif