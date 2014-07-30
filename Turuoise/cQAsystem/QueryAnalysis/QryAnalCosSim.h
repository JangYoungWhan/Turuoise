#ifndef _QUERY_ANALYSIS_COSINE_SIMILARITY_H_
#define _QUERY_ANALYSIS_COSINE_SIMILARITY_H_


#include <forward_list>
#include "../Utils/StdRedef.h"
#include "QueryAnalyzer.h"


class QryAnalCosSim : public QueryAnalyzer
{
public:
	QryAnalCosSim();
	virtual ~QryAnalCosSim();

public:
	virtual void beginQueryAnalysis(String& query, std::forward_list<Term<String, Integer>> **query_result);
	virtual void beginQueryAnalysis(String& query, std::set<Term<String, Integer>> **query_result);
};


#endif