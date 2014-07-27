#ifndef _STD_REDEF_H
#define _STD_REDEF_H


#include <string>

typedef std::string String;
typedef int			Integer;
typedef double		Real;
//typedef float		Real;


// It is used for compare between real numbers
const Real EPSILON = 1e-6;


// Determine whether it is question or answer
enum { QUESTION, ANSWER};


// Natural number
const Real M_E				= 2.7182818284590452354;

const Real QUESTION_RATIO	= 0.7;
const Real ANSWER_RATIO		= 0.3;


#endif