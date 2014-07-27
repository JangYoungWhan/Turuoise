#include "CosineSimilarity.h"

CosineSimilarity::CosineSimilarity()
{ }
CosineSimilarity::CosineSimilarity(SqliteConnector* SqlConnector)
	:ScoreCalculator(SqlConnector)
{ }
CosineSimilarity::~CosineSimilarity()
{ }

void CosineSimilarity::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{

}

void CosineSimilarity::beginScoring(std::set<Term<String, Integer>> *query_result)
{

}