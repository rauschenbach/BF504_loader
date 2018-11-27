/*******************************************************************************
 * В этом файле описываются инициализация PLL для нормального и спящего режима
 *******************************************************************************/
#include <ccblkfn.h>
#include <bfrom.h>
#include "pll.h"


/*  Инициализация PLL, на вариант в файле заголовка */
void PLL_init(void)
{
	u32 SIC_IWR1_reg;	/* backup SIC_IWR1 register */

	/* use Blackfin ROM SysControl() to change the PLL */
	ADI_SYSCTRL_VALUES sysctrl = {
		PLLVRCTL_VALUE,
		PLLCTL_VALUE,
		PLLDIV_VALUE,
		PLLLOCKCNT_VALUE,
		PLLSTAT_VALUE
	};

	SIC_IWR1_reg = *pSIC_IWR1;	/* save SIC_IWR1 due to anomaly 05-00-0432 */
	*pSIC_IWR1 = 0;		/* disable wakeups from SIC_IWR1 */

	/* use the ROM function */
	bfrom_SysControl(SYSCTRL_WRITE | SYSCTRL_PLLCTL | SYSCTRL_PLLDIV, &sysctrl, NULL);

	*pSIC_IWR1 = SIC_IWR1_reg;	/* restore SIC_IWR1 due to anomaly 05-00-0432 */
}



/* Перевести процессор в спящий режим  */
section("L1_code")
void PLL_sleep(DEV_STATE_ENUM state)
{
     if(state != DEV_COMMAND_MODE_STATE) {
	ADI_SYSCTRL_VALUES sleep;

	/* прочитать */
	bfrom_SysControl(SYSCTRL_EXTVOLTAGE | SYSCTRL_PLLCTL | SYSCTRL_READ, &sleep, NULL);
        sleep.uwPllCtl |= STOPCK;       /* Изменим на Sleep режим */

	/* и записать, нет нужды избегать аномальности - все равно проснеца */
	bfrom_SysControl(SYSCTRL_WRITE | SYSCTRL_EXTVOLTAGE | SYSCTRL_PLLCTL, &sleep, NULL);
    }
}

/* Перевести процессор в рабочий режим */
void PLL_fullon(void)
{
	ADI_SYSCTRL_VALUES fullon;

	/* прочитать */
	bfrom_SysControl(SYSCTRL_EXTVOLTAGE | SYSCTRL_PLLCTL | SYSCTRL_READ, &fullon, NULL);
        fullon.uwPllCtl &= ~STOPCK; 

	/* и записать, нет нужды избегать аномальности - все равно проснеца */
	bfrom_SysControl(SYSCTRL_WRITE | SYSCTRL_EXTVOLTAGE | SYSCTRL_PLLCTL, &fullon, NULL);
}


/* Гибернация */
void PLL_hibernate(void)
{
	ADI_SYSCTRL_VALUES hibernate;

	hibernate.uwVrCtl = 
	    WAKE_EN0 |	/* PH0 Wake-Up Enable */
	    WAKE_EN1 |		/* PF8 Wake-Up Enable */
	    WAKE_EN2 |		/* PF9 Wake-Up Enable */
	    CANWE |		/* CAN Rx Wake-Up Enable */
	    HIBERNATE;
	bfrom_SysControl(SYSCTRL_WRITE | SYSCTRL_VRCTL | SYSCTRL_EXTVOLTAGE, &hibernate, NULL);
}


/* сбросить процессор */
section("L1_code")
void PLL_reset(void)
{
	bfrom_SysControl(SYSCTRL_SYSRESET, NULL, NULL); /* either */
	bfrom_SysControl(SYSCTRL_SOFTRESET, NULL, NULL); /* or */
}

