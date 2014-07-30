/*
	File Name: ham-api.h
	Description: HAM API functions
	Written by: Kang, Seung-Shik	11/06/2002
	Modified at 03/24/2003
*/
/*-------------------------------------------------------------
	Compile option : 라이브러리 함수의 프로토타입 정의
	(Linux/UNIX에서는 아래 #define WINDOWS을 삭제하세요)
-------------------------------------------------------------*/
#define UNIX_CPP

#ifdef WINDOWS_DLL			/* DLL export */
#define PREFIX	_declspec(dllimport)
#elif defined(WINDOWS_DLL_CPP)	/* 윈도 VC++의 C++ 인터페이스 */
#define PREFIX	"C" _declspec(dllimport)
#elif defined(UNIX_CPP)	/* 윈도 VC++의 C++ 인터페이스 */
#define PREFIX	"C"
#else						/* Linux/Unix 등 유닉스 계열 */
#define PREFIX
#endif

extern PREFIX int open_HAM_index(
	HAM_RUNMODE *mode,	/* running mode of HAM */
	char *options,	/* option string */
	char *inifile);	/* file name: 'ham2000.ini' */
extern PREFIX int open_HAM_index_dicpath(
	HAM_RUNMODE *mode,	/* running mode of HAM */
	char *options,	/* option string */
	char *inifile,	/* file name: 'ham2000.ini' */
	char *dpath,	/* 사전 디렉토리 지정: 예) "/usr/sskang/HAM/hdic/" */
	char *dic_usr,	/* ham-usr.dic */
	char *dic_cnn,	/* ham-cnn.dic */
	char *dic_stop);	/* stopword.dic */
extern PREFIX void close_HAM_index(HAM_RUNMODE *);
extern PREFIX int get_stems(		/* 어절 or 문장 단위 인터페이스 */
	HAM_PUCHAR sent,	/* KSC5601 input sentence */
	HAM_PUCHAR keys[],	/* ptr. array of keywords */
	HAM_PRUNMODE mode);	/* running mode of HAM */
extern PREFIX int get_stems_TS(	/* 어절 or 문장 단위 & multi-thread */
	HAM_PUCHAR sent,	/* KSC5601 input sentence */
	HAM_PUCHAR keys[],	/* ptr. array of keywords */
	HAM_PUCHAR keylist,	/* keyword saving area */
	HAM_PMORES hamout,	/* morph. analysis result */
	HAM_PRUNMODE mode);	/* running mode of HAM */
extern PREFIX int get_terms_text(	/* 문서 단위 인터페이스 */
	unsigned char *text,	/* KSC5601 input text */
	HAM_TERMLIST term[],	/* array of extracted terms */
	HAM_TERMMEMORY *tm,		/* memories for term extraction */
	HAM_RUNMODE *mode,		/* running mode of HAM */
	int maxTerms,			/* 추출된 용어수: 명령행 인자로 용어의 최대 개수 지정 */
	int sortflag,			/* 0: 출현순서로(용어 중복), 1: sort & unify(중복 제거) */
	int widflag);			/* 0: 어절 순서(1, 2, 3, ...), 1: 문장단위(101, 102, 201, 202, ...) */
extern PREFIX int get_terms_file(	/* 문서 단위 인터페이스 -- 파일 입력 */
	char *fname,	/* input: text-file name */
	HAM_TERMLIST term[],	/* array of extracted terms */
	HAM_TERMMEMORY *tm,		/* memories for term extraction */
	HAM_RUNMODE *mode,		/* running mode of HAM */
	int maxTerms,			/* 추출된 용어수: 명령행 인자로 용어의 최대 개수 지정 */
	int sortflag,			/* 0: 출현순서로(용어 중복), 1: sort & unify(중복 제거) */
	int widflag);			/* 0: 어절 순서(1, 2, 3, ...), 1: 문장단위(101, 102, 201, 202, ...) */
extern PREFIX int get_terms_sent(	/* term-extraction from a sentence */
	unsigned char *sent,	/* KSC5601 input text */
	HAM_TERMLIST term[],	/* array of extracted terms */
	HAM_TERMMEMORY *tm,		/* memories for term extraction */
	HAM_RUNMODE *mode,		/* running mode of HAM */
	int weightflag);		/* term weighting 적용 여부: 0(TF), 1(term weighting) */
extern PREFIX int set_options_from_file(char *, HAM_RUNMODE *);
/*--------------------------------- end of ham-api.h ---------------------------------*/
