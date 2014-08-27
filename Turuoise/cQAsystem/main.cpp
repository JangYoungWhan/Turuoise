/*
 * Title	: CQA system implemented in C++
 * Author	: YW. Jang, JI. Jo
 */


#include <iostream>
#include <string>
#include <time.h>
#include <codecvt>
#include "QAsystem.h"
#include "CQAsystem.h"
#include "Utils/stringutil.h"
#include "Utils\MorphemeAnalyzer.hpp"

//#define _TRAINING_MODE_

struct QueryInput
{
	int qum;
	String query;
};

std::list<QueryInput>* readQueryXml(const String &query_xml_file)
{
	rapidxml::file<> xml_file(query_xml_file.c_str());
	rapidxml::xml_document<> xml_doc;

	xml_doc.parse<0>(xml_file.data());
	std::list<QueryInput>* pQueryInput = new std::list<QueryInput>();

	rapidxml::xml_node<char> *curNode = xml_doc.first_node("query");
	while(curNode != nullptr)
	{
		rapidxml::xml_node<char> *qnum_node = curNode->first_node("qnum");
		rapidxml::xml_node<char> *text_node = curNode->first_node("text");

		QueryInput query;
		query.qum	= atoi(convertFromUTF8ToANSI(qnum_node->value()).c_str());
		query.query = convertFromUTF8ToANSI(text_node->value());
		//std::cout << query.qum << std::endl << query.query << std::endl;
		curNode = curNode->next_sibling();

		pQueryInput->push_back(query);
	}

	return pQueryInput;
}

bool writeResultXml(int qnum, const std::vector<DocInfo> *cqa_result)
{
	const String QUERY_BEGIN_TAG		= "<query>";
	const String QUERY_END_TAG			= "</query>";
	const String QUERY_NUM_BEGIN_TAG	= "<qnum>";
	const String QUERY_NUM_END_TAG		= "</qnum>";
	const String RANK_BEGIN_TAG			= "<rank>";
	const String RANK_END_TAG			= "</rank>";

	std::ofstream outFile;
	outFile.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		outFile.open("result.xml", std::ios::app);
		if(outFile.fail())
			std::cerr << "Cannot make a file : result.xml" << std::endl;

		outFile << QUERY_BEGIN_TAG << QUERY_NUM_BEGIN_TAG << qnum << QUERY_NUM_END_TAG << std::endl;
		outFile << RANK_BEGIN_TAG << std::endl;
		for(auto iter=cqa_result->begin(); iter!=cqa_result->end(); iter++)
		{
			outFile << *iter << std::endl;
		}
		outFile << RANK_END_TAG << std::endl;
		outFile << QUERY_END_TAG << std::endl;
	}
	catch(const std::ifstream::failure& e)
	{
		std::cerr << "Exception is occured when reading a file!" << std::endl;
		std::cerr << "Exception: " << e.what() << std::endl;
		return false;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		return false;
	}
	catch(...)
	{
		std::cerr << "Undefined exception!" << std::endl;
		return false;
	}

	return true;
}


int main(int argc, char* argv[])
{
	std::cout << "=== Run Turuoise ===" << std::endl;

	String TRAINING_DATA_PATH = "../../../../training_data/";
	String TRAINING_DB_NAME = "Turuoise.db";
	String QUERY = "영어 super를 한글로 표기할 때 수퍼라고 해야 하나요, 슈퍼라고 해야 하나요?Superman, supermarket 등은 수퍼맨, 수퍼마켓인가요, 슈퍼맨, 슈퍼마켓인가요";
	Integer DISPLAY_LIMIT = 10;

	// 형태소 분석을 수행한다.
	/*
	MA::MorphemeAnalyzer analyzer;
	std::vector< MA::VECMorpheme> test = analyzer.Extract( "바람과 함께 사라지다.");
	*/

	/* 유의어추출을 위해 word.txt를 얻는부분임. 출력물은 word.txt
	std::cout << "=== 유의어추출을 위해 word.txt를 얻는부분임 ===" << std::endl;
	std::ofstream fout("word.txt");
	SqliteConnector *mSqliteConnector = new SqliteConnector( TRAINING_DB_NAME);
	mSqliteConnector->initDB();
	int wordnum = mSqliteConnector->getCountWordID();
	for( int i = 0 ; i < wordnum ; i++)
		fout << mSqliteConnector->getWord( i) << std::endl;
	fout.close();
	*/


	/*
	// 추출된 유의어 파일로 부터 db의 유의어 테이블에 추가함. 유의어가 저장된 파일은 synonym.txt
	SqliteConnector *mSqliteConnector = new SqliteConnector( TRAINING_DB_NAME);
	mSqliteConnector->initDB();
	std::cout << "=== db의 유의어 테이블을 업데이트하는 부분임 ===" << std::endl;
	std::ifstream fin("synonym.txt");
	char buf[ 2046];
	wchar_t wbuf[ 2046];
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
	
	mSqliteConnector->createSynonymTable();	// 유의어 테이블이 없을 경우 생성함.

	while( fin.getline( buf, 2046)) {		
		std::wstring wstr = mSqliteConnector->utf8_to_utf16( buf);
		wcsncpy( wbuf, wstr.c_str(), wstr.length() + 1);
		wchar_t* token = wcstok( wbuf, L" \t\n\r");

		std::string narrow = utfconv.to_bytes( token);     
		int word_id = mSqliteConnector->getWordID( mSqliteConnector->UTF8ToANSI( narrow.c_str()));
		if( word_id != -1 && token != NULL) {
			while( ( token = wcstok( NULL, L" \t\n\r")) != NULL)
				mSqliteConnector->updateSynonymTable( word_id, utfconv.to_bytes( token));
		}
	}
	*/

	
	QAsystem *pQAsystem = new CQAsystem(TRAINING_DB_NAME);

	#ifdef _TRAINING_MODE_
	clock_t start_time, end_time;
	start_time = clock();
	pQAsystem->beginTraning(TRAINING_DATA_PATH, false);
	end_time = clock();
	std::cout<< "Machine Learning Time : " << static_cast<Real>(end_time-start_time)/CLOCKS_PER_SEC << std::endl << std::endl;
	#else
	pQAsystem->beginTraning(TRAINING_DATA_PATH, true);
	#endif

	auto *qry_info = readQueryXml("query.xml");
	for(auto iter = qry_info->begin(); iter!=qry_info->end(); iter++)
	{
		pQAsystem->analyzeQuery(iter->query);
		pQAsystem->calculateScore();
		auto *result = pQAsystem->sortResult(DISPLAY_LIMIT);
		//pQAsystem->dispalyResult(DISPLAY_LIMIT);
		writeResultXml(iter->qum, result);
		delete result;
	}
	delete qry_info;

	delete pQAsystem;
	
	return 0;
}

