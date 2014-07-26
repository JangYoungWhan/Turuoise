/*                                                               */
/*                                                               */
/*        Definition of error codes for HAM: not perfect yet     */
/*                                                               */
/*                                                               */

#define HANDIC_FILE_OPEN_FAIL	11	/* file 'hdic/hangul.dic' */

#define USERDIC_FILE_OPEN_FAIL	21	/* file 'hdic/ham-usr.dic' */
#define USERDIC_SORTING_ERROR	22
#define USERDIC_TOO_MANY_WORDS	23
#define USERDIC_AREA_OVERFLOW	24
#define USERDIC_DIC_POS_WRONG	25

#define RMADIC_FILE_OPEN_FAIL	31	/* file 'hdic/ham-rma.dic' */
#define RMADIC_SORTING_ERROR	32
#define RMADIC_TOO_MANY_WORDS	33
#define RMADIC_AREA_OVERFLOW	34
#define RMADIC_RES_PARSE_FAIL	36
#define RMADIC_NSFX_CONV_FAIL	37
#define RMADIC_VSFX_CONV_FAIL	38
#define RMADIC_POMI_CONV_FAIL	39
#define RMADIC_POS_TAG_WRONG	40
#define RMADIC_STEM_DPOS_FAIL	41

#define CNNDIC_FILE_OPEN_FAIL	61	/* file 'hdic/ham-cnn.dic' */

#define STOPDIC_FILE_OPEN_FAIL	51	/* file 'hdic/stopword.dic' */
#define STOPDIC_SORTING_ERROR	52
#define STOPDIC_TOO_MANY_WORDS	53
#define STOPDIC_AREA_OVERFLOW	54

#define HAM_RESULT_AREA_FULL	77	/* MAXRESULT overflow */

#define LOW_BYTE_HANGUL_MISSED	101	/* no low byte Hangul char */

#define HAM_SENT_SIZE_TOO_LONG	150	/* sent size too long */
#define HAM_WORD_SIZE_TOO_LONG	151	/* word size too long */
#define HAM_STEM_SIZE_TOO_LONG	152	/* stem size too long */

#define HAM_INVALID_INPUT_CHAR	161	/* input errors found */
#define HAM_MORE_THAN_MAXWORDS	162	/* too many words in sent.*/

/*----------------------- End of err-code.h ----------------------*/
