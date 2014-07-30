/* USED ONLY FOR 'ADJUST_WORD_SPACING' OF INPUT SENTENCE */
extern PREFIX int load_bigram_data(char *filename);
extern PREFIX int load_bigram_dpath(char *dicpath, char *fname);

extern PREFIX int adjust_word_spacing(unsigned char sent[], int flag);
extern PREFIX int adjust_word_spacing_nsyl(
	unsigned char *sent,	/* 원문 */
	unsigned char *sent2,	/* 자동 띄어쓰기 한 결과 저장 */
	int maxsiz,				/* sent2[]의 크기 */
	int nsyl);	/* nsyl 이상인 어절들만 띄어쓰기 적용, nsyl < 0 이면 원문 blank 유지 */

extern PREFIX void free_bigram_data(void);

extern PREFIX unsigned char *HANJA2KSC(unsigned char *ksc, unsigned char *ksm);

extern PREFIX int convert_e2k(unsigned char *estring, unsigned char *kstring);
extern PREFIX int convert_k2e(unsigned char *kstring, unsigned char *estring);
