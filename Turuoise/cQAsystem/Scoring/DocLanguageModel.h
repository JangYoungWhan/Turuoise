#ifndef _DOCUMENT_LANGUAGE_MODEL_H_
#define _DOCUMENT_LANGUAGE_MODEL_H_


#include "ScoreCalculator.h"


class DocLanguageModel : public ScoreCalculator
{
private:
	const Real MLE_PARAM;
	const Real LAMBDA_JELINECK_MERCER;

	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInQuestion;
	std::map<String, FreqScore<Integer, Integer>>	*mSetDocInfoInAnswer;
	std::map<Integer, Integer> mUnigramCollection;
	std::map<std::pair<Integer, Integer>, Integer> mBigramCollection;

private:
	Integer setTotalUnigramsFreq(const std::map<Integer, Integer> &unigrams);
	Integer setTotalBigramsFreq(const std::map<std::pair<Integer, Integer>, Integer> &bigrams);
	Real calProbUnigram(const Integer c_unigram, const Integer c_total_unigram) const;
	Real calProbBigram(const Integer c_bigram, const Integer c_total_bigram) const;
	Real calProbNgram(const Integer c_ngram, const Integer c_total_ngram) const;
	Real make_qMLE(const Real p_bigram, const Real p_unigram) const;
	Real smoothJelinekMercer(const Real p_bigram, const Real p_unigram) const;
	Real calProWordGivenDoc(const Real p_bigram, const Real p_unigram, const Real p_total_bigram, const Real p_total_unigram) const;
	Real calDomainModel(Integer w1, Integer w2, const std::map<Integer, Integer> &unigram_table, const std::map<std::pair<Integer, Integer>, Integer> &bigram_table) const;

	// related to collection
	Integer count_unigram(Integer word_id) const;
	Integer count_unigram(Integer left_word_id, Integer right_word_id) const;
	Real p_unigram_collection(Integer c_unigram_collection, Integer c_total_unigram_collection) const;
	Real p_bigram_collection(Integer c_bigram_collection, Integer c_total_bigram_collection) const;
	Real q_MLE_collection(Real p_bigram_collection, Real p_unigram_collection) const;
	

public:
	DocLanguageModel();
	DocLanguageModel(Integer numOfDoc, SqliteConnector* SqlConnector);
	DocLanguageModel(Real question_ratio, Real answer_ratio, Integer numOfDoc, SqliteConnector* SqlConnector);
	virtual ~DocLanguageModel();

public:
	virtual void beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result, const double synonym = 0.0, const double levenshtein = 0.0);
	virtual void beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result, const double synonym = 0.0, const double levenshtein = 0.0);
};


#endif