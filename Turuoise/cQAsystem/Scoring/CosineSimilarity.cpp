#include "CosineSimilarity.h"

CosineSimilarity::CosineSimilarity()
{ }
CosineSimilarity::CosineSimilarity(Integer numOfDoc, SqliteConnector* SqlConnector)
	:ScoreCalculator(numOfDoc, SqlConnector)
{ }
CosineSimilarity::~CosineSimilarity()
{ }

void CosineSimilarity::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{

}

void CosineSimilarity::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result)
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
			// calculate question area
			for( int n = 0 ; n < mVectorDocInfoInQuestion.size() ; n++) {
				Term<String, Integer> que_term = mVectorDocInfoInQuestion[ n];
				if( qry->getTerm().compare( que_term.getTerm()) == 0) {
					que_prob += qry->getTermFreq() * que_term.getTermFreq();
					mVectorDocInfoInQuestion.erase( mVectorDocInfoInQuestion.begin() + n);
				}
			}
			
			// calculate answer area
			for( int n = 0 ; n < mVectorDocInfoInAnswer.size() ; n++) {
				Term<String, Integer> ans_term = mVectorDocInfoInAnswer[ n];
				if( qry->getTerm().compare( ans_term.getTerm()) == 0) {
					que_prob += qry->getTermFreq() * ans_term.getTermFreq();
					mVectorDocInfoInAnswer.erase( mVectorDocInfoInAnswer.begin() + n);
				}
			}

			magA += qry->getTermFreq() * qry->getTermFreq();
		}

		for( int n = 0 ; n < mVectorDocInfoInQuestion.size() ; n++) {
			Term<String, Integer> que_term = mVectorDocInfoInQuestion[ n];
			que_magB += que_term.getTermFreq() * que_term.getTermFreq();
		}
			
			// calculate answer area
		for( int n = 0 ; n < mVectorDocInfoInAnswer.size() ; n++) {
			Term<String, Integer> ans_term = mVectorDocInfoInAnswer[ n];
			ans_magB += ans_term.getTermFreq() * ans_term.getTermFreq();
		}
		
		/*
		magA = sqrt( magA);
		que_magB = sqrt( que_magB);
		ans_magB = sqrt( ans_magB);
		*/
		que_prob = que_prob / (double)( sqrt( magA) * sqrt( que_magB));
		ans_prob = ans_prob / (double)( sqrt( magA) * sqrt( ans_magB));

		DocInfo doc(i, que_prob*QUESTION_RATIO + ans_prob*ANSWER_RATIO);
		score_result[i] = doc;
		
	}
	std::cout << std::endl;
}