#define MAX_SENT_DOC	512

typedef struct summary {
	int nsent;					/* number of sentences in doc. */

	int sid[MAX_SENT_DOC];		/* sentence identifier */
	char *sent[MAX_SENT_DOC];	/* sentences in doc. */

	int nword[MAX_SENT_DOC];	/* number of words in sent[i] */
	int nterm[MAX_SENT_DOC];	/* number of terms in sent[i] */

	int score[MAX_SENT_DOC];	/* weight value for sent[i] */
	int wbase[MAX_SENT_DOC];	/* base of word-location */

	char term1[MAX_SENT_DOC];	/* 1st term is 1st word in doc. or not */
} HAM_SUMMARY, *HAM_PSUMMARY;

extern PREFIX int sentence_extraction(
	unsigned char *text,	/* KSC5601 input text */
	HAM_SUMMARY *ab,		/* setence extraction result */
	HAM_TERMLIST term[],	/* array of extracted terms */
	HAM_TERMMEMORY *tm,		/* memories for term extraction */
	HAM_RUNMODE *mode);		/* running mode of HAM */
