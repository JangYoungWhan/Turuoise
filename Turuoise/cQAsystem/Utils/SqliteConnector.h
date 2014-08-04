#ifndef _SQLITE_CONNECTOR_H_
#define _SQLITE_CONNECTOR_H_


#include <iostream>
#include <io.h>
#include <string>
#include <vector>
#include <Windows.h>
#include <forward_list>
#include <map>
#include <set>
#include "../sqlite/sqlite3.h"
#include "StdRedef.h"
#include "Term.h"
#include "FreqScore.h"


class SqliteConnector
{
private:
	sqlite3 *mSqliteDB;
	String mDbName;

private:
	bool isDbAccessible();
	bool createDB();
	void setDbConfig();
	bool makeDB();	

public:
	SqliteConnector();
	SqliteConnector(String &db_name);
	~SqliteConnector();

	bool openDB();
	bool initDB();
	int SqliteConnector::getDocCount( );
	bool updateDB( std::string fname);
	bool updateDB( const std::forward_list<Term<String, Integer>>* words, int flag);
	bool updateDB( const std::set<Term<String, Integer>>* words, int strlen, int flag);
	bool SqliteConnector::delete_m1_DB( int flag);
	bool closeDB();

public:
	Integer getWordID(const String &term);
	Integer SqliteConnector::getTF(const String &term, Integer doc, int flag);
	Integer getDF(const String &term, int flag);
	Real getIDF(const String &term, int flag);
	Integer SqliteConnector::getCountWordID();
	Integer SqliteConnector::getSumTermFreq();
	std::forward_list<Term<String, Integer>>* getDocInfoFlist(Integer doc_id, int flag);
	std::vector<Term<String, Integer>> getDocInfoVector(Integer doc_id, int flag);
	std::set<Term<String, Integer>>* getDocInfoSet(Integer doc_id, int flag);
	std::map<String, FreqScore<Integer, Integer>>* getDocInfoMap(Integer doc_id, int flag);

private: // Utils
	std::vector< std::vector< String>> SqliteConnector::queryDB(const char* query);
	bool SqliteConnector::hasEnding(String const &fullString, String const &ending);

public: // Utils
	char* SqliteConnector::UTF8ToANSI( const char *pszCode);
	char* SqliteConnector::ANSIToUTF8( const char * pszCode);
	

};


#endif