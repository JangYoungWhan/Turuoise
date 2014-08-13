/*
 * Title	: CQA system implemented in C++
 * Author	: YW. Jang, Zizo
 */


#include <iostream>
#include <string>
#include "QAsystem.h"
#include "CQAsystem.h"
#include <time.h>


#define _TRAINING_MODE_

int main(int argc, char* argv[])
{
	std::cout << "=== Run Turuoise ===" << std::endl;

	String TRAINING_DATA_PATH = "../../../../training_data/";
	String TRAINING_DB_NAME = "Turuoise.db";
	String QUERY = "영어 super를 한글로 표기할 때 수퍼라고 해야 하나요, 슈퍼라고 해야 하나요?Superman, supermarket 등은 수퍼맨, 수퍼마켓인가요, 슈퍼맨, 슈퍼마켓인가요";
	Integer DISPLAY_LIMIT = 10;

	QAsystem *pQAsystem = new CQAsystem(TRAINING_DB_NAME);

	#ifdef _TRAINING_MODE_
	clock_t start_time, end_time;
	start_time = clock();
	pQAsystem->beginTraning(TRAINING_DATA_PATH, false);
	end_time = clock();
	std::cout<< "Machine Learning Time : " << static_cast<Real>(end_time-start_time)/CLOCKS_PER_SEC << std::endl << std::endl;
	#else
	pQAsystem->beginTraning(TRAINING_DATA_PATH, true);
	#endif

	pQAsystem->analyzeQuery(QUERY);
	pQAsystem->calculateScore();
	pQAsystem->dispalyResult(DISPLAY_LIMIT);

	delete pQAsystem;

	return 0;
}
