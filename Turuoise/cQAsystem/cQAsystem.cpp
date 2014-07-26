#include "cQAsystem.h"


CQAsystem::CQAsystem()
{
	mTrainer			= nullptr;
	mQueryAnalyzer		= nullptr;
	mOutputPrinter		= nullptr;
}

CQAsystem::~CQAsystem()
{
	delete mQueryAnalyzer;
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
	mQueryAnalyzer->beginQueryAnalysis(query);
	std::cout << "Query analysis complete" << std::endl << std::endl;


	delete mSqliteConnector;
}
void CQAsystem::calculateScore()
{
	mSqliteConnector = new SqliteConnector(mDbName);
	mSqliteConnector->openDB();

	std::cout << "Ready to calculateScore" << std::endl;
	mScoreCalculator = new CosineSimilarity();
	mScoreCalculator->beginScoring();
	std::cout << "Scoring complete" << std::endl << std::endl;

	delete mSqliteConnector;
}
void CQAsystem::dispalyDocsDescendingOrder()
{
	std::cout << "Ready to dispalyDocsDescendingOrder" << std::endl;
	std::cout << "=== Done ===" << std::endl << std::endl;
}