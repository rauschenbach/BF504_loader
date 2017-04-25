#include "xpander.h"
#include "ports.h"
#include "sport0.h"
#include "timer0.h"
#include "rele.h"
#include "led.h"

/**
 * Инициализация портов на БФ + sport0 и SPI1
 * По-умолчанию все порты на входе в Z - состоянии (page 9 - 3)
 * Просто отключим все их функции
 * 
 */
section("L1_code")
void init_bf_ports(void)
{
	/* Порты F: отключаем функции и все на вход */
	*pPORTF_FER = 0;
	*pPORTFIO_DIR = 0;

	/* От всех портов отключаем функции */
	*pPORTG_FER &= ~(PG0 | PG1 | PG2 | PG3 | PG4 | PG5 | PG6 | PG7 | PG8 | PG9 | PG10 | PG11 /* | PG12 | PG13*/  | PG14 | PG15);
	*pPORTGIO_CLEAR = (PG3 | PG4 | PG11);	/* ставим в ноль */	
	*pPORTGIO_DIR = (PG3 | PG4 | PG11);	/* На выход, все остальные на вход  */

	*pPORTH_FER &= ~(PH0 | PH1 | PH2);	/* Порты H: PH1 на вход-остальные на выход и в ноль */
	*pPORTHIO_CLEAR = (PH0 | PH2);		/* ставим в ноль */	
	*pPORTHIO_DIR = (PH0 | PH2);

	 SPORT0_config();			/* СПОРТ0  */
}

/**
 * Инициализация портов на атмеге
 * По-умолчанию все порты на входе в Z - состоянии (page 9 - 3)
 */
section("L1_code")
void init_atmega_ports(void)
{
	/* Сбросим и уберем reset с экспандера PH0 */
	*pPORTH_FER &= ~PH0;			/* Отключаем функции */
	*pPORTHIO_CLEAR = PH0;			/* Делаем 0 на выход */	
	*pPORTHIO_DIR |= PH0;			/* Делаем их на выход */
	ssync();	
	*pPORTHIO_SET = PH0;			/* Делаем 1 на выход */
	ssync();

	delay_ms(WAIT_START_ATMEGA);	 /* Задержка, т.к. Наш DSP очень быстрый - не успевают инициализироваца Atmega регистры!!! */

	pin_clr(PORTG,  0x0F);	/* на выход порты G - ЛАМПЫ */
	pin_set(SD_SRCSEL_PORT, SD_SRCSEL_PIN);

	RELE_init();	/* Иниц. всех реле */
}


/**
 * Включить SD карту 
 * SD карта подключена к BF
 */
void select_sdcard_to_bf(void)
{
	pin_clr(SD_SRCSEL_PORT, SD_SRCSEL_PIN);
	delay_ms(100);
	pin_clr(SD_EN_PORT, SD_EN_PIN);
	delay_ms(10);
	pin_set(AT_SD_WP_PORT, AT_SD_WP_PIN);
	delay_ms(100);
	pin_clr(AT_SD_WP_PORT, AT_SD_WP_PIN);
	delay_ms(10);
	pin_set(AT_SD_CD_PORT, AT_SD_CD_PIN);
	delay_ms(10);	
}

/**
 */
void unselect_ports(void)
{
	pin_set(SD_EN_PORT, SD_EN_PIN);
	delay_ms(10);

	/* Сбросим экспандер PH0 */
	*pPORTH_FER &= ~PH0;			/* Отключаем функции */
	*pPORTHIO_CLEAR = PH0;			/* Делаем 0 на выход */	
	*pPORTHIO_DIR |= PH0;			/* Делаем их на выход */
	ssync();
	delay_ms(10);		
}

