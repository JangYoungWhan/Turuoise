#ifndef _QUERY_ANALYSIS_COSINE_SIMILARITY_H_
#define _QUERY_ANALYSIS_COSINE_SIMILARITY_H_


#include <forward_list>
#include "StdRedef.h"
#include "QueryAnalyzer.h"


class QryAnalCosSim : public QueryAnalyzer
{
private:
	std::forward_list<Term<String, Integer>> mQueryResult;

public:
	virtual void beginQueryAnalysis(String& query);

	std::forward_list<Term<String, Integer>>& getQueryResult() const;
};



#endif