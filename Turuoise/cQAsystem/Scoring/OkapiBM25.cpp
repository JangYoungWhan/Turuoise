#include "OkapiBM25.h"


OkapiBM25::OkapiBM25()
	:BM25_k1(1.0), BM25_b(0.75)
{ }
OkapiBM25::OkapiBM25(Integer numOfDoc, SqliteConnector* SqlConnector)
	:BM25_k1(2.0), BM25_b(0.75), ScoreCalculator(numOfDoc, SqlConnector)
{ }
OkapiBM25::OkapiBM25(Real question_ratio, Real answer_ratio, Integer numOfDoc, SqliteConnector* SqlConnector)
	:BM25_k1(1.0), BM25_b(0.75), ScoreCalculator(question_ratio, answer_ratio, numOfDoc, SqlConnector)
{ }
OkapiBM25::~OkapiBM25()
{ }

void OkapiBM25::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{
	// do nothing.
}

void OkapiBM25::beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result)
{
	// do nothing.
}

inline
Real OkapiBM25::cal_idf(Real df) const
{
	return log((mNumOfDocs-df+0.5)/(df+0.5));
}

inline
Real OkapiBM25::cal_tf(String q, Integer d, int flag) const
{
	
	return mSqlConnector->getTF(q, d, flag);
}

inline 
Real OkapiBM25::cal_df(String q, int flag) const
{
	return mSqlConnector->getDF(q, flag);
}

inline
Integer OkapiBM25::cal_dl(Integer d, int flag) const
{
	return mSqlConnector->getDocTextLength(d, flag);
}

inline
Real OkapiBM25::cal_avgdl(int flag) const
{
	Integer sumOfDocLen = 0;

	for(auto doc_id=0; doc_id<mNumOfDocs; doc_id++)
	{
		sumOfDocLen += mSqlConnector->getDocTextLength(doc_id, flag);
	}

	return static_cast<Real>(sumOfDocLen) / mNumOfDocs;
}

inline
Real OkapiBM25::calBM25(Real tf, Real idf, Integer dl, Real avgdl) const
{
	return idf*((tf*(BM25_k1+1))/(tf*(1-BM25_b+BM25_b*(dl/avgdl))));
}

void OkapiBM25::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result)
{
	std::cout << "OkapiBM25::beginScoring" << std::endl;

	score_result.resize(mNumOfDocs);
	for(auto doc_id=0; doc_id<mNumOfDocs; doc_id++)
	{
		mProgressBar->dispalyPrgressBar(doc_id, mNumOfDocs-1);

		mSetDocInfoInQuestion = mSqlConnector->getDocInfoMap(doc_id, QUESTION);
		mSetDocInfoInAnswer = mSqlConnector->getDocInfoMap(doc_id, ANSWER);

		Real que_prob = 0.0;
		Real ans_prob = 0.0;
		auto avgdl_q = cal_avgdl(QUESTION);
		auto avgdl_a = cal_avgdl(ANSWER);
		for(auto qry=query_result->begin(); qry!=query_result->end(); qry++)
		{
			// calculate question area
			auto find_result_in_que = mSetDocInfoInQuestion->find(qry->getTerm());
			if(find_result_in_que != mSetDocInfoInQuestion->end())
			{
				auto tf = cal_tf(qry->getTerm(), doc_id, QUESTION);
				auto df = cal_df(qry->getTerm(), QUESTION);				
				auto idf = cal_idf(df);
				auto curdl = cal_dl(doc_id, QUESTION);

				que_prob += calBM25(tf, idf, curdl, avgdl_q);
			}

			// calculate answer area
			auto find_result_in_ans = mSetDocInfoInAnswer->find(qry->getTerm());
			if(find_result_in_ans != mSetDocInfoInAnswer->end())
			{
				auto tf = cal_tf(qry->getTerm(), doc_id, ANSWER);
				auto df = cal_df(qry->getTerm(), ANSWER);				
				auto idf = cal_idf(df);
				auto curdl = cal_dl(doc_id, ANSWER);

				ans_prob += calBM25(tf, idf, curdl, avgdl_a);
			}
		}
		
		DocInfo doc(doc_id, que_prob*QUESTION_RATIO + ans_prob*ANSWER_RATIO);
		score_result[doc_id] = doc;
		
	}
	std::cout << std::endl;
}