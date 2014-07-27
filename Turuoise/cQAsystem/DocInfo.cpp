#include "DocInfo.h"


DocInfo::DocInfo()
{ }
DocInfo::DocInfo(Integer doc_id, Real score)
	:mDocID(doc_id), mScore(score)
{ }
DocInfo::~DocInfo()
{ }

Integer DocInfo::getDocID() const
{
	return this->mDocID;
}

Real DocInfo::getScore() const
{
	return this->mScore;
}

bool DocInfo::operator<(const DocInfo &rhs) const
{
	if(fabs(this->mScore - rhs.getScore()) < EPSILON)
		return false;
	else
		return true;
}

bool DocInfo::operator>(const DocInfo &rhs) const
{
	if(fabs(this->mScore - rhs.getScore()) > EPSILON)
		return true;
	else
		return false;
}

std::ostream& operator<<(std::ostream& os, const DocInfo& doc)
{
	os << "DocID : " << doc.mDocID << "\tScore : " << doc.mScore;

	return os;
}