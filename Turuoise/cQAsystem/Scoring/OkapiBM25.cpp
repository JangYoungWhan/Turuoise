#include "OkapiBM25.h"
#include <omp.h>


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

/*
void OkapiBM25::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{
	// do nothing.
}

void OkapiBM25::beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result)
{
	// do nothing.
}
*/

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
Real OkapiBM25::cal_avgdl( std::map< Integer, Integer> map_dl) const
{
	Integer sumOfDocLen = 0;

	for(auto iter = map_dl.begin() ; iter != map_dl.end() ; iter++)
	{
		sumOfDocLen += iter->second;
	}

	return static_cast<Real>(sumOfDocLen) / mNumOfDocs;
}

inline
Real OkapiBM25::calBM25(Real tf, Real idf, Integer dl, Real avgdl) const
{
	return idf*((tf*(BM25_k1+1))/(tf*(1-BM25_b+BM25_b*(dl/avgdl))));
}

/*
void OkapiBM25::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, const double synonym, const double levenshtein)
{
	std::cout << "OkapiBM25::beginScoring" << std::endl;

	score_result.resize(mNumOfDocs);
	for(auto doc_id=0; doc_id<mNumOfDocs; doc_id++)
	{
		mProgressBar->dispalyPrgressBar(doc_id, mNumOfDocs-1);

		Real que_prob = 0.0;
		Real ans_prob = 0.0;
		auto avgdl_q = cal_avgdl(QUESTION);
		auto avgdl_a = cal_avgdl(ANSWER);
		
		mSetDocInfoInQuestion = mSqlConnector->getDocInfoMap(doc_id, QUESTION);
		mSetDocInfoInAnswer = mSqlConnector->getDocInfoMap(doc_id, ANSWER);

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
*/

void OkapiBM25::beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result, const double synonym, const double levenshtein)
{

}



