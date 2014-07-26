#ifndef _QUERY_ANALYZER_H_
#define _QUERY_ANALYZER_H_


#include "StdRedef.h"
#include "KoreanMorphologicalAnalyzer.h"

class QueryAnalyzer : virtual public KoreanMorphologicalAnalyzer<String, Integer>
{
protected:

public:
	virtual void beginQueryAnalysis(String& query) = 0;
};


#endif