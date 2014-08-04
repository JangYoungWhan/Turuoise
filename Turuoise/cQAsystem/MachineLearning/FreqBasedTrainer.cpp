#include "FreqBasedTrainer.h"

FreqBasedTrainer::FreqBasedTrainer()
{

}

FreqBasedTrainer::FreqBasedTrainer(SqliteConnector* SqlConnector)
	:BaseTrainer(SqlConnector)
{

}

FreqBasedTrainer::~FreqBasedTrainer()
{
	
}


bool FreqBasedTrainer::beginTraning(String& trainingPath, std::map<Integer, String>& docID2Path)
{
	std::cout << "beginTraning" << std::endl;

	// traverse all files 
	std::forward_list<String>* pFlist = new std::forward_list<String>();
	readDirectory(trainingPath.c_str(), pFlist);

	auto cur_progress = 0;
	auto numOfFiles = std::distance(pFlist->begin(), pFlist->end())-1;
	for(String& xmlFile : *pFlist)
	{
		std::pair<Integer, String> int2str_pair(cur_progress, xmlFile);
		docID2Path.insert(int2str_pair);

		mProgressBar->dispalyPrgressBar(cur_progress++, numOfFiles);
		mXmlParser = new RapidXmlParser(xmlFile);
		mXmlParser->runParsing();

		if(!mSqlConnector->updateDB( xmlFile))
			return false;

		const std::set<Term<String, Integer>>* extractWords = nullptr;

		extractWords = extractIndex2Set(mSqlConnector->UTF8ToANSI(mXmlParser->getQuestionColumn()));
		if(!mSqlConnector->updateDB(extractWords, strlen( mSqlConnector->UTF8ToANSI(mXmlParser->getQuestionColumn())), QUESTION))
			return false;

		extractWords = extractIndex2Set(mSqlConnector->UTF8ToANSI(mXmlParser->getAnswerColumn()));
		if(!mSqlConnector->updateDB(extractWords, strlen( mSqlConnector->UTF8ToANSI(mXmlParser->getAnswerColumn())), ANSWER))
			return false;

		delete extractWords;
		delete mXmlParser;
	}
	delete pFlist;

	mSqlConnector->delete_m1_DB( QUESTION);
	mSqlConnector->delete_m1_DB( ANSWER);
	std::cout<<std::endl;

	return true;
}