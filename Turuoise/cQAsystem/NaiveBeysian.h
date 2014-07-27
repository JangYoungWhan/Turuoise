#ifndef _NAIVE_BEYSIAN_H_
#define _NAIVE_BEYSIAN_H_


#include <iostream>
#include <forward_list>
#include <set>
#include "StdRedef.h"
#include "Term.h"
#include "SqliteConnector.h"
#include "ScoreCalculator.h"

class NaiveBeysian : public ScoreCalculator
{
private:
	std::set<Term<String, Integer>>	*mSetDocInfoInQuestion;
	std::set<Term<String, Integer>>	*mSetDocInfoInAnswer;

public:
	NaiveBeysian();
	NaiveBeysian(SqliteConnector* SqlConnector);
	virtual ~NaiveBeysian();

public:
	virtual void beginScoring(std::forward_list<Term<String, Integer>> *query_result);
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result);
};


#endif