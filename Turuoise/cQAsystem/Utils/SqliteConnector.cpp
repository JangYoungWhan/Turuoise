#include "SqliteConnector.h"


SqliteConnector::SqliteConnector()
{

}

SqliteConnector::SqliteConnector(String &db_name)
{
	this->mDbName			= db_name;
}

SqliteConnector::~SqliteConnector()
{
	sqlite3_exec(mSqliteDB, "END TRANSACTION;", NULL, NULL, NULL);
	sqlite3_close(mSqliteDB);
}

bool SqliteConnector::isDbAccessible()
{
	if( _access( mDbName.c_str(), 0) == 0)
		return true;
	else
		return false;
}

bool SqliteConnector::createDB()
{
	char *sql;

	if( sqlite3_open( mDbName.c_str(), &mSqliteDB) != 0)
		return false;

	sql =	"CREATE TABLE WORD_ID( "	\
			"WORDID	INT	PRIMARY	KEY		NOT	NULL, "	\
			"NAME	TEXT				NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;


	sql =	"CREATE TABLE QUESTION_TF( "	\
			"DOCID		INT	NOT	NULL, "	\
			"WORDID		INT	NOT	NULL, "	\
			"FREQUENCY	INT	NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;


	sql =	"CREATE TABLE ANSWER_TF( "	\
			"DOCID		INT	NOT	NULL, "	\
			"WORDID		INT	NOT	NULL, "	\
			"FREQUENCY	INT	NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;


	sql =	"CREATE TABLE QUESTION_DF( "	\
			"WORDID		INT	PRIMARY	KEY		NOT	NULL, "	\
			"FREQUENCY	INT					NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;


	sql =	"CREATE TABLE ANSWER_DF( "	\
			"WORDID		INT	PRIMARY	KEY		NOT	NULL, "	\
			"FREQUENCY	INT					NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;
	
	return true;
}

