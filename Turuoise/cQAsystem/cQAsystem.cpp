#include "cQAsystem.h"


CQAsystem::CQAsystem()
	:mDbName("Turuoise.db")
{
	this->mFlstQueryResult			= nullptr;
	this->mLstQueryResult			= nullptr;
	this->mSetQueryResult			= nullptr;

	this->mFreqTrainer				= nullptr;
	this->mQueryAnalyzer			= nullptr;
}

CQAsystem::CQAsystem(String& dbName)
	: mDbName(dbName)
{
	this->mFlstQueryResult			= nullptr;
	this->mSetQueryResult			= nullptr;

	this->mFreqTrainer				= nullptr;
	this->mQueryAnalyzer			= nullptr;
}

CQAsystem::~CQAsystem()
{
	delete this->mFlstQueryResult;
	delete this->mLstQueryResult;
	delete this->mSetQueryResult;
}

bool CQAsystem::loadTraningDir(const char* training_dir)
{
	DIR *dir = opendir(training_dir);
	if(!dir)
	{
		std::cerr << "Can not open directory : [" << dir << "]" << std::endl;
		return false;
	}

	// traverse files in directory
	dirent *entry = nullptr;
	Integer prog = 0;
	while((entry = readdir(dir)) != NULL)
	{
		auto curFileName = entry->d_name;
		if(!strcmp(curFileName, ".") || !strcmp(curFileName, ".."))
			continue;

		char *path = (char*)malloc(strlen(training_dir)+strlen(curFileName)+2);
		sprintf(path, "%s/%s", training_dir, curFileName);

		mDocId2DocPath.insert(std::pair<Integer, String>(prog++, String(path)));

		free(path);
	}
	closedir(dir);

	return true;
}

void CQAsystem::beginTraning(String& srcDir, bool isTrained)
{
	if(!isTrained)
	{
		mSqliteConnector = new SqliteConnector(mDbName);
		if(mSqliteConnector->initExistsDB())
			std::cout << "Delete already existed DB." << std::endl;
		mSqliteConnector->initDB();

		std::cout << "Ready to beginTraning" << std::endl;
		mFreqTrainer = new FreqBasedTrainer(mSqliteConnector);
		mFreqTrainer->beginTraning(srcDir, mDocId2DocPath);
		std::cout << "Training complete" << std::endl << std::endl;

		delete mFreqTrainer;
		delete mSqliteConnector;
	}
	else
	{
		loadTraningDir(srcDir.c_str());
	}
	// training for query likely method
	#ifdef _NGRAM_TRAINING_
	if(!isTrained)
	{
		mSqliteConnector = new SqliteConnector(mDbName);
		if(mSqliteConnector->initExistsNgramTable())
			std::cout << "Delete already existed ngram tables." << std::endl;
		mSqliteConnector->initNgramTables();
		mNgramTrainer = new NgramTrainer(mSqliteConnector);

		std::cout << "Ready to begin ngram traning" << std::endl;
		mNgramTrainer->beginTraning(srcDir, mDocId2DocPath);
		std::cout << "Ngram training complete" << std::endl << std::endl;

		delete mNgramTrainer;
		delete mSqliteConnector;
	}
	else
	{
		loadTraningDir(srcDir.c_str());
	}
	#endif
}
void CQAsystem::analyzeQuery(String& query)
{
	mSqliteConnector = new SqliteConnector(mDbName);
	mSqliteConnector->openDB();

	std::cout << "Ready to analyzeQuery" << std::endl;
	#ifndef _QUERY_LIKELYHOOD_METHOD_
	mQueryAnalyzer = new QryAnalCosSim();
	mQueryAnalyzer->beginQueryAnalysis(query, &mSetQueryResult);
	#else
	mQueryAnalyzer = new QryNgram(mSqliteConnector);
	mQueryAnalyzer->beginQueryAnalysis(query, &mLstQueryResult);
	#endif
	std::cout << "Query analysis complete" << std::endl << std::endl;

	delete mSqliteConnector;
}
void CQAsystem::calculateScore()
{
	mSqliteConnector = new SqliteConnector(mDbName);
	mSqliteConnector->openDB();

	auto numOfDocs = mDocId2DocPath.size();

	std::cout << "Ready to calculateScore" << std::endl;

	#ifndef _QUERY_LIKELYHOOD_METHOD_
	//mScoreCalculator = new CosineSimilarity(numOfDocs, mSqliteConnector);
	//mScoreCalculator = new NaiveBeysian(numOfDocs, mSqliteConnector);
	mScoreCalculator = new OkapiBM25(0.8, 0.2, numOfDocs, mSqliteConnector);
	mScoreCalculator->beginScoring(mSetQueryResult, mScoreResult);
	#else
	mScoreCalculator = new DocLanguageModel(0.0, 1.0, numOfDocs, mSqliteConnector);
	mScoreCalculator->beginScoring(mLstQueryResult, mScoreResult);
	#endif

	std::cout << "Scoring complete" << std::endl << std::endl;

	delete mQueryAnalyzer;
	delete mScoreCalculator;
	delete mSqliteConnector;
}

const String& CQAsystem::getXmlPathFromDocID(Integer doc_id) const
{
	auto find_result = mDocId2DocPath.find(doc_id);

	return find_result->second;
}

const std::vector<DocInfo>* CQAsystem::sortResult(const Integer show_limit)
{
	std::partial_sort(mScoreResult.begin(), mScoreResult.begin()+show_limit, mScoreResult.end());
	std::vector<DocInfo> *sorted_result = new std::vector<DocInfo>();

	auto iter = mScoreResult.begin();
	for(int i=0; i<show_limit; i++)
	{
		sorted_result->push_back(*iter);
		iter++;
	}

	return sorted_result;
}

void CQAsystem::dispalyResult(const Integer show_limit)
{
	std::cout << "Ready to dispalyResult" << std::endl;
	std::partial_sort(mScoreResult.begin(), mScoreResult.begin()+show_limit, mScoreResult.end());

	for(auto i=0; i<show_limit; i++)
	{
		auto file_name = getXmlPathFromDocID(mScoreResult[i].getDocID());
		rapidxml::file<> xml_file(file_name.c_str());
		rapidxml::xml_document<> xml_doc;

		xml_doc.parse<0>(xml_file.data());

		rapidxml::xml_node<char> *data_node = xml_doc.first_node("data");

		rapidxml::xml_node<char> *question_node = data_node->first_node("question");;
		rapidxml::xml_node<char> *answer_node = data_node->first_node("answer");

		std::cout << "=== Rank " << i+1 << " (Score:" << mScoreResult[i] << ") ===" << std::endl;

		std::cout << "== Question ==" << std::endl;
		std::cout << mSqliteConnector->UTF8ToANSI(question_node->value()) << std::endl;
		std::cout << "== Answer ==" << std::endl;
		std::cout << mSqliteConnector->UTF8ToANSI(answer_node->value()) << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;
	}
	std::cout << "=== Done ===" << std::endl << std::endl;
}