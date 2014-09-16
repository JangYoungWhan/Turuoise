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


	sql =	"CREATE TABLE DOC_FILENAME( "	\
			"DOCID		INT	PRIMARY	KEY		NOT	NULL, "	\
			"FNAME		TEXT				NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	sql =	"CREATE TABLE QUESTION_DOC_STRLEN( "	\
			"DOCID		INT	PRIMARY	KEY		NOT	NULL, "	\
			"STRLEN		INT					NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;
	
	sql =	"CREATE TABLE ANSWER_DOC_STRLEN( "	\
			"DOCID		INT	PRIMARY	KEY		NOT	NULL, "	\
			"STRLEN		INT					NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	sql =	"CREATE TABLE SYNONYM( "	\
			"WORDID		INT		NOT	NULL, "	\
			"NAME		TEXT	NOT	NULL);";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	return true;
}

bool SqliteConnector::createNgramTables()
{
	char *sql;

	if( sqlite3_open( mDbName.c_str(), &mSqliteDB) != 0)
		return false;

	// make ngram table
	sql =	"CREATE TABLE NGRAM_WORD_ID( "	\
			"NAME		TEXT	NOT	NULL, "	\
			"WORDID		INT		NOT	NULL, "	\
			"PRIMARY KEY(NAME));";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	sql =	"CREATE TABLE UNIGRAM_QUESTION( "	\
			"DOCID		INT		NOT	NULL, "	\
			"WORDID		INT		NOT	NULL, "	\
			"FREQUENCY	INT		NOT	NULL, "	\
			"PRIMARY KEY(DOCID, WORDID));";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	sql =	"CREATE TABLE UNIGRAM_ANSWER( "	\
			"DOCID		INT		NOT	NULL, "	\
			"WORDID		INT		NOT	NULL, "	\
			"FREQUENCY	INT		NOT	NULL, "	\
			"PRIMARY KEY(DOCID, WORDID));";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	sql =	"CREATE TABLE BIGRAM_QUESTION( "	\
			"DOCID			INT		NOT	NULL, "	\
			"LEFTWORDID		INT		NOT	NULL, "	\
			"RIGHTWORDID	INT		NOT	NULL, "	\
			"FREQUENCY		INT		NOT	NULL, "	\
			"PRIMARY KEY(DOCID, LEFTWORDID, RIGHTWORDID));";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	sql =	"CREATE TABLE BIGRAM_ANSWER( "	\
			"DOCID			INT		NOT	NULL, "	\
			"LEFTWORDID		INT		NOT	NULL, "	\
			"RIGHTWORDID	INT		NOT	NULL, "	\
			"FREQUENCY		INT		NOT	NULL, "	\
			"PRIMARY KEY(DOCID, LEFTWORDID, RIGHTWORDID));";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	return true;
}

