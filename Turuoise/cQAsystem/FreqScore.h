#ifndef _FREQ_SCORE_H_
#define _FREQ_SCORE_H_


template <typename T_freq, typename T_scor>
class FreqScore
{
private:
	T_freq	mTermFreq;
	T_scor	mScore;

public:
	FreqScore();
	FreqScore(T_freq term_freq, T_scor score);
	~FreqScore();

public:
	T_freq getTermFreq() const;
	T_scor getScore() const;
};


template <typename T_freq, typename T_scor>
FreqScore<T_freq, T_scor>::FreqScore()
{ }
template <typename T_freq, typename T_scor>
FreqScore<T_freq, T_scor>::FreqScore(T_freq term_freq, T_scor score)
	:mTermFreq(term_freq), mScore(score)
{ }
template <typename T_freq, typename T_scor>
FreqScore<T_freq, T_scor>::~FreqScore()
{ }

template <typename T_freq, typename T_scor>
T_freq FreqScore<T_freq, T_scor>::getTermFreq() const
{
	return this->mTermFreq;
}

template <typename T_freq, typename T_scor>
T_scor FreqScore<T_freq, T_scor>::getScore() const
{
	return this->mScore;
}


#endif