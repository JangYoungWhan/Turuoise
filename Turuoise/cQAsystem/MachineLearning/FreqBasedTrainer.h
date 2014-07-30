#ifndef _FREQUENCY_BASED_TRAINER_
#define _FREQUENCY_BASED_TRAINER_


#include <iostream>
#include "../Utils/StdRedef.h"
#include "BaseTrainer.h"

class FreqBasedTrainer : public BaseTrainer
{
public:
	FreqBasedTrainer();
	FreqBasedTrainer(SqliteConnector* SqlConnector);
	virtual ~FreqBasedTrainer();

	virtual bool beginTraning(String& trainingPath, std::map<Integer, String>& docID2Path);
};


#endif