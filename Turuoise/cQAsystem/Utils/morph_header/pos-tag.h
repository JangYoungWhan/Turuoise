/*
	File name: pos-tag.h
	Description: parts of speech tags for 'bpos' or 'spos'.
	Written by: Kang, Seung-Shik	2000. 12. 15.
*/

/*-------------------- Basic POS tagset --------------------*/
#define HAM_POS_NOUN     'N'       /* noun, pnoun, xn, nume */

#define HAM_POS_VERB     'V'       /* verb                  */
#define HAM_POS_XVERB    'x'       /* auxiliary verb        */
#define HAM_POS_ADJ      'J'       /* adjective             */
#define HAM_POS_COPULA   'c'       /* copula '-Wi-'         */

#define HAM_POS_ADV      'B'       /* adverb                */
#define HAM_POS_DET      'D'       /* determiner            */
#define HAM_POS_EXCL     'L'       /* exclamation           */

#define HAM_POS_PUNC     'P'       /* punctuation mark      */
/*----------------------------------------------------------*/

/*-------------------- Rarely-used tags --------------------*/
#define HAM_POS_JOSA     'j'       /* Korean Josa           */
#define HAM_POS_EOMI     'e'       /* final Ending          */
#define HAM_POS_SYMB     'S'       /* special symbols       */
#define HAM_POS_ETC      'Z'       /* one of adv/det/excl   */
/*----------------------------------------------------------*/

/*--------------------- Unused POS tag ---------------------*/
#define HAM_POS_NVERBK   'Y'       /* guessed as noun+verb  */

#define HAM_POS_SQUOTE   's'       /* single quotation      */
#define HAM_POS_DQUOTE   'd'       /* double quotation      */
#define HAM_POS_LPAREN   'l'       /* left parenthesis      */
#define HAM_POS_RPAREN   'r'       /* right parenthesis     */
/*------------------- end of pos-patn.h  -------------------*/
