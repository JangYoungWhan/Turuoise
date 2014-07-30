#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_


#include <iostream>
#include <iomanip>
#include "StdRedef.h"


template <typename T_int>
class ProgressBar
{
public:
	ProgressBar();
	virtual ~ProgressBar();
	void dispalyPrgressBar(T_int x, T_int n, int w = 50);
};

template <typename T_int>
ProgressBar<T_int>::ProgressBar()
{

}

template <typename T_int>
ProgressBar<T_int>::~ProgressBar()
{

}

template <typename T_int>
inline
void ProgressBar<T_int>::dispalyPrgressBar(T_int x, T_int n, int w = 50)
{
    if ( (x != n) && (x % (n/100+1) != 0) ) return;
 
    float ratio  =  x/(float)n;
    int   c      =  static_cast<int>(ratio*w);
 
    std::wcout << std::setw(3) << static_cast<int>(ratio*100) << "% [";
	for(auto x=0; x<w; x++)
	{
		if(x<c)			std::wcout << "=";
		else if(x==c)	std::wcout << ">";
		else			std::wcout << " ";
	}
    std::wcout << "]\r" << std::flush;
}

#endif