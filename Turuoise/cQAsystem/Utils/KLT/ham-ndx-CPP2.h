/*
	File Name: ham-ndx.h
	Description: header file collections for HAM
	Written by: Kang, Seung-Shik	Apr.1997 - Mar.2005
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "typedefs.h"
#include "sizedefs.h"
#include "runmode.h"

#include "hdics.h"
#include "io-defs.h"

#include "err-code.h"
#include "hamout1.h"

#include "keyword.h"
#include "termtext.h"	/* 한 문서에 대한 색인어 추출에 필요한 자료구조 */

#include "ham-api-CPP2.h"

#define WELCOME(version) \
	fprintf(stderr, "------------------------------------------------------------------\n"); \
	fprintf(stderr, "      Welcome to KLT(Korean Language Technology) version %s\n", version); \
	fprintf(stderr, "   <<< BEFORE USING IT, YOU SHALL READ THE FILE LICENSE.TXT >>>\n"); \
	fprintf(stderr, "------------------------------------------------------------------\n")

#define GOODBYE(year) \
	fprintf(stderr, "------------------------------------------------------------------\n"); \
	fprintf(stderr, "(c) 1993-%d KLT(Korean Language Technology), Seung-Shik Kang\n", year); \
	fprintf(stderr, "    Email: sskang@kookmin.ac.kr, http://nlp.kookmin.ac.kr/\n"); \
	fprintf(stderr, "------------------------------------------------------------------\n")
/*--------------------------------- end of ham-ndx.h ---------------------------------*/
