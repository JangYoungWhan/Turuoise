#ifndef _COSINE_SIMILARITY_H_
#define _COSINE_SIMILARITY_H_


#include <forward_list>
#include <set>
#include "../Utils/StdRedef.h"
#include "../Utils/Term.h"
#include "../Scoring/ScoreCalculator.h"
#include "../Utils/SqliteConnector.h"
#include "../Utils/DocInfo.h"


class CosineSimilarity : public ScoreCalculator
{
private:

public:
	CosineSimilarity();
	CosineSimilarity(Integer numOfDoc, SqliteConnector* SqlConnector);
	virtual ~CosineSimilarity();

public:
	//virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result);
	virtual void beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result, const double synonym = 0.0, const double levenshtein = 0.0);
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, const double synonym = 0.0, const double levenshtein = 0.0);
};


#endif