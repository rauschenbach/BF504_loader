/* Моргание лампочками */
#ifndef _LED_H
#define _LED_H

#include "globdefs.h"
#include "xpander.h"

/* Состояние и моргание лампочками */
#define		LED_OFF_STATE  		0
#define		LED_ON_STATE   		1
#define		LED_SLOW_STATE 		2
#define		LED_QUICK_STATE 	3


/*******************************************************************
 *  function prototypes
 *******************************************************************/
void LED_init(void);
void LED_on(u8);
void LED_off(u8);
void LED_toggle(u8);

#define LED_all_off()    do { LED_off(LED1);LED_off(LED2);LED_off(LED3);LED_off(LED4);} while(0)

#if defined 	GNS110_R2A_BOARD || defined GNS110_R2B_BOARD || defined GNS110_R2C_BOARD
	#define LED1 		LED4_PIN
	#define LED2 		LED3_PIN
	#define LED3 		LED2_PIN
	#define LED4 		LED1_PIN
	#define LEDS_PORT	LED1_PORT
#else
	/* Пины через 1 равны - поэтому так */
	#define LED1 		0
	#define LED2 		1
	#define LED3 		2
	#define LED4 		3
#endif



#define LED_GREEN	LED1
#define LED_YELLOW	LED2
#define LED_RED  	LED3
#define LED_BLUE  	LED4

/* Лампочка обработчика сигнала */
#define LED_SIGNAL	LED_YELLOW

/* Лампочка включения */
#define LED_POWER	LED_RED


#endif /* led.h */
