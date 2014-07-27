#ifndef _QUERY_ANALYZER_H_
#define _QUERY_ANALYZER_H_


#include "StdRedef.h"
#include "KoreanMorphologicalAnalyzer.h"

class QueryAnalyzer : virtual public KoreanMorphologicalAnalyzer<String, Integer>
{
public:
	virtual void beginQueryAnalysis(String& query, std::forward_list<Term<String, Integer>> **query_result) = 0;
};


#endif