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
	Term<T_str, T_int>& operator=(const Term& rhs);
	bool operator<(const Term<T_str, T_int> &rhs) const;
	bool operator>(const Term<T_str, T_int> &rhs) const;
	bool operator==(const Term<T_str, T_int> &rhs) const;
	bool operator!=(const Term<T_str, T_int> &rhs) const;

public:
	const T_str& getTerm() const		{	return this->mTerm;			};
	const T_int& getTermFreq() const	{	return this->mTermFreq;		};
	const int getScore() const			{	return this->mScore;		};

	void setTerm(const T_str& term)		{	this->mTerm		= term;		};
	void setTermFreq(const T_int& tf)	{	this->mTermFreq	= tf;		};
	void setScore(const int score)		{	this->mScore	= score;	};
};


template <typename T_str, typename T_int>
Term<T_str, T_int>& Term<T_str, T_int>::operator=(const Term<T_str, T_int>& rhs)
{
	this->mTerm		= rhs.mTerm;
	this->mTermFreq	= rhs.mTermFreq;
	this->mScore	= rhs.mScore;

	return *this;
}

template <typename T_str, typename T_int>
bool Term<T_str, T_int>::operator<(const Term<T_str, T_int> &rhs) const
{
	if(this->mTerm < rhs.mTerm)
		return true;
	else
		return false;
}

template <typename T_str, typename T_int>
bool Term<T_str, T_int>::operator>(const Term<T_str, T_int> &rhs) const
{
	if(this->mTerm > rhs.mTerm)
		return true;
	else
		return false;
}

template <typename T_str, typename T_int>
bool Term<T_str, T_int>::operator==(const Term<T_str, T_int> &rhs) const
{
	if(this->mTerm == rhs.mTerm)
		return true;
	else
		return false;
}

template <typename T_str, typename T_int>
bool Term<T_str, T_int>::operator!=(const Term<T_str, T_int> &rhs) const
{
	if(*this == rhs)
		return false;
	else
		return true;
}


#endif