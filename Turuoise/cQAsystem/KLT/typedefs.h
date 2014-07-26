/*
	File Name: typedefs.h
	Description: Type declarations for HAM and KLP.
	Written by: Kang, Seung-Shik	04/11/1997
*/

/*-----------------------------------------------------------*/
/*                                                           */
/*      Well-defined types and well-defined variables        */
/*      will make you happy in the near future.              */
/*                                                           */
/*-----------------------------------------------------------*/

/*                                                           */
/*                                                           */
/*          Common types used by HAM routines                */
/*                                                           */
/*                                                           */

/*
	If 'signed' or 'unsigned' ... ERROR occurs when compiling,
	'signed' and 'unsigned' should be commented like 'far'.
*/
#define HAM_FAR		/*far*/
#define SIGNED		/*signed*/
#define UNSIGNED	unsigned

typedef FILE *                FILE_PTR;

typedef SIGNED int            HAM_SHORT;
typedef HAM_SHORT HAM_FAR *   HAM_PSHORT;
typedef UNSIGNED int          HAM_USHORT;
typedef HAM_USHORT HAM_FAR *  HAM_PUSHORT;
typedef SIGNED long           HAM_SLONG;
typedef UNSIGNED long         HAM_ULONG;

typedef UNSIGNED char         HAM_UCHAR;
typedef HAM_UCHAR HAM_FAR *   HAM_PUCHAR;
typedef HAM_PUCHAR HAM_FAR *  HAM_PPUCHAR;

typedef void HAM_FAR *        HAM_PVOID;
typedef HAM_PVOID HAM_FAR *   HAM_PPVOID;

typedef int	MYBOOLEAN;

/*                                                          */
/*           Common types used by HAM for                   */
/*              Hangul code representation.                 */
/*                                                          */

typedef UNSIGNED int		SYLLABLE;
typedef SYLLABLE HAM_FAR *	PSYLLABLE;
typedef PSYLLABLE HAM_FAR *	PPSYLLABLE;

/*------------------ end of typedefs.h ---------------------*/
