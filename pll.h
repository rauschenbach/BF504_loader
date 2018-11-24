/************************************************************************************** 
 * Для процессора BF504F с внутренней флешкой 
 * CLKIN    = 19.2 МГц
 * умножение частоты с шагом для:
 * CCLK     =  288, 230.4, 192, 96 МГц
 * SCLK     =  72, 64, 48, 32, 24  МГц
 * потом можно будет добавить и дробные частоты, но частота SCLK должна быть целой!
 * Будут добавлены функцыи для управления энергопотреблением и сном
 * Выбрать частоты клоков из дефиницый
 * Пробуем такие значения!
 * CCLK = 192MHZ SCLK = 48MHZ
 **************************************************************************************/
#ifndef	__PLL_H__
#define __PLL_H__

#include "globdefs.h"


/* Причины сброса прибора - перечислены здесь. Из запишем в eeprom  */
#define		CAUSE_POWER_OFF		0x12345678
#define		CAUSE_EXT_RESET		0xabcdef90
#define		CAUSE_BURN_OUT		0xaa55aa55		
#define		CAUSE_UNKNOWN		0xFFFFFFFF	


/* Частота кварца, поставленная на нашей плате, это константа нужна для SD карты */
#define QUARTZ_CLK_FREQ 8192000
/* Частота периферии проца = 24.576 МГц
 * MSEL[5:0] = 30 - получили VCO = 245.760 МГц из 8.192 
 * CSEL[1:0] = 0  - получили CCLK = VSO / 4 = 61.440 МГц, 
 * SSEL[3:0] = 10  - получили SCLK = VSO / 10 = 24.576 МГц */
#define 	SCLK_VALUE 	     24576000UL	
#define 	PLLCTL_VALUE         (30 << 9)
#define 	PLLDIV_VALUE         0x002A


/* Эти значения для Power Managements */
#define 	PLLSTAT_VALUE       0x0000			/* NB: Только чтение!!!  */
#define 	PLLLOCKCNT_VALUE    0x0200			/* Через 512 тактов заснуть */
#define 	PLLVRCTL_VALUE      ((1 << 9)|(1 << 11))	/* Просыпаться по прерываниям на ногах PF8 и PF9 */

/* Частоту таймера определяем в PLL  */
#define TIMER_PERIOD (SCLK_VALUE)


void PLL_init(void);
void PLL_sleep(DEV_STATE_ENUM);

void PLL_reset(void);
void PLL_fullon(void);
void PLL_hibernate(void);

#endif				/* pll.h */