void OkapiBM25::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, const double synonym, const double levenshtein)
{
	std::cout << "OkapiBM25::beginScoring" << std::endl;

	score_result.resize(mNumOfDocs);

	std::map< Integer, std::map<String, FreqScore<Integer, Integer>>> mapQuestionDoc = mSqlConnector->getALLDocInfoMap( QUESTION);
	//std::map< Integer, std::map<String, FreqScore<Integer, Integer>>> mapAnswerDoc = mSqlConnector->getALLDocInfoMap( ANSWER);

	std::map< Integer, Integer> mapQuestionDF = mSqlConnector->getALLDF( QUESTION);
	//std::map< Integer, Integer> mapAnswerDF = mSqlConnector->getALLDF( ANSWER);

	std::map< Integer, Integer> mapQuestionDL = mSqlConnector->getALLDocTextLength( QUESTION);
	//std::map< Integer, Integer> mapAnswerDL = mSqlConnector->getALLDocTextLength( ANSWER);
		
	omp_set_num_threads( 4);
	//omp_set_num_threads( 8);
		
	int count = 0;
#pragma omp parallel
#pragma omp for nowait
	for(auto doc_id=0; doc_id<mNumOfDocs; doc_id++)
	{
#pragma omp critical
		mProgressBar->dispalyPrgressBar( count++, mNumOfDocs-1);

		Real que_prob = 0.0;
		Real ans_prob = 0.0;
		auto avgdl_q = cal_avgdl( mapQuestionDL);
		//auto avgdl_a = cal_avgdl( mapAnswerDL);
		
		std::map<String, FreqScore<Integer, Integer>>	mSetDocInfoInQuestion = mapQuestionDoc[ doc_id];
		//std::map<String, FreqScore<Integer, Integer>>	mSetDocInfoInAnswer = mapAnswerDoc[ doc_id];
		//mSetDocInfoInQuestion = mSqlConnector->getDocInfoMap(doc_id, QUESTION);
		//mSetDocInfoInAnswer = mSqlConnector->getDocInfoMap(doc_id, ANSWER);

		for(auto qry=query_result->begin(); qry!=query_result->end(); qry++)
		{
			// if 동의어 온일때만 가져오는거
			std::vector< String> vec_synonym;
			if( synonym > EPSILON) {
				vec_synonym = getSynonymFromMemory( qry->getTerm());
				//vec_synonym = getSynonymFromMemory( mSqlConnector->getWordID( qry->getTerm()));
				//vec_synonym = mSqlConnector->getSynonym( mSqlConnector->ANSIToUTF8( qry->getTerm().c_str()));
				if( vec_synonym.size() == 0)
					vec_synonym.push_back( qry->getTerm());
			}

			// calculate question area
			for( auto que = mSetDocInfoInQuestion.begin() ; que != mSetDocInfoInQuestion.end() ; que++)
			{
				if( qry->getTerm().compare( que->first) == 0) {
					auto tf = qry->getTermFreq();
					auto df = mapQuestionDF[ getWORDID_FromMemory( qry->getTerm())];
					auto idf = cal_idf(df);
					auto curdl = mapQuestionDL[ doc_id];

					que_prob += calBM25(tf, idf, curdl, avgdl_q);
				}
				else {
					std::vector< Term<String, Integer>> vec_qry;
					double synonym_score = synonym;

					if( synonym > EPSILON) {
						for( int n2 = 0 ; n2 < vec_synonym.size() ; n2++)
							vec_qry.push_back( Term<String, Integer>( vec_synonym[ n2], qry->getTermFreq()));
						synonym_score = synonym;
					}
					else {
						vec_qry.push_back( *qry);
						synonym_score = 1;
					}

					double max_element_que_prob = 0;
					for( int n3 = 0 ; n3 < vec_qry.size() ; n3++) {
						double levenshtein_score;
						if( vec_qry[ n3].getTerm().compare( que->first) == 0)
							levenshtein_score = 1;
						else {
							if( levenshtein > EPSILON) {
								std::string str1 = vec_qry[ n3].getTerm();
								std::string str2 = que->first;
								levenshtein_score = mSqlConnector->get_levenshtein_distance( mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str1.c_str())), mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str2.c_str())));
								levenshtein_score *= levenshtein;
							}
							else
								levenshtein_score = 0;
						}

						if( levenshtein_score > 0.5) {
							auto tf = qry->getTermFreq();
							auto df = mapQuestionDF[ getWORDID_FromMemory( qry->getTerm())];
							auto idf = cal_idf(df);
							auto curdl = mapQuestionDL[ doc_id];
							
							double temp_que_prob = calBM25( tf, idf, curdl, avgdl_q) * levenshtein_score * synonym_score;
							if( temp_que_prob > max_element_que_prob)
								max_element_que_prob = temp_que_prob;
						}
					}
					que_prob += max_element_que_prob;
				}
			}
			/*
			// calculate answer area
			for( auto ans = mSetDocInfoInAnswer.begin() ; ans != mSetDocInfoInAnswer.end() ; ans++)
			{
				if( qry->getTerm().compare( ans->first) == 0) {
					auto tf = qry->getTermFreq();
					auto df = mapAnswerDF[ getWORDID_FromMemory( qry->getTerm())];
					auto idf = cal_idf(df);
					auto curdl = mapAnswerDL[ doc_id];

					ans_prob += calBM25(tf, idf, curdl, avgdl_a);
				}
				else {
					std::vector< Term<String, Integer>> vec_qry;
					double synonym_score = synonym;

					if( synonym > EPSILON) {
						for( int n2 = 0 ; n2 < vec_synonym.size() ; n2++)
							vec_qry.push_back( Term<String, Integer>( vec_synonym[ n2], qry->getTermFreq()));
						synonym_score = synonym;
					}
					else {
						vec_qry.push_back( *qry);
						synonym_score = 1;
					}

					double max_element_ans_prob = 0;
					for( int n3 = 0 ; n3 < vec_qry.size() ; n3++) {
						double levenshtein_score;

						if( vec_qry[ n3].getTerm().compare( ans->first) == 0)
							levenshtein_score = 1;
						else {
							if( levenshtein > EPSILON) {
								std::string str1 = vec_qry[ n3].getTerm();
								std::string str2 = ans->first;
								levenshtein_score = mSqlConnector->get_levenshtein_distance( mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str1.c_str())), mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str2.c_str())));
								levenshtein_score *= levenshtein;
							}
							else 
								levenshtein_score = 0;
						}

						if( levenshtein_score > 0.5) {
							auto tf = qry->getTermFreq();
							auto df = mapAnswerDF[ getWORDID_FromMemory( qry->getTerm())];
							auto idf = cal_idf(df);
							auto curdl = mapAnswerDL[ doc_id];

							double temp_ans_prob = calBM25(tf, idf, curdl, avgdl_a) * levenshtein_score * synonym_score;
							if( temp_ans_prob > max_element_ans_prob)
								max_element_ans_prob = temp_ans_prob;
						}
					}
					ans_prob += max_element_ans_prob;
				}
			}
			*/
		}
		

		//DocInfo doc(doc_id, que_prob*0.5 + ans_prob*0.5);
		DocInfo doc(doc_id, que_prob*QUESTION_RATIO);
		score_result[doc_id] = doc;
	}
	std::cout << std::endl;
}
