#include "FreqBasedTrainer.h"

FreqBasedTrainer::FreqBasedTrainer()
{

}

FreqBasedTrainer::FreqBasedTrainer(SqliteConnector* SqlConnector)
	:BaseTrainer(SqlConnector)
{
	//this->mSqlConnector = SqlConnector;
}

FreqBasedTrainer::~FreqBasedTrainer()
{
	
}


bool FreqBasedTrainer::beginTraning(String& trainingPath)
{
	std::cout << "beginTraning" << std::endl;

	// traverse all files 
	std::forward_list<String>* pFlist = new std::forward_list<String>();
	readDirectory(trainingPath.c_str(), pFlist);
	for(String& xmlFile : *pFlist)
	{
		std::cout << xmlFile << " : Training..." << std::endl;
		mXmlParser = new RapidXmlParser(xmlFile);
		mXmlParser->runParsing();	

		const std::forward_list<Term<String, Integer>>* extractWords = nullptr;

		extractWords = extractIndex2List(mSqlConnector->UTF8ToANSI(mXmlParser->getQuestionColumn()));
		if(!mSqlConnector->updateDB(extractWords, QUESTION))
			return false;

		extractWords = extractIndex2List(mSqlConnector->UTF8ToANSI(mXmlParser->getAnswerColumn()));
		if(!mSqlConnector->updateDB(extractWords, ANSWER))
			return false;

		delete mXmlParser;
	}
	delete pFlist;

	return true;
}