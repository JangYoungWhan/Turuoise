#ifndef _COSINE_SIMILARITY_H_
#define _COSINE_SIMILARITY_H_


#include <iostream> // for naive debug!
#include "ScoreCalculator.h"


class CosineSimilarity : public ScoreCalculator
{
	virtual void beginScoring();
};


#endif