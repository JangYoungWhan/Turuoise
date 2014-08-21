#include "DocLanguageModel.h"

DocLanguageModel::DocLanguageModel()
	:MLE_PARAM(0.7), LAMBDA_JELINECK_MERCER(0.7)
{ }

DocLanguageModel::DocLanguageModel(Integer numOfDoc, SqliteConnector* SqlConnector)
	:MLE_PARAM(0.5), LAMBDA_JELINECK_MERCER(0.7), ScoreCalculator(numOfDoc, SqlConnector)
{
	for(auto i=0; i<mNumOfDocs; i++)
	{
		std::map<Integer, Integer> unigram_table_q;
		std::map<std::pair<Integer, Integer>, Integer> bigram_table_q;
		std::map<Integer, Integer> unigram_table_a;
		std::map<std::pair<Integer, Integer>, Integer> bigram_table_a;

		// load ngram table.
		mSqlConnector->getUnigramTable(unigram_table_q, i, QUESTION);
		mSqlConnector->getBigramTable(bigram_table_q, i, QUESTION);
		mSqlConnector->getUnigramTable(unigram_table_a, i, ANSWER);
		mSqlConnector->getBigramTable(bigram_table_a, i, ANSWER);

		// sum of each total freqeuncies of ngram.
		setTotalUnigramsFreq(unigram_table_q);
		setTotalBigramsFreq(bigram_table_q);
		setTotalUnigramsFreq(unigram_table_a);
		setTotalBigramsFreq(bigram_table_a);
	}
}
DocLanguageModel::DocLanguageModel(Real question_ratio, Real answer_ratio, Integer numOfDoc, SqliteConnector* SqlConnector)
	:MLE_PARAM(0.5), LAMBDA_JELINECK_MERCER(0.7), ScoreCalculator(question_ratio, answer_ratio, numOfDoc, SqlConnector)
{
	for(auto i=0; i<mNumOfDocs; i++)
	{
		std::map<Integer, Integer> unigram_table_q;
		std::map<std::pair<Integer, Integer>, Integer> bigram_table_q;
		std::map<Integer, Integer> unigram_table_a;
		std::map<std::pair<Integer, Integer>, Integer> bigram_table_a;

		// load ngram table.
		mSqlConnector->getUnigramTable(unigram_table_q, i, QUESTION);
		mSqlConnector->getBigramTable(bigram_table_q, i, QUESTION);
		mSqlConnector->getUnigramTable(unigram_table_a, i, ANSWER);
		mSqlConnector->getBigramTable(bigram_table_a, i, ANSWER);

		// sum of each total freqeuncies of ngram.
		setTotalUnigramsFreq(unigram_table_q);
		setTotalBigramsFreq(bigram_table_q);
		setTotalUnigramsFreq(unigram_table_a);
		setTotalBigramsFreq(bigram_table_a);
	}
}

DocLanguageModel::~DocLanguageModel()
{ }

inline
Integer DocLanguageModel::setTotalUnigramsFreq(const std::map<Integer, Integer> &unigrams)
{
	Integer sum = 0;
	for(auto iter=unigrams.begin(); iter!=unigrams.end(); iter++)
	{
		auto find_result = mUnigramCollection.find(iter->first);

		if(find_result != mUnigramCollection.end())
			find_result->second += iter->second;
		else
			mUnigramCollection.insert(std::make_pair(iter->first, iter->second));

		sum += iter->second;
	}

	return sum;
}

inline
Integer DocLanguageModel::setTotalBigramsFreq(const std::map<std::pair<Integer, Integer>, Integer> &bigrams)
{
	Integer sum = 0;
	for(auto iter=bigrams.begin(); iter!=bigrams.end(); iter++)
	{
		auto find_result = mBigramCollection.find(iter->first);

		if(find_result != mBigramCollection.end())
			find_result->second += iter->second;
		else
			mBigramCollection.insert(std::make_pair(iter->first, iter->second));

		sum += iter->second;
	}

	return sum;
}

inline
Real DocLanguageModel::calProbUnigram(const Integer c_unigram, const Integer c_total_unigram) const
{
	return static_cast<Real>(c_unigram)/c_total_unigram;
}

inline
Real DocLanguageModel::calProbBigram(const Integer c_bigram, const Integer c_total_bigram) const
{
	return static_cast<Real>(c_bigram)/c_total_bigram;
}

inline
Real DocLanguageModel::calProbNgram(const Integer c_ngram, const Integer c_total_ngram) const
{
	return static_cast<Real>(c_ngram)/c_total_ngram;
}

