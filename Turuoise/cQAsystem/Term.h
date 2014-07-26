#ifndef _TERM_H_
#define _TERM_H_


template <typename T_str, typename T_int>
class Term
{
protected:
	T_str		mTerm;
	T_int		mTermFreq;
	int			mScore;

public:
	const T_str& getTerm() const		{	return this->mTerm;			};
	const T_int& getTermFreq() const	{	return this->mTermFreq;		};
	const int getScore() const			{	return this->mScore;		};

	void setTerm(const T_str& term)		{	this->mTerm		= term;		};
	void setTermFreq(const T_int& tf)	{	this->mTermFreq	= tf;		};
	void setScore(const int score)		{	this->mScore	= score;	};
};


#endif