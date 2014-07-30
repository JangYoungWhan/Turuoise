#ifndef _RAPID_XML_PARSER_H_
#define _RAPID_XML_PARSER_H_


#include <iostream>
#include "StdRedef.h"
#include "../xml_parser\rapidxml.hpp"
#include "../xml_parser\rapidxml_iterators.hpp"
#include "../xml_parser\rapidxml_utils.hpp"


class RapidXmlParser
{
private:
	rapidxml::file<>			*mXmlFile;//( training_file_path.c_str());
	rapidxml::xml_document<>	*mDoc;

	char*						mQuestionColumn;
	char*						mAnswerColumn;

private:
	bool isValidDataTag() const;
	bool isValidQuestionTag(rapidxml::xml_node<char> *node) const;
	bool isValidAnswerTag(rapidxml::xml_node<char> *node) const;

public:
	RapidXmlParser();
	RapidXmlParser(String &training_file_path);
	virtual ~RapidXmlParser();

	bool runParsing();
	char* getQuestionColumn() const;
	char* getAnswerColumn() const;
};


#endif