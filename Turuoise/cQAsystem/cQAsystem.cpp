#include "cQAsystem.h"


CQAsystem::CQAsystem()
	:mDbName("Turuoise.db")
{
	this->mFlstQueryResult			= nullptr;
	this->mSetQueryResult			= nullptr;
	this->mScoreResult				= new std::forward_list<DocInfo>();

	this->mTrainer					= nullptr;
	this->mQueryAnalyzer			= nullptr;
}

CQAsystem::CQAsystem(String& dbName)
	: mDbName(dbName)
{
	this->mFlstQueryResult			= nullptr;
	this->mSetQueryResult			= nullptr;
	this->mScoreResult				= new std::forward_list<DocInfo>();

	this->mTrainer					= nullptr;
	this->mQueryAnalyzer			= nullptr;
}

CQAsystem::~CQAsystem()
{
	delete this->mFlstQueryResult;
	delete this->mSetQueryResult;
	delete this->mScoreResult;
}

void CQAsystem::beginTraning(String& srcDir)
{
	mSqliteConnector = new SqliteConnector(mDbName);
	mSqliteConnector->initDB();

	std::cout << "Ready to beginTraning" << std::endl;
	mTrainer = new FreqBasedTrainer(mSqliteConnector);
	mTrainer->beginTraning(srcDir);
	std::cout << "Training complete" << std::endl << std::endl;

	delete mTrainer;
	delete mSqliteConnector;
}
void CQAsystem::analyzeQuery(String& query)
{
	mSqliteConnector = new SqliteConnector(mDbName);
	mSqliteConnector->openDB();

	std::cout << "Ready to analyzeQuery" << std::endl;
	mQueryAnalyzer = new QryAnalCosSim();
	mQueryAnalyzer->beginQueryAnalysis(query, &mSetQueryResult);
	std::cout << "Query analysis complete" << std::endl << std::endl;

	delete mSqliteConnector;
}
void CQAsystem::calculateScore()
{
	mSqliteConnector = new SqliteConnector(mDbName);
	mSqliteConnector->openDB();

	std::cout << "Ready to calculateScore" << std::endl;
	//mScoreCalculator = new CosineSimilarity(mSqliteConnector);
	mScoreCalculator = new NaiveBeysian(mSqliteConnector);
	mScoreCalculator->beginScoring(mSetQueryResult);
	std::cout << "Scoring complete" << std::endl << std::endl;

	delete mQueryAnalyzer;
	delete mScoreCalculator;
	delete mSqliteConnector;
}
void CQAsystem::dispalyResult()
{
	std::cout << "Ready to dispalyResult" << std::endl;
	std::cout << "=== Done ===" << std::endl << std::endl;
}