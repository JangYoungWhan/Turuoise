#include "QryNgram.h"

QryNgram::QryNgram()
{
}
QryNgram::QryNgram(SqliteConnector* sqlConnector)
{
	this->mSqlConnector = sqlConnector;
}
QryNgram::~QryNgram()
{
}

std::list<Integer> *QryNgram::refineQuery(String &query)
{
	std::list<Integer> *pLstResult = new std::list<Integer>();

	String tokenized_query;
	filterSentence(tokenized_query, query);
	std::list<String> *pAnswerSplited = new std::list<String>();
	splitStringToNgram(pAnswerSplited, tokenized_query);

	for(auto iter=pAnswerSplited->begin(); iter!=pAnswerSplited->end(); iter++)
	{
		auto term_id = mSqlConnector->getNgramWordID(*iter);

		pLstResult->push_back(term_id);
	}
	delete pAnswerSplited;

	auto sentence_tag_id = mSqlConnector->getNgramWordID(SENTENCE_TAG);
	pLstResult->push_front(sentence_tag_id);
	pLstResult->push_back(sentence_tag_id);

	return pLstResult;
}

void QryNgram::beginQueryAnalysis(String& query, std::forward_list<Term<String, Integer>> **query_result)
{
	std::cout << "QryNgram::beginQueryAnalysis()" << std::endl;

	// do nothing.
}

void QryNgram::beginQueryAnalysis(String& query, std::list<Integer> **query_result)
{
	std::cout << "QryNgram::beginQueryAnalysis()" << std::endl;

	*query_result = refineQuery(query);
}

void QryNgram::beginQueryAnalysis(String& query, std::set<Term<String, Integer>> **query_result)
{
	std::cout << "QryNgram::beginQueryAnalysis()" << std::endl;

	// do nothing.
}