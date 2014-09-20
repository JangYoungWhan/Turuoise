#ifndef _SCORE_CALCULATOR_H_
#define _SCORE_CALCULATOR_H_


#include <vector>
#include <forward_list>
#include <list>
#include "../Utils/StdRedef.h"
#include "../Utils/Term.h"
#include "../Utils/SqliteConnector.h"
#include "../Utils/ProgressBar.h"
#include "../Utils/DocInfo.h"

extern std::vector< SynonymTerm> mSynonymTable;
extern std::map< Integer, String> mWordIDTable;

class ScoreCalculator
{
protected:
	Real QUESTION_RATIO;
	Real ANSWER_RATIO;

protected:
	Integer					mNumOfDocs;
	SqliteConnector*		mSqlConnector;
	ProgressBar<Integer>*	mProgressBar;

public:
	ScoreCalculator()
		:QUESTION_RATIO(0.5), ANSWER_RATIO(0.5), mNumOfDocs(0)
	{ }
	ScoreCalculator(Integer numOfDocs, SqliteConnector* SqlConnector)
		:QUESTION_RATIO(0.5), ANSWER_RATIO(0.5), mNumOfDocs(numOfDocs)
	{
		this->mProgressBar	= new ProgressBar<Integer>();
		this->mSqlConnector = SqlConnector;
	}
	ScoreCalculator(Real question_ratio, Real answer_ratio, Integer numOfDocs, SqliteConnector* SqlConnector)
		:QUESTION_RATIO(question_ratio), ANSWER_RATIO(answer_ratio), mNumOfDocs(numOfDocs)
	{
		this->mProgressBar	= new ProgressBar<Integer>();
		this->mSqlConnector = SqlConnector;
	}

	virtual ~ScoreCalculator()
	{
		delete this->mProgressBar;
	}

public:
	virtual void beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result, const double synonym = 0.0, const double levenshtein = 0.0) = 0;
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, const double synonym = 0.0, const double levenshtein = 0.0) = 0;

	std::vector< String> getSynonymFromMemory( int wordid) {
		std::vector< String> return_vec;
		for( int n = 0 ; n < mSynonymTable.size() ; n++) {
			if( wordid == mSynonymTable[ n].getWordid())
				return_vec.push_back( mSynonymTable[ n].getTerm());
		}

		return return_vec;
	}

	std::vector< String> getSynonymFromMemory( const String& str) {
		std::vector< String> return_vec;

		int wordid = -1;
		for( int n = 0 ; n < mSynonymTable.size() ; n++) {
			if( str.compare( mSynonymTable[ n].getTerm()) == 0) {
				wordid = mSynonymTable[ n].getWordid();
				break;
			}
		}

		if( wordid != -1) {
			for( int n = 0 ; n < mSynonymTable.size() ; n++) {
				if( wordid == mSynonymTable[ n].getWordid())
					return_vec.push_back( mSynonymTable[ n].getTerm());
			}
		}
		return return_vec;
	}

	Integer getWORDID_FromMemory( const String& str) {
	
		for( auto iter = mWordIDTable.begin() ; iter != mWordIDTable.end() ; iter++) {
			if( iter->second.compare( str) == 0)
				return iter->first;
		}
		return -1;
	}
};


#endif