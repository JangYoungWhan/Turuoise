#include "NaiveBeysian.h"


NaiveBeysian::NaiveBeysian()
{

}

NaiveBeysian::NaiveBeysian(SqliteConnector* SqlConnector)
	:ScoreCalculator(SqlConnector)
{

}

NaiveBeysian::~NaiveBeysian()
{

}

void NaiveBeysian::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{

}

void NaiveBeysian::beginScoring(std::set<Term<String, Integer>> *query_result)
{
	std::cout << "NaiveBeysian::beginScoring" << std::endl;

	for(auto i=0; i<500; i++)
	{
		mSetDocInfoInQuestion = mSqlConnector->getDocInfoSet(i, QUESTION);
		mSetDocInfoInAnswer = mSqlConnector->getDocInfoSet(i, ANSWER);
	}

}