#include "OkapiBM25.h"


OkapiBM25::OkapiBM25()
	:BM25_k1(2.0), BM25_b(0.75)
{ }

OkapiBM25::OkapiBM25(Integer numOfDoc, SqliteConnector* SqlConnector)
	:ScoreCalculator(numOfDoc, SqlConnector), BM25_k1(2.0), BM25_b(0.75)
{ }

OkapiBM25::~OkapiBM25()
{ }


void OkapiBM25::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{

}

inline
Real OkapiBM25::calc_idf(Real df)
{
	return log((mNumOfDocs-df+0.5)/(df+0.5));
}

inline
Real OkapiBM25::calc_tf(String q, Integer d, Integer f)
{
	
	return mSqlConnector->getTF(q, d, f);
}

inline 
Real OkapiBM25::calc_df(String q, Integer f)
{
	return mSqlConnector->getDF(q, f);
}

inline
Integer OkapiBM25::calc_dl(Integer d, Integer f)
{
	//return mSqlConnector->getDocLen(d, f);
	return 1;
}

inline
Integer OkapiBM25::calc_avgdl(Integer f)
{
	return 1;
}


void OkapiBM25::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result)
{
	std::cout << "NaiveBeysian::beginScoring" << std::endl;

	score_result.resize(mNumOfDocs);
	for(auto doc_id=0; doc_id<mNumOfDocs; doc_id++)
	{
		mProgressBar->dispalyPrgressBar(doc_id, mNumOfDocs-1);

		mSetDocInfoInQuestion = mSqlConnector->getDocInfoMap(doc_id, QUESTION);
		mSetDocInfoInAnswer = mSqlConnector->getDocInfoMap(doc_id, ANSWER);
		
		//auto sumOfQueFreq = getSumOfDocFreq(mSetDocInfoInQuestion);
		//auto sumOfAnsFreq = getSumOfDocFreq(mSetDocInfoInAnswer);
		Real que_prob = 0;
		Real ans_prob = 0;
		auto avgdl_q = calc_avgdl(QUESTION);
		auto avgdl_a = calc_avgdl(ANSWER);
		for(auto qry=query_result->begin(); qry!=query_result->end(); qry++)
		{
			// calculate question area
			auto find_result_in_que = mSetDocInfoInQuestion->find(qry->getTerm());
			if(find_result_in_que != mSetDocInfoInQuestion->end())
			{
				auto tf = calc_tf(qry->getTerm(), doc_id, QUESTION);
				auto df = calc_df(qry->getTerm(), QUESTION);				
				auto idf = calc_idf(df);
				auto curdl = calc_dl(doc_id, QUESTION);

				que_prob += idf*((tf*(BM25_k1+1))/(tf*(1-BM25_b+BM25_b*(curdl/avgdl_q))));
			}


			// calculate answer area
			auto find_result_in_ans = mSetDocInfoInAnswer->find(qry->getTerm());
			if(find_result_in_ans != mSetDocInfoInAnswer->end())
			{
				auto tf = calc_tf(qry->getTerm(), doc_id, ANSWER);
				auto df = calc_df(qry->getTerm(), ANSWER);				
				auto idf = calc_idf(df);
				auto curdl = calc_dl(doc_id, ANSWER);

				que_prob += idf*((tf*(BM25_k1+1))/(tf*(1-BM25_b+BM25_b*(curdl/avgdl_q))));
			}
		}
		
		DocInfo doc(doc_id, que_prob*QUESTION_RATIO + ans_prob*ANSWER_RATIO);
		score_result[doc_id] = doc;
		
	}
	std::cout << std::endl;
}