Real DocLanguageModel::make_qMLE(const Real p_bigram, const Real p_unigram) const
{
	return (1.0-MLE_PARAM)*p_bigram+MLE_PARAM*p_unigram;
}

inline
Real DocLanguageModel::smoothJelinekMercer(const Real p_bigram, const Real p_unigram) const
{
	return (1.0-LAMBDA_JELINECK_MERCER)*make_qMLE(p_bigram, p_unigram);
}

inline
Real DocLanguageModel::calProWordGivenDoc(const Real p_bigram, const Real p_unigram, const Real p_total_bigram, const Real p_total_unigram) const
{
	auto p_smoothed = smoothJelinekMercer(p_bigram, p_unigram);
	auto df_penalty = LAMBDA_JELINECK_MERCER*make_qMLE(p_total_bigram, p_total_unigram);
	
	return log((p_smoothed/df_penalty)+M_E);
}

Real DocLanguageModel::calDomainModel(Integer w0, Integer w1, const std::map<Integer, Integer> &unigram_table, const std::map<std::pair<Integer, Integer>, Integer> &bigram_table) const
{
	Integer uni_freq = 0;
	Integer bi_freq = 0;
	auto find_result_uni = unigram_table.find(w0);
	auto find_result_bi = bigram_table.find(std::make_pair(w0, w1));
	if(find_result_uni != unigram_table.end())
		uni_freq = find_result_uni->second;
	if(find_result_bi != bigram_table.end())
		bi_freq = find_result_bi->second;

	Integer coll_uni_freq = 0;
	Integer coll_bi_freq = 0;
	auto find_result_coll_uni = mUnigramCollection.find(w0);
	auto find_result_coll_bi = mBigramCollection.find(std::make_pair(w0, w1));
	if(find_result_coll_uni != mUnigramCollection.end())
		coll_uni_freq = find_result_coll_uni->second;
	if(find_result_coll_bi != mBigramCollection.end())
		coll_bi_freq = find_result_coll_bi->second;

	auto p_uni = calProbNgram(uni_freq, unigram_table.size());
	auto p_bi = calProbNgram(bi_freq, bigram_table.size());
	auto p_coll_uni = calProbNgram(coll_uni_freq, mUnigramCollection.size());
	auto p_coll_bi = calProbNgram(coll_bi_freq, mBigramCollection.size());

	return calProWordGivenDoc(p_bi, p_uni, p_coll_uni, p_coll_bi);
}

void DocLanguageModel::beginScoring(std::forward_list<Term<String, Integer>> *query_result)
{
	// do nothing.
}

void DocLanguageModel::beginScoring(std::list<Integer> *query_result, std::vector<DocInfo>& score_result)
{
	std::cout << "DocLanguageModel::beginScoring" << std::endl;

	score_result.resize(mNumOfDocs);
	for(auto i=0; i<mNumOfDocs; i++)
	{
		mProgressBar->dispalyPrgressBar(i, mNumOfDocs-1);

		std::map<Integer, Integer> unigram_table_q;
		std::map<std::pair<Integer, Integer>, Integer> bigram_table_q;
		std::map<Integer, Integer> unigram_table_a;
		std::map<std::pair<Integer, Integer>, Integer> bigram_table_a;

		// load ngram table.
		mSqlConnector->getUnigramTable(unigram_table_q, i, QUESTION);
		mSqlConnector->getBigramTable(bigram_table_q, i, QUESTION);
		mSqlConnector->getUnigramTable(unigram_table_a, i, ANSWER);
		mSqlConnector->getBigramTable(bigram_table_a, i, ANSWER);

		// initialize document domain probability.
		Real que_prob = 0;
		Real ans_prob = 0;

		// try to make a query using by document language model.
		auto q1 = query_result->begin();
		auto q0 = q1++;
		while(q1 != query_result->end())
		{
			// calculate question area
			que_prob += calDomainModel(*q0, *q1, unigram_table_q, bigram_table_q);

			// calculate answer area
			ans_prob += calDomainModel(*q0, *q1, unigram_table_a, bigram_table_a);

			q0 = q1++;
		}
		
		DocInfo doc(i, que_prob*QUESTION_RATIO + ans_prob*ANSWER_RATIO);
		score_result[i] = doc;
	}

	std::cout << std::endl;
}

void DocLanguageModel::beginScoring(std::set<Term<String, Integer>> *query_result, std::vector<DocInfo>& score_result)
{
	// do nothing.
}