void SqliteConnector::setDbConfig()
{
	sqlite3_exec(mSqliteDB, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	sqlite3_exec(mSqliteDB, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
	sqlite3_exec(mSqliteDB, "PRAGMA journal_mode=MEMORY", NULL, NULL, NULL);
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

bool SqliteConnector::initExistsNgramTable()
{
	char *sql;

	if( sqlite3_open( mDbName.c_str(), &mSqliteDB) != 0)
		return false;

	sql =	"DROP TABLE NGRAM_WORD_ID;";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;
	sql =	"DROP TABLE UNIGRAM_QUESTION;";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;
	sql =	"DROP TABLE UNIGRAM_ANSWER;";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;
	sql =	"DROP TABLE BIGRAM_QUESTION;";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;
	sql =	"DROP TABLE BIGRAM_ANSWER;";
	if( sqlite3_exec( mSqliteDB, sql, 0, 0, 0) != SQLITE_OK )
		return false;

	return true;
}

bool SqliteConnector::initNgramTables()
{
	if(!isDbAccessible())
	{
		std::cerr << ">> DB 접근 실패, " << sqlite3_errmsg( mSqliteDB) << std::endl;
			return false;
	}
	else
	{
		if(openDB())
		{
			if(createNgramTables())
			{
				setDbConfig();
				return true;
			}
			else
				return false;
		}
		else
		{
			std::cerr << ">> Ngram Table 열기 실패, " << sqlite3_errmsg( mSqliteDB) << std::endl;
			return false;
		}
	}	
}

bool SqliteConnector::insertNgramStr2Int(std::map<String, Integer> &str2int)
{
	String sql;

	for(auto iter=str2int.begin(); iter!=str2int.end(); iter++)
	{
		auto word = ANSIToUTF8(iter->first.c_str());
		auto word_id = std::to_string(iter->second);

		sql = "INSERT INTO NGRAM_WORD_ID";
		sql += " VALUES( '" + word + "', " + word_id + ")";
		queryDB(sql.c_str());
	}

	return true;
}

bool SqliteConnector::insertUnigrams(Integer docID, std::map<Integer, Integer> *unigram, int flag)
{
	String sql;

	for(auto iter=unigram->begin(); iter!=unigram->end(); iter++)
	{
		auto doc_id = std::to_string(docID);
		auto word = std::to_string(iter->first);
		auto freq = std::to_string(iter->second);

		sql = "INSERT INTO ";
		sql += ( flag == QUESTION)?	"UNIGRAM_QUESTION" : "UNIGRAM_ANSWER";
		sql += " VALUES( " + doc_id + ", " + word + ", " + freq + ")";
		queryDB(sql.c_str());
	}

	return true;
}

bool SqliteConnector::insertBigrams(Integer docID, std::map<std::pair<Integer, Integer>, Integer> *bigram, int flag)
{
	String sql;

	for(auto iter=bigram->begin(); iter!=bigram->end(); iter++)
	{
		auto doc_id = std::to_string(docID);
		auto left_word = std::to_string(iter->first.first);
		auto right_word = std::to_string(iter->first.second);
		auto freq = std::to_string(iter->second);

		sql = "INSERT INTO ";
		sql += ( flag == QUESTION)?	"BIGRAM_QUESTION" : "BIGRAM_ANSWER";
		sql += " VALUES( " + doc_id + ", " + left_word + ", " + right_word + + ", " + freq + ")";
		queryDB(sql.c_str());
	}

	return true;
}

Integer SqliteConnector::getNgramWordID(const String &term)
{
	String  sql;
	sql = "SELECT WORDID FROM NGRAM_WORD_ID WHERE NAME='";
	sql += ANSIToUTF8( term.c_str());
	sql += "'";

	std::vector< std::vector< String>> result = queryDB(sql.c_str());
	if( result.size() == 0) 
		return -1;
	else
		return atoi( result[ 0].at( 0).c_str());
}

void SqliteConnector::getUnigramTable(std::map<Integer, Integer> &unigramTable, Integer docID, int flag)
{
	String sql;
	std::vector<std::vector<String>> result;

	sql = "SELECT WORDID, FREQUENCY FROM UNIGRAM_";
	sql += (flag == QUESTION)?	"QUESTION" : "ANSWER";
	sql += " WHERE DOCID='";
	sql += std::to_string(docID);
	sql += "'";
	
	result = queryDB( sql.c_str());

	for( auto i=0; i<result.size(); i++)
	{
		Integer word_id = atoi(result[i].at(0).c_str());
		Integer freq = atoi(result[i].at(1).c_str());
		unigramTable.insert(std::make_pair(word_id, freq));
	}
	unigramTable.insert(std::make_pair(getNgramWordID(SENTENCE_TAG), 2)); // insert a pair that has word id of <s> and 2 frequencies.
}

void SqliteConnector::getBigramTable(std::map<std::pair<Integer, Integer>, Integer> &bigramTable, Integer docID, int flag)
{
	String sql;
	std::vector<std::vector<String>> result;

	sql = "SELECT LEFTWORDID, RIGHTWORDID, FREQUENCY FROM BIGRAM_";
	sql += (flag == QUESTION)?	"QUESTION" : "ANSWER";
	sql += " WHERE DOCID='";
	sql += std::to_string(docID);
	sql += "'";
	
	result = queryDB( sql.c_str());

	for( auto i=0; i<result.size(); i++)
	{
		Integer left_word_id = atoi(result[i].at(0).c_str());
		Integer right_word_id = atoi(result[i].at(1).c_str());
		Integer freq = atoi(result[i].at(2).c_str());
		
		bigramTable.insert(std::make_pair(std::make_pair(left_word_id, right_word_id), freq));
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

int SqliteConnector::getDocCount( ){
	String sql;
	std::vector< std::vector<String>> result;

	sql = "SELECT COUNT( DOCID) FROM DOC_FILENAME";
	result = queryDB(sql.c_str());
	int doc_number;
	if( atoi( result[ 0].at( 0).c_str()) == 0)
		doc_number = 0;
	else {
		sql = "SELECT MAX( DOCID) FROM DOC_FILENAME";
		result = queryDB(sql.c_str());
		doc_number = atoi( result[ 0].at( 0).c_str()) + 1;
	}

	return doc_number;
}


bool SqliteConnector::updateDB( std::string fname){
	String sql;
	std::vector< std::vector<String>> result;

	int index = fname.find_last_of( "//");
	if( std::string::npos != index) {
		fname = fname.substr( index + 1);
	}

	sql = "SELECT COUNT( DOCID) FROM DOC_FILENAME";
	result = queryDB(sql.c_str());
	int doc_number;
	if( atoi( result[ 0].at( 0).c_str()) == 0)
		doc_number = 0;
	else {
		sql = "SELECT MAX( DOCID) FROM DOC_FILENAME";
		result = queryDB(sql.c_str());
		doc_number = atoi( result[ 0].at( 0).c_str()) + 1;
	}

	sql = "INSERT INTO DOC_FILENAME";
	sql += " VALUES( " + std::to_string( doc_number) + ", '"+ fname + "')";
	queryDB(sql.c_str());

	return true;
}


bool SqliteConnector::updateDB(const std::set<Term<String, Integer>>* words, int strlen, int flag){

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

	sql = "INSERT INTO ";
	sql += ( flag == QUESTION)?	"QUESTION_DOC_STRLEN" : "ANSWER_DOC_STRLEN";
	sql += " VALUES( " + std::to_string( doc_number) + ", " + std::to_string( strlen) + ")";
	queryDB(sql.c_str());

	if( map_id_freqeuncy.size() == 0) {
		sql = "INSERT INTO ";
		sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
		sql += " VALUES( " + std::to_string( doc_number) + ", " + std::to_string( -1) + ", " + std::to_string( -1) + ")";
		queryDB(sql.c_str());
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


bool SqliteConnector::updateSynonymTable( int term_id, String synonym) {
	
	String sql;

	sql = "INSERT INTO SYNONYM VALUES( " + std::to_string( term_id);
	sql += ", '";
	sql += synonym;
	sql += "')";
	queryDB(sql.c_str());

	return true;
}


bool SqliteConnector::delete_m1_DB( int flag){

	String sql;

	sql = "DELETE FROM ";
	sql += ( flag == QUESTION)?	"QUESTION_TF" : "ANSWER_TF";
	sql +=	" WHERE WORDID=";
	sql += std::to_string( -1);

	queryDB(sql.c_str());
	
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


String SqliteConnector::getWord( int word_id)
{
	String  sql;
	sql = "SELECT NAME FROM WORD_ID WHERE WORDID=";
	sql += std::to_string( word_id);

	std::vector< std::vector< String>> result = queryDB(sql.c_str());
	if( result.size() == 0) 
		return NULL;
	else
		return result[ 0].at( 0);
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


std::map< Integer, Integer> SqliteConnector::getALLDF( int flag) {
	
	String  sql;
	sql = "SELECT WORD_ID.WORDID, FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_DF ON WORD_ID.WORDID = QUESTION_DF.WORDID" : "INNER JOIN ANSWER_DF ON WORD_ID.WORDID = ANSWER_DF.WORDID";
	
	std::vector< std::vector< String>> result = queryDB(sql.c_str());

	std::map< Integer, Integer> return_map;
	
	for( int n = 0 ; n < result.size() ; n++)
		return_map.insert( std::pair< Integer, Integer>( atoi( result[ n].at( 0).c_str()), atoi( result[ n].at( 1).c_str())));

	return return_map;
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

std::vector<Term<String, Integer>> SqliteConnector::getDocInfoVector(Integer doc_id, int flag){
	
	String sql;
	std::vector< std::vector< String>> result;
	sql = "SELECT NAME, FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_TF ON WORD_ID.WORDID = QUESTION_TF.WORDID" : "INNER JOIN ANSWER_TF ON WORD_ID.WORDID = ANSWER_TF.WORDID";
	sql += " WHERE DOCID='";
	sql += std::to_string( doc_id);
	sql += "'";
	
	result = queryDB( sql.c_str());

	std::vector<Term<String, Integer>> vec_Term;
	for( auto n1 = 0 ; n1 < result.size() ; n1++) {

		Term< String, Integer> term;
		term.setTerm( UTF8ToANSI( result[ n1].at( 0).c_str()));
		term.setTermFreq( atoi( result[ n1].at( 1).c_str()));
		term.setScore( 0);

		vec_Term.push_back( term);
	}
	
	return vec_Term;
}


std::map< Integer, std::vector<Term<String, Integer>>> SqliteConnector::getALLDocInfoVector( int flag){
	
	String sql;
	std::vector< std::vector< String>> result;
	std::map< Integer, std::vector<Term<String, Integer>>> return_map;

	sql = "SELECT DOCID, NAME, FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_TF ON WORD_ID.WORDID = QUESTION_TF.WORDID" : "INNER JOIN ANSWER_TF ON WORD_ID.WORDID = ANSWER_TF.WORDID ORDER BY DOCID ASC";
	
	result = queryDB( sql.c_str());

	int ex_docid = -1;

	std::vector<Term<String, Integer>> vec_Term;
	for( int n0 = 0 ; n0 < result.size() ; n0++) {
		
		int doc_id = atoi( result[ n0].at( 0).c_str());
		if( ex_docid != doc_id) {
			if( vec_Term.size() > 0)
				return_map.insert( std::pair< Integer, std::vector<Term<String, Integer>>>( ex_docid, vec_Term));
			ex_docid = doc_id;
			vec_Term.clear();
		}
		
		Term< String, Integer> term;
		term.setTerm( UTF8ToANSI( result[ n0].at( 1).c_str()));
		term.setTermFreq( atoi( result[ n0].at( 2).c_str()));
		term.setScore( 0);

		vec_Term.push_back( term);
	}
	
	return return_map;
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


std::map< Integer, std::map<String, FreqScore<Integer, Integer>>> SqliteConnector::getALLDocInfoMap( int flag){
	
	String sql;
	std::vector< std::vector< String>> result;
	std::map< Integer, std::map<String, FreqScore<Integer, Integer>>> return_map;

	sql = "SELECT DOCID, NAME, FREQUENCY FROM WORD_ID ";
	sql += ( flag == QUESTION)?	"INNER JOIN QUESTION_TF ON WORD_ID.WORDID = QUESTION_TF.WORDID" : "INNER JOIN ANSWER_TF ON WORD_ID.WORDID = ANSWER_TF.WORDID ORDER BY DOCID ASC";
	
	result = queryDB( sql.c_str());

	int ex_docid = -1;

	std::map<String, FreqScore<Integer, Integer>> map_Term;
	for( int n0 = 0 ; n0 < result.size() ; n0++) {
		
		int doc_id = atoi( result[ n0].at( 0).c_str());
		if( ex_docid != doc_id) {
			if( map_Term.size() > 0)
				return_map.insert( std::pair< Integer, std::map<String, FreqScore<Integer, Integer>>>( ex_docid, map_Term));
			ex_docid = doc_id;
			map_Term.clear();
		}
		
		FreqScore<Integer, Integer> freq_score(atoi( result[ n0].at( 2).c_str()), 0);
		std::pair<String, FreqScore<Integer, Integer>> term_pair(UTF8ToANSI( result[ n0].at( 1).c_str()), freq_score);

		map_Term.insert( term_pair);
	}
	
	return return_map;
}


std::vector<String> SqliteConnector::getSynonym( int word_id) {
	
	String sql;
	std::vector< std::vector< String>> result;
	sql = "SELECT NAME FROM SYNONYM ";
	sql += " WHERE WORDID='";
	sql += std::to_string( word_id);
	sql += "'";
	
	result = queryDB( sql.c_str());

	std::vector<String> vec_Term;
	for( auto n1 = 0 ; n1 < result.size() ; n1++)
		vec_Term.push_back( UTF8ToANSI( result[ n1].at( 0).c_str()));
	
	return vec_Term;
}


std::vector<String> SqliteConnector::getSynonym( std::string term) {
	
	String sql;
	std::vector< std::vector< String>> result;
	sql = "SELECT WORDID FROM SYNONYM ";
	sql += " WHERE NAME='";
	sql += term;
	sql += "'";
	
	result = queryDB( sql.c_str());

	std::vector<String> vec_Term;
	if( result.size() > 0)
		vec_Term = getSynonym( atoi( result[0].at( 0).c_str()));
	
	return vec_Term;
}


std::vector< SynonymTerm> SqliteConnector::getSynonymTable() {
	String sql;
	std::vector< std::vector< String>> result;
	std::vector< SynonymTerm> return_vec;
	sql = "SELECT * FROM SYNONYM";
		
	result = queryDB( sql.c_str());
	
	for( int n = 0 ; n < result.size() ; n++) {
		SynonymTerm temp( atoi( result[ n].at( 0).c_str()), UTF8ToANSI( result[ n].at( 1).c_str()));
		return_vec.push_back( temp);
	}

	return return_vec;
}


std::map< Integer, String> SqliteConnector::getWordIDTable() {
	String sql;
	std::vector< std::vector< String>> result;
	sql = "SELECT WORDID, NAME FROM WORD_ID ";
	
	result = queryDB( sql.c_str());

	std::map< Integer, String> return_map;
	for( int n = 0 ; n < result.size() ; n++)
		return_map.insert( std::pair< Integer, String>( atoi( result[ n].at( 0).c_str()), UTF8ToANSI( result[ n].at( 1).c_str())));
	
	return return_map;
}


bool SqliteConnector::createSynonymTable() {
	String sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='SYNONYM'";
	std::vector< std::vector< String>> result;

	result = queryDB( sql.c_str());
	
	if( result.size() == 0) {
		std::cout << "유의어 테이블 생성" << std::endl;
		sql =	"CREATE TABLE SYNONYM( "	\
				"WORDID		INT		NOT	NULL, "	\
				"NAME		TEXT	NOT	NULL);";
	
		queryDB( sql.c_str());
		/*if( sqlite3_exec( mSqliteDB, sql.c_str(), 0, 0, 0) != SQLITE_OK )
			return false;*/
	}
	return true;
}


int SqliteConnector::getDocTextLength( Integer doc_id, int flag) {
	String sql;
	std::vector< std::vector< String>> result;

	sql = "SELECT STRLEN FROM ";
	sql += ( flag == QUESTION)?	"QUESTION_DOC_STRLEN" : "ANSWER_DOC_STRLEN";
	sql += " WHERE DOCID='";
	sql += std::to_string( doc_id);
	sql += "'";

	result = queryDB( sql.c_str());
	if( result.size() == 0)
		return -1;
	else
		return atoi( result[0].at( 0).c_str());
}


std::map< Integer, Integer> SqliteConnector::getALLDocTextLength( int flag) {
	String sql;
	std::vector< std::vector< String>> result;

	sql = "SELECT DOCID, STRLEN FROM ";
	sql += ( flag == QUESTION)?	"QUESTION_DOC_STRLEN" : "ANSWER_DOC_STRLEN";

	result = queryDB( sql.c_str());

	std::map< Integer, Integer> return_map;

	for( int n = 0 ; n < result.size() ; n++)
		return_map.insert( std::pair< Integer, Integer>( atoi( result[ n].at( 0).c_str()), atoi( result[ n].at( 1).c_str())));
	return return_map;
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

std::string SqliteConnector::UTF8ToANSI( const char *pszCode)
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

	std::string return_str = pszAnsi;
	delete[] pszAnsi;
	return return_str;
}

std::string SqliteConnector::ANSIToUTF8( const char * pszCode)
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

	std::string return_str = pszUTFCode;
	free( pszUTFCode);
	return return_str;
}


std::wstring SqliteConnector::utf8_to_utf16(const std::string& utf8)
{
    std::vector<unsigned long> unicode;
    size_t i = 0;
    while (i < utf8.size())
    {
        unsigned long uni;
        size_t todo;
        bool error = false;
        unsigned char ch = utf8[i++];
        if (ch <= 0x7F)
        {
            uni = ch;
            todo = 0;
        }
        else if (ch <= 0xBF)
        {
            throw std::logic_error("not a UTF-8 string");
        }
        else if (ch <= 0xDF)
        {
            uni = ch&0x1F;
            todo = 1;
        }
        else if (ch <= 0xEF)
        {
            uni = ch&0x0F;
            todo = 2;
        }
        else if (ch <= 0xF7)
        {
            uni = ch&0x07;
            todo = 3;
        }
        else
        {
            throw std::logic_error("not a UTF-8 string");
        }
        for (size_t j = 0; j < todo; ++j)
        {
            if (i == utf8.size())
                throw std::logic_error("not a UTF-8 string");
            unsigned char ch = utf8[i++];
            if (ch < 0x80 || ch > 0xBF)
                throw std::logic_error("not a UTF-8 string");
            uni <<= 6;
            uni += ch & 0x3F;
        }
        if (uni >= 0xD800 && uni <= 0xDFFF)
            throw std::logic_error("not a UTF-8 string");
        if (uni > 0x10FFFF)
            throw std::logic_error("not a UTF-8 string");
        unicode.push_back(uni);
    }
    std::wstring utf16;
    for (size_t i = 0; i < unicode.size(); ++i)
    {
        unsigned long uni = unicode[i];
        if (uni <= 0xFFFF)
        {
            utf16 += (wchar_t)uni;
        }
        else
        {
            uni -= 0x10000;
            utf16 += (wchar_t)((uni >> 10) + 0xD800);
            utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
        }
    }
    return utf16;
}


double SqliteConnector::get_levenshtein_distance( const std::wstring& wcs1, const std::wstring& wcs2)
{ 
    unsigned int len1 = wcslen(wcs1.c_str());
    unsigned int len2 = wcslen(wcs2.c_str());
	
    int** d = new int*[ len1 + 1];
	for( int n = 0 ; n < len1 + 1 ; n++)
		d[ n] = new int[ len2 + 1];
 
    for ( unsigned int i = 0; i <= len1; ++i) {
        d[i][0] = i;
    }
    for ( unsigned int j = 0; j <= len2; ++j) {
        d[0][j] = j;
    }
 
    for ( unsigned int i = 0; i < len1; ++i) {
        for ( unsigned int j = 0; j < len2; ++j) {
            int cost;
            if (wcs1.at( i) == wcs2.at( j)) {
                cost = 0;
            } else {
                cost = 1;
            }
            d[i + 1][j + 1] =
                min(min(d[i][j + 1] + 1, d[i + 1][j] + 1), d[i][j] + cost);
        }
    }
 
	double return_val = 1.0 - double(d[len1][len2]) / double(max(len1, len2));
	for( int n = 0 ; n < len1 + 1 ; n++)
		delete[] d[ n];
	delete[] d;
    
	return return_val;
}


bool SqliteConnector::hasEnding(String const &fullString, String const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool SqliteConnector::initExistsDB() const
{
	struct stat buffer;
	if(stat(mDbName.c_str(), &buffer) == 0)
	{
		#ifdef _WIN32
		String command = "del /s /q " + mDbName;
		#else
		String command = "rm -f " + mDbName;
		#endif
		system(command.c_str());
		return true;
	}
	else
		return false;
}


#include <direct.h>
#include <fstream>
void SqliteConnector::getLSAtestfileTF() {
	String sql;
	std::vector< std::vector< String>> result;

	sql = "SELECT DOCID, WORDID, SUM( FREQUENCY) FROM ( "	\
			"SELECT DOCID, WORDID, FREQUENCY FROM QUESTION_TF "	\
			"UNION ALL "	\
			"SELECT DOCID, WORDID, FREQUENCY FROM ANSWER_TF "	\
			") AS a GROUP BY DOCID, WORDID";
	result = queryDB( sql.c_str());
	
	_mkdir( "LSA");
	_mkdir( "LSA/TF");
	std::ofstream fout;
	if( result.size() == 0)
		return;
	else {
		String fname = "LSA/TF/doc" + std::to_string( atoi( result[ 0].at( 0).c_str())) + ".txt";
		fout.open( fname);
	}

	int ex_doc_id = -1;
	for( int n = 0 ; n < result.size() ; n++) {
		if( ex_doc_id != atoi( result[ n].at( 0).c_str())) {
			ex_doc_id = atoi( result[ n].at( 0).c_str());
			fout.close();
			String fname = "LSA/TF/doc" + std::to_string( ex_doc_id) + ".txt";
			fout.open( fname);
		}
		fout << result[ n].at( 1) << " " << result[ n].at( 2) << std::endl;
	}
}

void SqliteConnector::getLSAtestfileTF_MUL_IDF() {
	String sql;
	std::vector< std::vector< String>> result;

	sql = "SELECT DOCID, T1.WORDID, TF * 1.0 / DF FROM ( "	\
			"SELECT DOCID, WORDID, SUM( FREQUENCY) AS TF FROM ( "	\
			"SELECT DOCID, WORDID, FREQUENCY FROM QUESTION_TF "	\
			"UNION ALL "	\
			"SELECT DOCID, WORDID, FREQUENCY FROM ANSWER_TF "	\
			") AS a GROUP BY DOCID, WORDID ) AS T1 "	\
			"INNER JOIN ( "	\
			"SELECT WORDID, SUM( FREQUENCY) AS DF FROM ( "	\
			"SELECT WORDID, FREQUENCY FROM QUESTION_DF "	\
			"UNION ALL "	\
			"SELECT WORDID, FREQUENCY FROM	ANSWER_DF "	\
			") AS A GROUP BY WORDID ) AS T2 ON T1.WORDID = T2.WORDID ";
	result = queryDB( sql.c_str());
	
	_mkdir( "LSA");
	_mkdir( "LSA/TF_MUL_IDF");
	std::ofstream fout;
	if( result.size() == 0)
		return;
	else {
		String fname = "LSA/TF_MUL_IDF/doc" + std::to_string( atoi( result[ 0].at( 0).c_str())) + ".txt";
		fout.open( fname);
	}

	int ex_doc_id = -1;
	for( int n = 0 ; n < result.size() ; n++) {
		if( ex_doc_id != atoi( result[ n].at( 0).c_str())) {
			ex_doc_id = atoi( result[ n].at( 0).c_str());
			fout.close();
			String fname = "LSA/TF_MUL_IDF/doc" + std::to_string( ex_doc_id) + ".txt";
			fout.open( fname);
		}
		fout << result[ n].at( 1) << " " << result[ n].at( 2) << std::endl;
	}
}