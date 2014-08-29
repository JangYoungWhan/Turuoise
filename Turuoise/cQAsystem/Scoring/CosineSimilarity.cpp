#include "CosineSimilarity.h"
#include <omp.h>

CosineSimilarity::CosineSimilarity()
{ }
CosineSimilarity::CosineSimilarity(Integer numOfDoc, SqliteConnector* SqlConnector)
	:ScoreCalculator(numOfDoc, SqlConnector)
{ }
CosineSimilarity::~CosineSimilarity()
{ }

/*
void CosineSimilarity::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{

}

void CosineSimilarity::beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result)
{

}
*/

void CosineSimilarity::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, double synonym, double levenshtein)
{
	std::cout << "CosineSimilarity::beginScoring" << std::endl;

	mNumOfDocs = mSqlConnector->getDocCount();
	score_result.resize(mNumOfDocs);
	for(auto i=0; i<mNumOfDocs; i++)
	{
		mProgressBar->dispalyPrgressBar(i, mNumOfDocs-1);
		
		mVectorDocInfoInQuestion = mSqlConnector->getDocInfoVector(i, QUESTION);
		mVectorDocInfoInAnswer = mSqlConnector->getDocInfoVector(i, ANSWER);

		Real que_prob = 0;
		Real ans_prob = 0;
		Integer magA, que_magB, ans_magB;
		magA = que_magB = ans_magB = 0;

		for(auto qry=query_result->begin(); qry!=query_result->end(); qry++)
		{
			std::vector< String> vec_synonym;
			if( synonym > EPSILON) {
				
				vec_synonym = getSynonymFromMemory( mSqlConnector->getWordID( qry->getTerm()));
				//vec_synonym = mSqlConnector->getSynonym( mSqlConnector->ANSIToUTF8( qry->getTerm().c_str()));
				if( vec_synonym.size() == 0)
					vec_synonym.push_back( qry->getTerm());
			}

			// calculate question area
			for( int n = 0 ; n < mVectorDocInfoInQuestion.size() ; n++) {
				Term<String, Integer> que_term = mVectorDocInfoInQuestion[ n];
				if( qry->getTerm().compare( que_term.getTerm()) == 0)
					que_prob += qry->getTermFreq() * que_term.getTermFreq();
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
						if( vec_qry[ n3].getTerm().compare( que_term.getTerm()) == 0)
							levenshtein_score = 1;
						else {
							if( levenshtein > EPSILON) {
								std::string str1 = vec_qry[ n3].getTerm();
								std::string str2 = que_term.getTerm();
								levenshtein_score = mSqlConnector->get_levenshtein_distance( mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str1.c_str())), mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str2.c_str())));
								levenshtein_score *= levenshtein;
							}
							else
								levenshtein_score = 0;
						}

						if( levenshtein_score > 0.5) {
							double temp_que_prob = qry->getTermFreq() * que_term.getTermFreq() * levenshtein_score * synonym_score;
							if( temp_que_prob > max_element_que_prob)
								max_element_que_prob = temp_que_prob;
						}
					}
					que_prob += max_element_que_prob;
				}
			}
			

			// calculate answer area
			for( int n = 0 ; n < mVectorDocInfoInAnswer.size() ; n++) {
				Term<String, Integer> ans_term = mVectorDocInfoInAnswer[ n];
				if( qry->getTerm().compare( ans_term.getTerm()) == 0)
					ans_prob += qry->getTermFreq() * ans_term.getTermFreq();
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

						if( vec_qry[ n3].getTerm().compare( ans_term.getTerm()) == 0)
							levenshtein_score = 1;
						else {
							if( levenshtein > EPSILON) {
								std::string str1 = vec_qry[ n3].getTerm();
								std::string str2 = ans_term.getTerm();
								levenshtein_score = mSqlConnector->get_levenshtein_distance( mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str1.c_str())), mSqlConnector->utf8_to_utf16( mSqlConnector->ANSIToUTF8( str2.c_str())));
								levenshtein_score *= levenshtein;
							}
							else 
								levenshtein_score = 0;
						}

						if( levenshtein_score > 0.5) {
							double temp_ans_prob = qry->getTermFreq() * ans_term.getTermFreq() * levenshtein_score * synonym_score;
							if( temp_ans_prob > max_element_ans_prob)
								max_element_ans_prob = temp_ans_prob;
						}
					}
					ans_prob += max_element_ans_prob;
				}
			}

			magA += qry->getTermFreq() * qry->getTermFreq();
		}
		


		for( int n = 0 ; n < mVectorDocInfoInQuestion.size() ; n++) {
			Term<String, Integer> que_term = mVectorDocInfoInQuestion[ n];
			que_magB += que_term.getTermFreq() * que_term.getTermFreq();
		}
			
		for( int n = 0 ; n < mVectorDocInfoInAnswer.size() ; n++) {
			Term<String, Integer> ans_term = mVectorDocInfoInAnswer[ n];
			ans_magB += ans_term.getTermFreq() * ans_term.getTermFreq();
		}
		
		
		double que_denom = sqrt( magA) * sqrt( que_magB);
		double ans_denom = sqrt( magA) * sqrt( ans_magB);
		que_prob = ( que_denom == 0)? 0 : que_prob / que_denom;
		ans_prob = ( ans_denom == 0)? 0 : ans_prob / ans_denom;

		DocInfo doc(i, que_prob*QUESTION_RATIO + ans_prob * ANSWER_RATIO);
		score_result[i] = doc;
		
	}
	std::cout << std::endl;
}