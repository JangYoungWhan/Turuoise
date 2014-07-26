#ifndef _FREQUENCY_BASED_TRAINER_
#define _FREQUENCY_BASED_TRAINER_


#include <iostream>
#include "StdRedef.h"
#include "BaseTrainer.h"

class FreqBasedTrainer : public BaseTrainer
{
public:
	FreqBasedTrainer();
	FreqBasedTrainer(SqliteConnector* SqlConnector);
	virtual ~FreqBasedTrainer();

	virtual bool beginTraning(String& trainingPath);
};


#endif