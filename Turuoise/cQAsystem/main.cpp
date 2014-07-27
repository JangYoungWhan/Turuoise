/*
 * Title	: It just my memo
 * Author	: YW. Jang, Zizo
 */
// DEBUG : io-defs.h(78) unicode redef

#include <iostream>
#include <string>
#include "QAsystem.h"
#include "CQAsystem.h"
#include <time.h>


//#define _TRAINING_MODE_

int main(int argc, char* argv[])
{
	std::cout << "=== Run Turuoise ===" << std::endl;

	String TRAINING_DATA_PATH = "../../../../training_data/";
	String TRAINING_DB_NAME = "Turuoise.db";
	String QUERY = "못하다는 동사인 줄 알았는데, 형용사로도 쓰인다고 하네요. 형용사 못하다가 어떻게 쓰이는지 딱 떠오르지를 않는데, 좀 알려 주세요.";

	QAsystem *pQAsystem = new CQAsystem(TRAINING_DB_NAME);

	#ifdef _TRAINING_MODE_
	clock_t start_time, end_time;
	start_time = clock();
	pQAsystem->beginTraning(TRAINING_DATA_PATH);
	end_time = clock();
	std::cout<< "Machine Learning Time : " << static_cast<Real>(end_time-start_time)/CLOCKS_PER_SEC << std::endl << std::endl;
	#endif

	pQAsystem->analyzeQuery(QUERY);
	pQAsystem->calculateScore();
	pQAsystem->dispalyResult();

	delete pQAsystem;

	

	
	return 0;
}
