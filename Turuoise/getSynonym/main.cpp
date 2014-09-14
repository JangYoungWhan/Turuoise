#include <iostream>
#include <fstream>
#include <regex>
#include <Windows.h>
#include <vector>
#include <thread>


#pragma comment( lib, "urlmon.lib")
#pragma comment( lib, "OleAut32.lib")

// 전역변수
#define THREAD_NUM 20		// 사용할 스레드 갯수
bool life_print_progress_thread;
int progress[ THREAD_NUM];
std::vector< std::string> vec_word;
std::vector< std::vector< std::string>> vec_vec_synonym[ THREAD_NUM];

LARGE_INTEGER Tbegin, Tend, TFrequency;


// 함수
void getSynonym( int thread_number, int start_index, int end_index);
void Print_progress( int m);

std::string UTF8ToANSI( const char *pszCode);
std::string ANSIToUTF8( const char * pszCode);
std::wstring utf8_to_utf16(const std::string& utf8);



// main 
int main( int argc, char* argv[]) {

	std::string output_file_name;
	if( argc != 2)
		output_file_name = "synonym.txt";
	else 
		output_file_name = argv[ 1];

	QueryPerformanceFrequency( &TFrequency);
	QueryPerformanceCounter( &Tbegin);      
	std::cout << ">>현재 스레드 [" << THREAD_NUM << "]사용. 코드에서 갯수 수정" << std::endl;
	std::ifstream fin_( "word.txt");

	if( fin_.is_open() == false) { 
		std::cout << ">>word.txt 파열열기 실패" << std::endl;
		return 0;
	}
	
	char buf[ 1024];
	while( fin_.getline( buf, 1024)) {
		if( *buf == '\r' || *buf == '\n')
			break;
		vec_word.push_back( buf);
	}
	fin_.close();

	if( THREAD_NUM > vec_word.size()) {
		std::cout << ">>THREAD_NUM[" << THREAD_NUM << "] > 단어갯수[" << vec_word.size() << "] error" << std::endl;
		return 1;
	}

	life_print_progress_thread = true;
	std::thread print_progress_thread( &Print_progress, vec_word.size());

	int step = vec_word.size() / THREAD_NUM;
	int arr_step[ THREAD_NUM + 1] = { 0,};
	for( int s1 = 1 ; s1 < THREAD_NUM ; s1++)
		arr_step[ s1] = step * s1;
	arr_step[ THREAD_NUM] = vec_word.size();

	std::thread* arr_pthread[ THREAD_NUM];
	for( int t1 = 0 ; t1 < THREAD_NUM ; t1++)
		arr_pthread[ t1] = new std::thread( &getSynonym, t1, arr_step[ t1], arr_step[ t1 + 1]);

	for( int t1 = 0 ; t1 < THREAD_NUM ; t1++) {
		arr_pthread[ t1]->join();
		delete arr_pthread[ t1];
	}
	
	life_print_progress_thread = false;
	print_progress_thread.join();

	std::ofstream fout( output_file_name.c_str());
	for( int n1 = 0 ; n1 < THREAD_NUM ; n1++) {
		for( int n2 = 0 ; n2 < vec_vec_synonym[ n1].size() ; n2++) {
			std::vector< std::string>& vec = vec_vec_synonym[ n1].at( n2);
			for( int n3 = 0 ; n3 < vec.size() ; n3++)
				//fout << vec[ n3].c_str() << "\t";		// 텍스트 파일이 ANSI인코딩일 경우
				fout << ANSIToUTF8( vec[ n3].c_str()) << "\t";	// 텍스트 파일이 UTF8인코딩 일경우
			fout << "\n";
		}
	}
	fout.close();

	
    QueryPerformanceCounter( &Tend);  
	std::cout << "\n>> " << output_file_name << " 출력 완료" << std::endl;
	std::cout << ">>수행시간 : [" << ( Tend.QuadPart - Tbegin.QuadPart) / (double)TFrequency.QuadPart << "]s " << std::endl;

	return 0;
}