void SqliteConnector::setDbConfig()
{
	sqlite3_exec(mSqliteDB, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	sqlite3_exec(mSqliteDB, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
	sqlite3_exec(mSqliteDB, "PRAGMA journal_mode=OFF", NULL, NULL, NULL);
	sqlite3_exec(mSqliteDB, "PRAGMA locking_mode=exclusive", NULL, NULL, NULL);
	sqlite3_exec(mSqliteDB, "PRAGMA temp_store=OFF", NULL, NULL, NULL);
}

bool SqliteConnector::makeDB()
{
	if(createDB()) {
		setDbConfig();
		return true;
	}
	else
		return false;
}


bool SqliteConnector::openDB()
{
	if( sqlite3_open( mDbName.c_str(), &mSqliteDB) == 0) {
		setDbConfig();
		return true;
	}
	else
		return false;
}

bool SqliteConnector::initDB()
{
	if(!isDbAccessible())
	{
		if(makeDB())
			return true;
		else
		{
			std::cerr << ">> db 생성 실패, " << sqlite3_errmsg( mSqliteDB) << std::endl;
			return false;
		}
	}
	else
	{
		if(openDB())
			return true;
		else
		{
			std::cerr << ">> db 열기 실패, " << sqlite3_errmsg( mSqliteDB) << std::endl;
			return false;
		}
	}
}

bool SqliteConnector::updateDB(const std::forward_list<Term<String, Integer>>* words, int flag){

	String sql;
	std::vector< std::vector<String>> result;

	std::map< Integer, Integer> map_id_freqeuncy;
	for( auto iter = words->begin() ; iter != words->end(); iter++) {
		//std::cout << *iter << std::endl;
		
		sql = "SELECT WORDID FROM WORD_ID WHERE NAME='";
		sql += ANSIToUTF8( iter->getTerm().c_str());
		sql += "'";
		result = queryDB(sql.c_str());
		if( result.size() == 0) {
			sql = "SELECT Count( WORDID) FROM WORD_ID";
			result = queryDB(sql.c_str());
		
			sql = "INSERT INTO WORD_ID VALUES( " + result[0].at( 0) + ", '";
			sql += ANSIToUTF8( iter->getTerm().c_str());
			sql += "')";
			queryDB(sql.c_str());
		}

		if( map_id_freqeuncy.find( atoi( result[0].at( 0).c_str())) == map_id_freqeuncy.end()) {
			map_id_freqeuncy.insert( std::make_pair( atoi( result[0].at( 0).c_str()), iter->getTermFreq()));
		}
		else {
			map_id_freqeuncy[ atoi( result[0].at( 0).c_str())]++;
		}
	}


	sql = "SELECT COUNT( DOCID) FROM ";
	sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
	result = queryDB(sql.c_str());
	int doc_number;
	if( atoi( result[ 0].at( 0).c_str()) == 0)
		doc_number = 0;
	else {
		sql = "SELECT MAX( DOCID) FROM ";
		sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
		result = queryDB(sql.c_str());
		doc_number = atoi( result[ 0].at( 0).c_str()) + 1;
	}

	for( auto iter = map_id_freqeuncy.begin() ; iter != map_id_freqeuncy.end() ; iter++) {
		sql = "SELECT * FROM ";
		sql += ( flag == QUESTION)?	"QUESTION_DF" : "ANSWER_DF";
		sql +=	" WHERE WORDID=";
		sql += std::to_string( iter->first);

		result = queryDB(sql.c_str());
		if( result.size() == 0) {
			sql = "INSERT INTO ";
			sql += ( flag == QUESTION)?	"QUESTION_DF" : "ANSWER_DF";
			sql += " VALUES( " + std::to_string( iter->first) + ", 1)";
			queryDB(sql.c_str());
		} else {
			sql = "UPDATE ";
			sql += ( flag == QUESTION)?	"QUESTION_DF" : "ANSWER_DF";
			sql += " SET FREQUENCY=";
			sql += std::to_string( atoi( result[0].at(1).c_str()) + 1);
			sql +=	" WHERE WORDID=";
			sql += std::to_string( iter->first);
			queryDB(sql.c_str());
		}

				
		sql = "INSERT INTO ";
		sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
		sql += " VALUES( " + std::to_string( doc_number) + ", " + std::to_string( iter->first) + ", " + std::to_string( iter->second) + ")";
		queryDB(sql.c_str());

	}
	return true;
}

bool SqliteConnector::updateDB(const std::set<Term<String, Integer>>* words, int flag){

	String sql;
	std::vector< std::vector<String>> result;

	std::map< Integer, Integer> map_id_freqeuncy;
	for( auto iter = words->begin() ; iter != words->end(); iter++) {
		//std::cout << *iter << std::endl;
		
		sql = "SELECT WORDID FROM WORD_ID WHERE NAME='";
		sql += ANSIToUTF8( iter->getTerm().c_str());
		sql += "'";
		result = queryDB(sql.c_str());
		if( result.size() == 0) {
			sql = "SELECT Count( WORDID) FROM WORD_ID";
			result = queryDB(sql.c_str());
		
			sql = "INSERT INTO WORD_ID VALUES( " + result[0].at( 0) + ", '";
			sql += ANSIToUTF8( iter->getTerm().c_str());
			sql += "')";
			queryDB(sql.c_str());
		}

		if( map_id_freqeuncy.find( atoi( result[0].at( 0).c_str())) == map_id_freqeuncy.end()) {
			map_id_freqeuncy.insert( std::make_pair( atoi( result[0].at( 0).c_str()), iter->getTermFreq()));
		}
		else {
			map_id_freqeuncy[ atoi( result[0].at( 0).c_str())]++;
		}
	}


	sql = "SELECT COUNT( DOCID) FROM ";
	sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
	result = queryDB(sql.c_str());
	int doc_number;
	if( atoi( result[ 0].at( 0).c_str()) == 0)
		doc_number = 0;
	else {
		sql = "SELECT MAX( DOCID) FROM ";
		sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
		result = queryDB(sql.c_str());
		doc_number = atoi( result[ 0].at( 0).c_str()) + 1;
	}

	for( auto iter = map_id_freqeuncy.begin() ; iter != map_id_freqeuncy.end() ; iter++) {
		sql = "SELECT * FROM ";
		sql += ( flag == QUESTION)?	"QUESTION_DF" : "ANSWER_DF";
		sql +=	" WHERE WORDID=";
		sql += std::to_string( iter->first);

		result = queryDB(sql.c_str());
		if( result.size() == 0) {
			sql = "INSERT INTO ";
			sql += ( flag == QUESTION)?	"QUESTION_DF" : "ANSWER_DF";
			sql += " VALUES( " + std::to_string( iter->first) + ", 1)";
			queryDB(sql.c_str());
		} else {
			sql = "UPDATE ";
			sql += ( flag == QUESTION)?	"QUESTION_DF" : "ANSWER_DF";
			sql += " SET FREQUENCY=";
			sql += std::to_string( atoi( result[0].at(1).c_str()) + 1);
			sql +=	" WHERE WORDID=";
			sql += std::to_string( iter->first);
			queryDB(sql.c_str());
		}

				
		sql = "INSERT INTO ";
		sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
		sql += " VALUES( " + std::to_string( doc_number) + ", " + std::to_string( iter->first) + ", " + std::to_string( iter->second) + ")";
		queryDB(sql.c_str());

	}
	return true;
}

bool SqliteConnector::closeDB() {
	sqlite3_close(mSqliteDB);
	return true;
}

Integer SqliteConnector::getWordID(const String &term)
{
	String  sql;
	sql = "SELECT WORDID FROM WORD_ID WHERE NAME='";
	sql += ANSIToUTF8( term.c_str());
	sql += "'";

	std::vector< std::vector< String>> result = queryDB(sql.c_str());
	if( result.size() == 0) 
		return -1;
	else
		return atoi( result[ 0].at( 0).c_str());
}


Integer SqliteConnector::getTF(const String &term, Integer doc, int flag)
{
	String  sql;
	sql = "SELECT FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_TF ON WORD_ID.WORDID = QUESTION_TF.WORDID" : "INNER JOIN ANSWER_TF ON WORD_ID.WORDID = ANSWER_TF.WORDID";
	sql += " WHERE NAME='";
	sql += ANSIToUTF8( term.c_str());
	sql += "' AND DOCID=" + std::to_string( doc) +"";

	std::vector< std::vector< String>> result = queryDB(sql.c_str());
	if( result.size() == 0) 
		return -1;
	else
		return atoi( result[ 0].at( 0).c_str());
}


Integer SqliteConnector::getDF(const String &term, int flag) {
	
	String  sql;
	sql = "SELECT FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_DF ON WORD_ID.WORDID = QUESTION_DF.WORDID" : "INNER JOIN ANSWER_DF ON WORD_ID.WORDID = ANSWER_DF.WORDID";
	sql += " WHERE NAME='";
	sql += ANSIToUTF8( term.c_str());
	sql += "'";

	std::vector< std::vector< String>> result = queryDB(sql.c_str());
	if( result.size() == 0) 
		return -1;
	else
		return atoi( result[ 0].at( 0).c_str());
}


Real SqliteConnector::getIDF(const String &term, int flag) {
	
	String  sql;
	sql = "SELECT FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_DF ON WORD_ID.WORDID = QUESTION_DF.WORDID" : "INNER JOIN ANSWER_DF ON WORD_ID.WORDID = ANSWER_DF.WORDID";
	sql += " WHERE NAME='";
	sql += ANSIToUTF8( term.c_str());
	sql += "'";

	std::vector< std::vector< String>> result = queryDB(sql.c_str());
	if( result.size() == 0) 
		return -1;
	else {
		int DF = atoi( result[ 0].at( 0).c_str());

		sql = "SELECT SUM(FREQUENCY) FROM ";
		sql += ( flag == QUESTION)?	"QUESTION_DF" : "ANSWER_DF";
		result = queryDB(sql.c_str());
		
		int sum_frequency = atoi( result[ 0].at( 0).c_str());
		return DF / (double)sum_frequency;
	}
}

Integer SqliteConnector::getCountWordID(){
	String sql;
	std::vector< std::vector<String>> result;

	sql = "SELECT Count( WORDID) FROM WORD_ID";
	result = queryDB(sql.c_str());

	if( result.size() == 0)
		return 0;
	else 
		return atoi( result[ 0].at( 0).c_str());
}

Integer SqliteConnector::getSumTermFreq(){
	String sql;
	std::vector< std::vector<String>> result;
	int frequency_sum;

	sql = "SELECT SUM( FREQUENCY) FROM QUESTION_TF";
	result = queryDB(sql.c_str());

	if( result.size() == 0)
		frequency_sum = 0;
	else 
		frequency_sum = atoi( result[ 0].at( 0).c_str());

	sql = "SELECT SUM( FREQUENCY) FROM ANSWER_TF";
	result = queryDB(sql.c_str());

	if( result.size() == 0)
		return frequency_sum;
	else 
		return frequency_sum + atoi( result[ 0].at( 0).c_str());
}

std::forward_list<Term<String, Integer>>* SqliteConnector::getDocInfoFlist(Integer doc_id, int flag){
	
	String sql;
	std::vector< std::vector< String>> result;
	sql = "SELECT NAME, FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_TF ON WORD_ID.WORDID = QUESTION_TF.WORDID" : "INNER JOIN ANSWER_TF ON WORD_ID.WORDID = ANSWER_TF.WORDID";
	sql += " WHERE DOCID='";
	sql += std::to_string( doc_id);
	sql += "'";
	
	result = queryDB( sql.c_str());

	std::forward_list<Term<String, Integer>> *vec_Term = new std::forward_list<Term<String, Integer>>;
	for( auto n1 = 0 ; n1 < result.size() ; n1++) {

		Term< String, Integer> term;
		term.setTerm( UTF8ToANSI( result[ n1].at( 0).c_str()));
		term.setTermFreq( atoi( result[ n1].at( 1).c_str()));
		term.setScore( 0);

		vec_Term->push_front( term);
	}
	
	return vec_Term;
}

std::set<Term<String, Integer>>* SqliteConnector::getDocInfoSet(Integer doc_id, int flag){
	
	String sql;
	std::vector< std::vector< String>> result;
	sql = "SELECT NAME, FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_TF ON WORD_ID.WORDID = QUESTION_TF.WORDID" : "INNER JOIN ANSWER_TF ON WORD_ID.WORDID = ANSWER_TF.WORDID";
	sql += " WHERE DOCID='";
	sql += std::to_string( doc_id);
	sql += "'";
	
	result = queryDB( sql.c_str());

	std::set<Term<String, Integer>> *set_Term = new std::set<Term<String, Integer>>;
	for( auto n1 = 0 ; n1 < result.size() ; n1++) {

		Term< String, Integer> term;
		term.setTerm( UTF8ToANSI( result[ n1].at( 0).c_str()));
		term.setTermFreq( atoi( result[ n1].at( 1).c_str()));
		term.setScore( 0);

		set_Term->insert( term);
	}
	
	return set_Term;
}

std::map<String, FreqScore<Integer, Integer>>* SqliteConnector::getDocInfoMap(Integer doc_id, int flag){
	
	String sql;
	std::vector< std::vector< String>> result;
	sql = "SELECT NAME, FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_TF ON WORD_ID.WORDID = QUESTION_TF.WORDID" : "INNER JOIN ANSWER_TF ON WORD_ID.WORDID = ANSWER_TF.WORDID";
	sql += " WHERE DOCID='";
	sql += std::to_string( doc_id);
	sql += "'";
	
	result = queryDB( sql.c_str());

	std::map<String, FreqScore<Integer, Integer>> *map_Term = new std::map<String, FreqScore<Integer, Integer>>();
	for( auto n1 = 0 ; n1 < result.size() ; n1++) {

		FreqScore<Integer, Integer> freq_score(atoi( result[ n1].at( 1).c_str()), 0);
		std::pair<String, FreqScore<Integer, Integer>> term_pair(UTF8ToANSI( result[ n1].at( 0).c_str()), freq_score);

		map_Term->insert( term_pair);
	}
	
	return map_Term;
}
//
std::vector< std::vector< String>> SqliteConnector::queryDB(const char* query)
{
    sqlite3_stmt *statement;
    std::vector< std::vector< String>> results;
 
    if(sqlite3_prepare_v2( mSqliteDB, query, -1, &statement, 0) == SQLITE_OK) {
        int cols = sqlite3_column_count( statement);
        int result = 0;
        while( true) {
            result = sqlite3_step( statement);
             
            if( result == SQLITE_ROW) {
                std::vector< String> values;
                for(int col = 0; col < cols; col++) {
                    values.push_back((char*)sqlite3_column_text( statement, col));
                }
                results.push_back( values);
            }
            else {
                break;   
            }
        }
        
        sqlite3_finalize( statement);
    }
     
    String error = sqlite3_errmsg( mSqliteDB);
    if( error != "not an error") std::cout << query << " " << error << std::endl;
     
    return results;  
}

char* SqliteConnector::UTF8ToANSI( const char *pszCode)
{
	BSTR    bstrWide;
	char*   pszAnsi;
	int     nLength;
	
	nLength = MultiByteToWideChar( CP_UTF8, 0, pszCode, strlen( pszCode) + 1, NULL, NULL);
	nLength = MultiByteToWideChar( CP_UTF8, 0, pszCode, strlen( pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen( NULL, nLength);

	MultiByteToWideChar( CP_UTF8, 0, pszCode, strlen( pszCode) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte( CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[ nLength];

	WideCharToMultiByte( CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString( bstrWide);

	return pszAnsi;
}

char* SqliteConnector::ANSIToUTF8( const char * pszCode)
{
	int	nLength, nLength2;
	BSTR bstrCode; 
	char *pszUTFCode = NULL;

	nLength = MultiByteToWideChar( CP_ACP, 0, pszCode, strlen( pszCode), NULL, NULL); 
	bstrCode = SysAllocStringLen( NULL, nLength); 
	MultiByteToWideChar( CP_ACP, 0, pszCode, strlen (pszCode), bstrCode, nLength);


	nLength2 = WideCharToMultiByte( CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL); 
	pszUTFCode = (char*)malloc( nLength2+1); 
	WideCharToMultiByte( CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2, NULL, NULL); 

	return pszUTFCode;
}

bool SqliteConnector::hasEnding(String const &fullString, String const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}