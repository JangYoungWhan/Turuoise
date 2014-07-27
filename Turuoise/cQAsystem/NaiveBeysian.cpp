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
	std::cout << "NaiveBeysian::beginScoring" << std::endl;
}