#ifndef _RELE_H
#define _RELE_H

#include "my_defs.h"

/**************************************************************
 * 	Структуры и типы РЕЛЕ, кот. присутствуют на плате
 *************************************************************/
/* Типы РЕЛЕ  */
typedef enum {
	RELEPOW = 0,
	RELEAM,
	RELEBURN
} RELE_TYPE_ENUM;


bool RELE_on(RELE_TYPE_ENUM);
bool RELE_off(RELE_TYPE_ENUM);
void RELE_init(void);

#endif				/* rele.h  */
