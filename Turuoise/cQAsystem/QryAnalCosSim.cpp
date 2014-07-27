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

	*query_result = extractIndex2List(const_cast<char*>(query.c_str()));
}