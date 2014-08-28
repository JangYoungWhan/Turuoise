#include "NaiveBeysian.h"


NaiveBeysian::NaiveBeysian()
{ }

NaiveBeysian::NaiveBeysian(Integer numOfDoc, SqliteConnector* SqlConnector)
	:ScoreCalculator(numOfDoc, SqlConnector)
{ }

NaiveBeysian::~NaiveBeysian()
{ }

inline
Integer NaiveBeysian::getSumOfDocFreq(const std::map<String, FreqScore<Integer, Integer>>	*doc_info) const
{
	Integer sumOfFreq = 0;

	for(auto doc=doc_info->begin(); doc!=doc_info->end(); doc++)
		sumOfFreq += doc->second.getTermFreq();

	return sumOfFreq;
}

inline
Real NaiveBeysian::prob_w_d(Integer w_freq, Integer d_freq)
{
	return (w_freq) / static_cast<Real>(d_freq);
}

inline
Real NaiveBeysian::applyLaplaceSmoothing(Real real)
{
	return log(real+M_E); // It always have a positive real value due to laplace smoothing.
}

/*
void NaiveBeysian::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{
	// do nothing.
}

void NaiveBeysian::beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result)
{
	// do nothing.
}
*/

void NaiveBeysian::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, double synonym, double levenshtein)
{
	std::cout << "NaiveBeysian::beginScoring" << std::endl;

	score_result.resize(mNumOfDocs);
	for(auto i=0; i<mNumOfDocs; i++)
	{
		mProgressBar->dispalyPrgressBar(i, mNumOfDocs-1);

		mSetDocInfoInQuestion = mSqlConnector->getDocInfoMap(i, QUESTION);
		mSetDocInfoInAnswer = mSqlConnector->getDocInfoMap(i, ANSWER);

		auto sumOfQueFreq = getSumOfDocFreq(mSetDocInfoInQuestion);
		auto sumOfAnsFreq = getSumOfDocFreq(mSetDocInfoInAnswer);
		Real que_prob = 0;
		Real ans_prob = 0;
		for(auto qry=query_result->begin(); qry!=query_result->end(); qry++)
		{
			// calculate question area
			auto find_result_in_que = mSetDocInfoInQuestion->find(qry->getTerm());
			if(find_result_in_que != mSetDocInfoInQuestion->end())
				que_prob += applyLaplaceSmoothing(prob_w_d(find_result_in_que->second.getTermFreq(), sumOfQueFreq));


			// calculate answer area
			auto find_result_in_ans = mSetDocInfoInAnswer->find(qry->getTerm());
			if(find_result_in_ans != mSetDocInfoInAnswer->end())
				ans_prob += applyLaplaceSmoothing(prob_w_d(find_result_in_ans->second.getTermFreq(), sumOfAnsFreq));
		}

		delete mSetDocInfoInQuestion;
		delete mSetDocInfoInAnswer;

		DocInfo doc(i, que_prob*QUESTION_RATIO + ans_prob*ANSWER_RATIO);
		score_result[i] = doc;
	}
	std::cout << std::endl;
}