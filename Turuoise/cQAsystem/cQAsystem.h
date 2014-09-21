#ifndef _CQA_SYSTEM_H_
#define _CQA_SYSTEM_H_


#include <forward_list>
#include <algorithm>
#include "QAsystem.h"
#include "Utils/SqliteConnector.h"
#include "MachineLearning/BaseTrainer.h"
#include "MachineLearning/FreqBasedTrainer.h"
#include "MachineLearning/NgramTrainer.h"
#include "QueryAnalysis/QueryAnalyzer.h"
#include "QueryAnalysis/QryAnalCosSim.h"
#include "QueryAnalysis/QryNgram.h"
#include "Scoring/ScoreCalculator.h"
#include "Scoring/CosineSimilarity.h"
#include "Scoring/NaiveBeysian.h"
#include "Scoring/OkapiBM25.h"
#include "Scoring/DocLanguageModel.h"
#include "Utils/DocInfo.h"


#ifdef _WIN32
#include "Utils/winDirent.h"
#else
#include <dirent.h>
#endif

#define _NGRAM_TRAINING_
#define _QUERY_LIKELYHOOD_METHOD_

class CQAsystem : public QAsystem
{
private:
	Integer mNumOfDocs;
	String mDbName;

private:
	std::map<Integer, String>					mDocId2DocPath;
	std::forward_list<Term<String, Integer>>	*mFlstQueryResult;
	std::list<Integer>							*mLstQueryResult;
	std::set<Term<String, Integer>>				*mSetQueryResult;
	std::vector<DocInfo>						mScoreResult;

private:
	SqliteConnector*	mSqliteConnector;
	BaseTrainer*		mFreqTrainer;
	BaseTrainer*		mNgramTrainer;
	QueryAnalyzer*		mQueryAnalyzer;
	ScoreCalculator*	mScoreCalculator;

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
	virtual void calculateScore( const int scoring_func, const double question_weight, const double answer_weight, const double libenstein_weight, const double synonym_weight);
	virtual const std::vector<DocInfo>* sortResult(const Integer show_limit);
	virtual void displayResult(const Integer show_limit);
	virtual void writeResult(const String &query, const Integer show_limit);
};


#endif