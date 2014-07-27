#ifndef _DOC_INFO_H_
#define _DOC_INFO_H_


#include <iostream>
#include "StdRedef.h"


class DocInfo
{
private:
	Integer mDocID; // Q-A paired XML file name. It have to be integer value.
	Real	mScore; // Score of the file satisfied that query.

public:
	DocInfo();
	DocInfo(Integer doc_id, Real score);
	~DocInfo();

	Integer getDocID() const;
	Real getScore() const;

	bool operator<(const DocInfo &rhs) const;
	bool operator>(const DocInfo &rhs) const;

	friend std::ostream& operator<<(std::ostream& os, const DocInfo& doc);
};


#endif