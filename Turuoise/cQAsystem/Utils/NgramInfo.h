#ifndef _NGRAM_INFO_H_
#define _NGRAM_INFO_H_


template <typename T_int, typename T_str>
class NgramInfo
{
private:
	T_int	mNgramN;
	T_str	*mGrams;
	T_int	mFreq;

public:
	NgramInfo();
	NgramInfo(T_int ngramN);
	virtual ~NgramInfo();

	void setUnigram(T_str word, T_int freq);
	void setBigram(T_str first_word, T_str second_word, T_int freq);
};


template <typename T_int, typename T_str>
NgramInfo<T_int, T_str>::NgramInfo()
	:mGrams(nullptr), mFreq(0)
{ }
template <typename T_int, typename T_str>
NgramInfo<T_int, T_str>::NgramInfo(T_int ngramN)
	:mFreq(0)
{
	mGrams = new T_str[ngramN];
}
template <typename T_int, typename T_str>
NgramInfo<T_int, T_str>::~NgramInfo()
{
	delete [] mGrams;
}

template <typename T_int, typename T_str>
void NgramInfo<T_int, T_str>::setUnigram(T_str word, T_int freq)
{
	this->mGrams[0]	= word;
	this->mFreq		= freq;
}

template <typename T_int, typename T_str>
void NgramInfo<T_int, T_str>::setBigram(T_str first_word, T_str second_word, T_int freq)
{
	this->mGrams[0]	= first_word;
	this->mGrams[1]	= second_word;
	this->mFreq		= freq;
}


#endif