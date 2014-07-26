#include "RapidXmlParser.h"

RapidXmlParser::RapidXmlParser()
{

}

RapidXmlParser::RapidXmlParser(String &training_file_path)
{
	this->mXmlFile	= new rapidxml::file<>(training_file_path.c_str());
	this->mDoc		= new rapidxml::xml_document<>();
}

RapidXmlParser::~RapidXmlParser()
{
	delete mXmlFile;
	delete mDoc;
}

bool RapidXmlParser::isValidDataTag() const
{
	if( mDoc->first_node("data") == NULL) {
		std::cerr << ">> 잘못된 xml형식 (DATA_TAG)" << std::endl;
		return false;
	}

	return true;
}

bool RapidXmlParser::isValidQuestionTag(rapidxml::xml_node<char> *node) const
{
	if( node->first_node("question") == NULL) {
		std::cerr << ">> 잘못된 xml형식 (QUESTION_TAG)" << std::endl;
		return false;
	}

	return true;
}

bool RapidXmlParser::isValidAnswerTag(rapidxml::xml_node<char> *node) const
{
	if( node->first_node("answer") == NULL) {
		std::cerr << ">> 잘못된 xml형식 (ANSWER_TAG)" << std::endl;
		return false;
	}

	return true;
}

bool RapidXmlParser::runParsing()
{
	mDoc->parse<0>(mXmlFile->data());

	rapidxml::xml_node<char> *question_node;
	rapidxml::xml_node<char> *answer_node;

	if(!isValidDataTag())
	{
		return false;
	}
	else
	{
		rapidxml::xml_node<char> *data_node = mDoc->first_node("data");

		if(!isValidQuestionTag(data_node) || !isValidAnswerTag(data_node))
			return false;
		else
		{
			question_node	= data_node->first_node("question");
			answer_node		= data_node->first_node("answer");
		}
	}
	
	mQuestionColumn		= question_node->value();
	mAnswerColumn		= answer_node->value();
	//mQuestionColumn	= UTF8ToANSI( question_node->value());
	//mAnswerColumn	= UTF8ToANSI( answer_node->value());
	/*
	tool->pLearner->extractFromStr( UTF8ToANSI( question_node->value()));
	const std::list<std::string>* extractWords = tool->pLearner->getExtractReuslt();

	if( updateDB( tool->db, extractWords, QUESTION) == false) {
		std::cout << ">> db 업데이트 실패, " << sqlite3_errmsg( tool->db) << std::endl;
		return false;
	}
	

	tool->pLearner->initList();
	tool->pLearner->extractFromStr( UTF8ToANSI( answer_node->value()));
	extractWords = tool->pLearner->getExtractReuslt();
	
	if( updateDB( tool->db, extractWords, ANSWER) == false) {
		std::cout << ">> db 업데이트 실패, " << sqlite3_errmsg( tool->db) << std::endl;
		return false;
	}*/

	return true;
}

char* RapidXmlParser::getQuestionColumn() const
{
	return this->mQuestionColumn;
}

char* RapidXmlParser::getAnswerColumn() const
{
	return this->mAnswerColumn;
}
