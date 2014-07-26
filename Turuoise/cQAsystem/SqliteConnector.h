#ifndef _SQLITE_CONNECTOR_H_
#define _SQLITE_CONNECTOR_H_


#include <iostream>
#include <io.h>
#include <string>
#include <vector>
#include <Windows.h>
#include <forward_list>
#include <map>
#include "sqlite/sqlite3.h"
#include "StdRedef.h"
#include "Term.h"


class SqliteConnector
{
private:
	sqlite3 *mSqliteDB;
	String mDbName;

private:
	bool isDbAccessible();
	bool createDB();
	bool makeDB();

public:
	SqliteConnector();
	SqliteConnector(String &db_name);
	~SqliteConnector();

	bool openDB();
	bool initDB();
	bool updateDB( const std::forward_list<Term<String, Integer>>* words, int flag);
	bool closeDB();

public:
	Integer getWordID(String &term);
	Integer SqliteConnector::getTF(String &term, Integer doc, int flag);
	Integer getDF(String &term, int flag);
	Real getIDF(String &term, int flag);

private: // Utils
	std::vector< std::vector< String>> SqliteConnector::queryDB(const char* query);
	bool SqliteConnector::searchDirectory( String &folder_path, bool (*doSomething)( String, void *data), void *data);
	bool SqliteConnector::hasEnding(String const &fullString, String const &ending);

public: // Utils
	char* SqliteConnector::UTF8ToANSI( const char *pszCode);
	char* SqliteConnector::ANSIToUTF8( const char * pszCode);
	

};


#endif