#ifndef _QUERY_ANALYZER_H_
#define _QUERY_ANALYZER_H_


#include <forward_list>
#include <list>
#include "../Utils/StdRedef.h"
#include "../Utils/KoreanMorphologicalAnalyzer.h"

class QueryAnalyzer : virtual public KoreanMorphologicalAnalyzer<String, Integer>
{
public:
	virtual void beginQueryAnalysis(String& query, std::forward_list<Term<String, Integer>> **query_result) = 0;
	virtual void beginQueryAnalysis(String& query, std::list<Integer> **query_result) = 0;
	virtual void beginQueryAnalysis(String& query, std::set<Term<String, Integer>> **query_result) = 0;
};


#endif