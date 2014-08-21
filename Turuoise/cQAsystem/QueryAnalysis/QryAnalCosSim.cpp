#include "QryAnalCosSim.h"

QryAnalCosSim::QryAnalCosSim()
{
}

QryAnalCosSim::~QryAnalCosSim()
{
}

void QryAnalCosSim::beginQueryAnalysis(String& query, std::forward_list<Term<String, Integer>> **query_result)
{
	std::cout << "QryAnalCosSim::beginQueryAnalysis()" << std::endl;

	*query_result = extractIndex2Flist(const_cast<char*>(query.c_str()));
}

void QryAnalCosSim::beginQueryAnalysis(String& query, std::list<Integer> **query_result)
{
	std::cout << "QryAnalCosSim::beginQueryAnalysis()" << std::endl;

	// do nothing.
}

void QryAnalCosSim::beginQueryAnalysis(String& query, std::set<Term<String, Integer>> **query_result)
{
	std::cout << "QryAnalCosSim::beginQueryAnalysis()" << std::endl;

	*query_result = extractIndex2Set(const_cast<char*>(query.c_str()));
}