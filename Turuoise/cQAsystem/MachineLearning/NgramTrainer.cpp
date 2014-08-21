#include "NgramTrainer.h"

NgramTrainer::NgramTrainer()
	:mNgramN(2)
{
	this->mStr2Int.insert(std::make_pair(SENTENCE_TAG, 0));
}
NgramTrainer::NgramTrainer(SqliteConnector* SqlConnector)
	:BaseTrainer(SqlConnector), mNgramN(2)
{
	this->mStr2Int.insert(std::make_pair(SENTENCE_TAG, 0));
}
NgramTrainer::~NgramTrainer()
{ }

bool NgramTrainer::beginTraning(String& trainingPath, std::map<Integer, String>& docID2Path)
{
	std::cout << "NgramTrainer::beginTraning" << std::endl;

	// traverse all files 
	std::list<String>* pFlist = new std::list<String>();
	readDirectory(trainingPath.c_str(), pFlist);

	std::cout << "XMLÆÄ½ÌÁß!" << std::endl;
	auto cur_progress = 0;
	auto numOfFiles = std::distance(pFlist->begin(), pFlist->end())-1;

	//clock_t start_time, end_time;
	for(String& xmlFile : *pFlist)
	{
		//start_time = clock();
		std::pair<Integer, String> int2str_pair(cur_progress, xmlFile);

		mProgressBar->dispalyPrgressBar(cur_progress, numOfFiles);
		mXmlParser = new RapidXmlParser(xmlFile);
		mXmlParser->runParsing();

		// Question area
		String question = mSqlConnector->UTF8ToANSI(mXmlParser->getQuestionColumn());
		String tokenized_question;
		filterSentence(tokenized_question, question);
		std::list<String> *pQuestionSplited = new std::list<String>();
		splitStringToNgram(pQuestionSplited, tokenized_question);

		// construct unigram
		std::map<Integer, Integer>	*pQuestionUnigram = new std::map<Integer, Integer>();
		contructUnigram(pQuestionSplited, pQuestionUnigram);
		if(!mSqlConnector->insertUnigrams(cur_progress, pQuestionUnigram, QUESTION))
			return false;
		delete pQuestionUnigram;

		// construct bigram
		std::map<std::pair<Integer, Integer>, Integer> *pQuestionBigram = new std::map<std::pair<Integer, Integer>, Integer>();
		conructBigram(pQuestionSplited, pQuestionBigram);
		if(!mSqlConnector->insertBigrams(cur_progress, pQuestionBigram, QUESTION))
			return false;
		delete pQuestionBigram;

		delete pQuestionSplited;

		// Answer area
		String answer = mSqlConnector->UTF8ToANSI(mXmlParser->getAnswerColumn());
		String tokenized_answer;
		filterSentence(tokenized_answer, answer);
		std::list<String> *pAnswerSplited = new std::list<String>();
		splitStringToNgram(pAnswerSplited, tokenized_answer);

		// construct unigram
		std::map<Integer, Integer>	*pAnswerUnigram = new std::map<Integer, Integer>();
		contructUnigram(pAnswerSplited, pAnswerUnigram);
		if(!mSqlConnector->insertUnigrams(cur_progress, pAnswerUnigram, ANSWER))
			return false;
		delete pAnswerUnigram;

		// construct bigram
		std::map<std::pair<Integer, Integer>, Integer> *pAnswerBigram = new std::map<std::pair<Integer, Integer>, Integer>();
		conructBigram(pAnswerSplited, pAnswerBigram);
		if(!mSqlConnector->insertBigrams(cur_progress, pAnswerBigram, ANSWER))
			return false;
		delete pAnswerBigram;

		delete pAnswerSplited;

		delete mXmlParser;
		cur_progress++;
		//end_time = clock();
		//std::cout<< "Insert 1 docs training time : " << static_cast<Real>(end_time-start_time)/CLOCKS_PER_SEC << std::endl << std::endl;
	}
	delete pFlist;

	mSqlConnector->delete_m1_DB( QUESTION);
	mSqlConnector->delete_m1_DB( ANSWER);
	if(!mSqlConnector->insertNgramStr2Int(mStr2Int))
		return false;
	std::cout<<std::endl;

	return true;
}

void NgramTrainer::contructUnigram(std::list<String> *splited, std::map<Integer, Integer> *unigram)
{
	std::forward_list<Integer> lst_int_splited;

	for(auto word=splited->begin(); word!=splited->end(); word++)
	{
		Integer word_id;
		auto find_result = mStr2Int.find(*word);

		if(find_result != mStr2Int.end())
			word_id = find_result->second;
		else
		{
			word_id = mStr2Int.size();
			mStr2Int.insert(std::make_pair(*word, word_id));
		}
		lst_int_splited.push_front(word_id);
	}

	for(Integer& word_id : lst_int_splited)
	{
		auto find_result = unigram->find(word_id);

		if(find_result != unigram->end())
		{
			find_result->second++;
		}
		else
		{
			unigram->insert(std::make_pair(word_id, 1));
		}
	}
}

void NgramTrainer::conructBigram(std::list<String> *splited, std::map<std::pair<Integer, Integer>, Integer> *bigram)
{
	for(int i=0; i<mNgramN-1; i++)
	{
		splited->push_front(SENTENCE_TAG);
		splited->push_back(SENTENCE_TAG);
	}

	std::vector<Integer> lst_int_splited;

	for(auto word=splited->begin(); word!=splited->end(); word++)
	{
		Integer word_id;
		auto find_result = mStr2Int.find(*word);

		if(find_result != mStr2Int.end())
			word_id = find_result->second;
		else
		{
			word_id = mStr2Int.size();
			mStr2Int.insert(std::make_pair(*word, word_id));
		}
		lst_int_splited.push_back(word_id);
	}

	auto first_word = lst_int_splited.begin();
	auto second_word = first_word+1;
	while(second_word != lst_int_splited.end())
	{
		auto key = std::make_pair(*first_word, *second_word);
		auto find_result = bigram->find(key);

		if(find_result != bigram->end())
		{
			find_result->second++;
		}
		else
		{
			bigram->insert(std::make_pair(key, 1));
		}

		first_word++;
		second_word++;
	}
}