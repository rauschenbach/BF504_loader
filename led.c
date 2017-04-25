#include "led.h"
#include "timer0.h"


/* Задержка во время переключения - подобрать! */
#define LED_TOGGLE_DELAY_MS		5


/*******************************************************************
 *   Function:    LED_init - работает ТОЛЬКО внутри цикла CORE
 *******************************************************************/
section("L1_code")
void LED_init(void)
{
	pin_clr(LED1_PORT,  LED1_PIN);	/* 1 на выход G */
	pin_clr(LED2_PORT,  LED2_PIN);	/* 1 на выход Y */
	pin_clr(LED3_PORT,  LED3_PIN);	/* 1 на выход R */
	pin_clr(LED4_PORT,  LED4_PIN);	/* 1 на выход B */
}


/************************************************************************************* 
 * Включаем по одному 
 **************************************************************************************/
section("L1_code")
void LED_on(u8 led)
{
#if defined 	GNS110_R2A_BOARD || defined GNS110_R2B_BOARD || defined GNS110_R2C_BOARD
	pin_set(LEDS_PORT, led);
#else
	if(led == LED1)
		pin_set(LED1_PORT, LED1_PIN);
	else if(led == LED2)
		pin_set(LED2_PORT, LED2_PIN);
	else if(led == LED3)
		pin_set(LED3_PORT, LED3_PIN);
        else
		pin_set(LED4_PORT, LED4_PIN);
#endif
}

/************************************************************************************* 
 * Выключить по-одному 
 **************************************************************************************/
section("L1_code")
void LED_off(u8 led)
{
#if defined 	GNS110_R2A_BOARD || defined GNS110_R2B_BOARD || defined GNS110_R2C_BOARD
	pin_clr(LEDS_PORT, led);	/* "0" на выход порта */
#else
	if(led == LED1)
		pin_clr(LED1_PORT, LED1_PIN);
	else if(led == LED2)
		pin_clr(LED2_PORT, LED2_PIN);
	else if(led == LED3)
		pin_clr(LED3_PORT, LED3_PIN);
        else 
		pin_clr(LED4_PORT, LED4_PIN);
#endif
}

/************************************************************************************* 
 * Переключить по-одному 
 **************************************************************************************/
section("L1_code")
void LED_toggle(u8 led)
{
#if defined 	GNS110_R2A_BOARD || defined GNS110_R2B_BOARD|| defined GNS110_R2C_BOARD
	pin_set(LEDS_PORT, led);
	delay_ms(LED_TOGGLE_DELAY_MS);
	pin_clr(LEDS_PORT, led);
#else
	if(led == LED1) {
		pin_set(LED1_PORT, LED1_PIN);
		pin_clr(LED1_PORT, LED1_PIN);
	} else if (led == LED2) {
		pin_set(LED2_PORT, LED2_PIN);
		pin_clr(LED2_PORT, LED2_PIN);
	} else if(led == LED3){
		pin_set(LED3_PORT, LED3_PIN);
		pin_clr(LED3_PORT, LED3_PIN);
	} else {
		pin_set(LED4_PORT, LED4_PIN);
		pin_clr(LED4_PORT, LED4_PIN);
	}	
#endif
}


