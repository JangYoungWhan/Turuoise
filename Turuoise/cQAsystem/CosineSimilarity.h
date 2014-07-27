#ifndef _COSINE_SIMILARITY_H_
#define _COSINE_SIMILARITY_H_


#include <forward_list>
#include "StdRedef.h"
#include "Term.h"
#include "ScoreCalculator.h"
#include "SqliteConnector.h"


class CosineSimilarity : public ScoreCalculator
{
public:
	CosineSimilarity();
	CosineSimilarity(SqliteConnector* SqlConnector);
	virtual ~CosineSimilarity();

public:
	virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result);
};


#endif