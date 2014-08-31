#include "NaiveBeysian.h"
#include <omp.h>

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

/*
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
*/


void NaiveBeysian::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, double synonym, double levenshtein)
{
	std::cout << "NaiveBeysian::beginScoring" << std::endl;
	score_result.resize(mNumOfDocs);

	std::map< Integer, std::map<String, FreqScore<Integer, Integer>>> mapQuestionDoc = mSqlConnector->getALLDocInfoMap( QUESTION);
	std::map< Integer, std::map<String, FreqScore<Integer, Integer>>> mapAnswerDoc = mSqlConnector->getALLDocInfoMap( ANSWER);
		
	omp_set_num_threads( 4);
	//omp_set_num_threads( 8);
		
	int count = 0;
#pragma omp parallel
#pragma omp for nowait
	for(auto i=0; i<mNumOfDocs; i++)
	{
		#pragma omp critical
		mProgressBar->dispalyPrgressBar(count++, mNumOfDocs-1);

		std::map<String, FreqScore<Integer, Integer>>	mSetDocInfoInQuestion = mapQuestionDoc[ i];
		std::map<String, FreqScore<Integer, Integer>>	mSetDocInfoInAnswer = mapAnswerDoc[ i];

		//mSetDocInfoInQuestion = mSqlConnector->getDocInfoMap(i, QUESTION);
		//mSetDocInfoInAnswer = mSqlConnector->getDocInfoMap(i, ANSWER);
		
		auto sumOfQueFreq = getSumOfDocFreq( &mSetDocInfoInQuestion);
		auto sumOfAnsFreq = getSumOfDocFreq( &mSetDocInfoInAnswer);
		Real que_prob = 0;
		Real ans_prob = 0;
		for(auto qry=query_result->begin(); qry!=query_result->end(); qry++)
		{
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
				if( qry->getTerm().compare( que->first) == 0)
					que_prob += applyLaplaceSmoothing(prob_w_d( que->second.getTermFreq(), sumOfQueFreq));
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
							double temp_que_prob = applyLaplaceSmoothing(prob_w_d( que->second.getTermFreq(), sumOfQueFreq)) * levenshtein_score * synonym_score;
							if( temp_que_prob > max_element_que_prob)
								max_element_que_prob = temp_que_prob;
						}
					}
					que_prob += max_element_que_prob;
				}
			}

			// calculate answer area
			for( auto ans = mSetDocInfoInAnswer.begin() ; ans != mSetDocInfoInAnswer.end() ; ans++)
			{
				if( qry->getTerm().compare( ans->first) == 0)
					ans_prob += applyLaplaceSmoothing(prob_w_d( ans->second.getTermFreq(), sumOfAnsFreq));
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
							double temp_ans_prob = ans_prob += applyLaplaceSmoothing(prob_w_d( ans->second.getTermFreq(), sumOfAnsFreq)) * levenshtein_score * synonym_score;
							if( temp_ans_prob > max_element_ans_prob)
								max_element_ans_prob = temp_ans_prob;
						}
					}
					ans_prob += max_element_ans_prob;
				}
			}
		}

		//delete mSetDocInfoInQuestion;
		//delete mSetDocInfoInAnswer;

		DocInfo doc(i, que_prob*QUESTION_RATIO + ans_prob*ANSWER_RATIO);
		score_result[i] = doc;
	}
	std::cout << std::endl;
}