void getSynonym( int thread_number, int start_index, int end_index) {

	std::wstring temp_HTML = L"temp" + std::to_wstring( thread_number);
	temp_HTML += L".html";

	for( unsigned int l1 = start_index ; l1 < end_index ; l1++) {
	
		LPWSTR dic_url = L"http://dic.naver.com/search.nhn?&query=";
				
		std::string term = vec_word[ l1];

		std::wstring url = dic_url;
		//url += utf8_to_utf16( ANSIToUTF8( term.c_str()));	// word.txt가 ANSI인코딩일 경우
		url += utf8_to_utf16( term.c_str());	// word.txt 가 utf8 인코딩일경우

		HRESULT hr = URLDownloadToFile( NULL, url.c_str(), temp_HTML.c_str(), NULL, NULL);

		if( hr == S_OK) {
		}
		else {
			while ( hr == S_OK)	// 인터넷 연결안되있으면 무한루프임
				hr = URLDownloadToFile( NULL, url.c_str(), temp_HTML.c_str(), NULL, NULL);
			//std::cout << thread_number << "인터넷확인해라!!!!!!!!!!" << std::endl;
		}

		std::ifstream fin( temp_HTML.c_str());
		if( fin.is_open() == false) {
			std::cout << ">>스레드안에서 파열열기 실패" << std::endl;
			exit( 0);
		}

		std::vector< std::string> vec_synonym;
		
		// 0번 인덱스의 값은 유의어를 찾은 단어
		//vec_synonym.push_back( term);	// 텍스트 파일이 ANSI인코딩일 경우
		vec_synonym.push_back( UTF8ToANSI( term.c_str()));	// 텍스트파일이 utf8 인코딩일 경우

		char buf[ 4096];
		while( fin.getline( buf, 4096)) {
			if( *buf == '\0' || *buf == '\n' || *buf == '\r')
				continue;

			char* ptrbuf = buf;
			int n = 0;
			for( ; ( *ptrbuf == ' ' || *ptrbuf == '\t') && n < strlen( buf) ; n++, ptrbuf++);
		
			if( n == strlen( buf))
				continue;

			std::string line_str = UTF8ToANSI( ptrbuf);

			if( strstr( line_str.c_str(), "[유의어]") != NULL) {   // 유의어가 있을 경우 첫번째 [유의어]가 명사 두번째는 동사... 그외
				while( fin.getline( buf, 4096)) {
				
					if( strstr( buf, "</p>") != NULL)
						break;

					line_str = UTF8ToANSI( buf);

					std::tr1::cmatch res;
					std::tr1::regex rx("<a.*\">([^<]+)");
					int num = std::tr1::regex_search( line_str.c_str(), res, rx);
												
					if( num > 0) {
						std::string synonym = res[ 1];
						vec_synonym.push_back( synonym);
					}
				}

				break;	// 명사 유의어만 추출
			}
		}
		
		vec_vec_synonym[ thread_number].push_back( vec_synonym);
		fin.close();
		progress[ thread_number]++;
	}

	std::string stemp_HTML = "temp" + std::to_string( thread_number);
	stemp_HTML += ".html";
	remove( stemp_HTML.c_str());

	/*
	for( unsigned int n1 = 0 ; n1 < vec_vec_synonym.size() ; n1++) {
		std::vector<std::string>& vec = vec_vec_synonym[ n1];

		for( unsigned int n2 = 0 ; n2 < vec.size() ; n2++) {
			std::cout << vec[ n2] << std::endl;
		}
	}
	*/
}


void Print_progress( int m) {

	while( life_print_progress_thread) {
		int progress_sum = 0;
		for( int n = 0 ; n < THREAD_NUM ; n++)
			progress_sum += progress[ n];
		std::cout << "\r>> " << progress_sum << " / " << m << "추출중...";
		Sleep( 500);
	}
}


std::string UTF8ToANSI( const char *pszCode)
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


std::string ANSIToUTF8( const char * pszCode)
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


std::wstring utf8_to_utf16(const std::string& utf8)
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