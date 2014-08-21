#ifndef _NGRAM_TRAINER_
#define _NGRAM_TRAINER_


#include <iostream>
#include <fstream>
#include <list>
#include "../Utils/StdRedef.h"
#include "../Utils/NgramInfo.h"
#include "../Utils/stringutil.h"
#include "BaseTrainer.h"

class NgramTrainer : public BaseTrainer
{
private:
	Integer						mNgramN;
	std::map<String, Integer>	mStr2Int;

private:
	void contructUnigram(std::list<String> *splited, std::map<Integer, Integer> *unigram);
	void conructBigram(std::list<String> *splited, std::map<std::pair<Integer, Integer>, Integer> *bigram);

public:
	NgramTrainer();
	NgramTrainer(SqliteConnector* SqlConnector);
	virtual ~NgramTrainer();

	virtual bool beginTraning(String& trainingPath, std::map<Integer, String>& docID2Path);
};


#endif