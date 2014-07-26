#include "QryAnalCosSim.h"


void QryAnalCosSim::beginQueryAnalysis(String& query)
{
	auto qryResult = extractIndex2List(const_cast<char*>(query.c_str()));
	std::cout << "QryAnalCosSim::beginQueryAnalysis(String& query)" << std::endl;
	//mQueryResult = extractIndex2List(const_cast<char*>(query.c_str()));
	// get cosine similarity between query and doc(n).
	// d1 => t1, t3, t4, ...
	// d2 => t5, t2, t3, ...
	// ...
	// DO NOT IMPLEMENT HERE!
	// Above job must operate in derived class of the CalcuateSocore abstract class.
}
