#ifndef _QUERY_NGRAM_H_
#define _QUERY_NGRAM_H_


#include "../Utils/StdRedef.h"
#include "../Utils/SqliteConnector.h"
#include "../Utils/stringutil.h"
#include "QueryAnalyzer.h"


class QryNgram : public QueryAnalyzer
{
private:
	SqliteConnector* mSqlConnector;

private:
	std::list<Integer> *refineQuery(String &query);

public:
	QryNgram();
	QryNgram(SqliteConnector* sqlConnector);
	virtual ~QryNgram();

public:
	virtual void beginQueryAnalysis(String& query, std::forward_list<Term<String, Integer>> **query_result);
	virtual void beginQueryAnalysis(String& query, std::list<Integer> **query_result);
	virtual void beginQueryAnalysis(String& query, std::set<Term<String, Integer>> **query_result);
};


#endif