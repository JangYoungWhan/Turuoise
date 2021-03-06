/*-----------------------------------------------------------------------
	지배-의존 관계 : 주로 서술어, 체언이 지배소가 됨.
	모든 노드는 실명(문장에 출현한 단어)이며, 가상노드는 없음.
	(단, root 노드만 가상노드임.)
-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------
	1. 지배소가 root노드일 때 가능한 지배-의존 관계
		root 노드는 문장의 마지막 단어와 문장부호를 의존소로 취함.

		1) 서술어의 종결형  -- 정상적인 문장  : DREL_FIN
		2) 명사로 끝난 문장 -- 제목 or 명사구 : DREL_NNN
		3) 불완전한 문장 등 -- "...에 대하여" : DREL_UNK

	2. 지배소가 서술어(어말어미)일 때 가능한 의존소

		DREL_SBJ -- 주어
		DREL_OBJ -- 목적어
		DREL_CMP -- 보어

		DREL_ADV -- 부사, 부사+보조사, 형용사+'게' 등
		DREL_NNA -- 부사성 명사: '오늘/내일/현재/지금/조금'
		DREL_IMP -- 감탄사 및 독립언 상당어구

	3. 지배소의 head가 체언일 때 가능한 의존소

		DREL_DET -- 관형사
		DREL_GEN -- 명사의 소유격, '-의', '-적/화/성' 등
		DREL_NNN -- 명사
		DREL_AND -- 명사+'와/과', '및'
		DREL_OR  -- 명사+'나', '또는'
		DREL_KAN -- 용언의 관형형: 'ㄴ/ㄹ'

	4. 지배소가 형용사/부사일 때 가능한 의존소

		DREL_ADV --- 부사어: 부사, 부사+보조사, 형용사+'게' 등
-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
	체언의 dependency relation
-----------------------------------------------------------------------*/
#define DREL_SBJ	'S'	/* 주어 */
#define DREL_SBj	's'	/* 2nd 주어 */
#define DREL_OBJ	'O'	/* 목적어 */
#define DREL_OBj	'o'	/* 2nd 목적어 */
#define DREL_CMP	'C'	/* 보어 */

#define DREL_NNN	'N'	/* 명사 */
#define DREL_NNA	'a'	/* 부사성 명사: '오늘/내일/현재/지금/조금' */
#define DREL_NAD	'B'	/* 명사 + 부사격조사 */

#define DREL_GEN	'G'	/* 명사의 소유격, '-의', '-적/화/성' 등 */
#define DREL_AND	'&'	/* 명사 + '와/과' */
#define DREL_OR		'|'	/* 명사 + '와/과' */

/*-----------------------------------------------------------------------
	임시 or 세부적인 relation 정의
-----------------------------------------------------------------------*/
/* SB2는 중간단계에서 사용되고 최종적으로 SBJ/OBJ로 바뀜 */
#define DREL_SOJ	'?'	/* 보조사: '은/는/만/도' */
/* ESO는 NAD중 '에'로 시작되는 것 */
#define DREL_ESO	'e'	/* '에'로 시작되는 조사 */

/*-----------------------------------------------------------------------
	서술어(어말어미 기준)의 dependency relation
-----------------------------------------------------------------------*/
#define DREL_FIN	'F'	/* 종결어미 */
#define DREL_KAN	'K'	/* 관형형 어미: 'ㄴ/ㄹ' */
#define DREL_SUB	'_'	/* 종속절: 용언의 연결어미 등으로 수식 */

#define DREL_AUX	'x'	/* 지배소가 보조용언인 경우 */

/*-----------------------------------------------------------------------
	그외 의존소의 dependency relation
-----------------------------------------------------------------------*/
#define DREL_DET	'D'	/* 관형사 */
#define DREL_ADV	'A'	/* 부사어: 부사, 부사+보조사, 형용사+'게' 등 */
#define DREL_IMP	'!'	/* 감탄사 및 독립언 상당어구 */

#define DREL_PUN	'P'	/* 문장부호: ,.()[]"' 등 */
#define DREL_UNK	'U'	/* 기타 정의되지 않은 것 */

/*------------------------- end of dep-rel.h --------------------------*/
