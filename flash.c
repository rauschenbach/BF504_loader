#include "flash.h"

#define ERASE_TIMEOUT			50000000
#define PROGRAM_TIMEOUT			1000000


/**
 * Для работы с flash я выдернул их сюда!
 */
static int PollToggleBit(u32, u32);
static bool IsStatusReady(u32);
static int WriteFlash(u32, u16);
static int ReadFlash(u32, u16 *);


/**
 * Init Flash. This function initializes parallel flash.
 */
section("L1_code")
int FLASH_init(void)
{
    *pEBIU_MODE = 0x1;
    ssync();

    *pFLASH_CONTROL_CLEAR = FLASH_ENABLE;	/* Reset the flash */
    asm("ssync;nop;nop;nop;nop;nop;nop;nop;");
    asm("ssync;nop;nop;nop;nop;nop;nop;nop;");
    asm("ssync;nop;nop;nop;nop;nop;nop;nop;");
    asm("ssync;nop;nop;nop;nop;nop;nop;nop;");

    *pFLASH_CONTROL_SET = FLASH_ENABLE;	/* Release flash from reset state */
    asm("ssync;nop;nop;nop;nop;nop;nop;nop;");

    *pFLASH_CONTROL_SET = FLASH_UNPROTECT;	/* Unprotect the flash to enable program/erase */
    asm("ssync;nop;nop;nop;nop;nop;nop;nop;");

    *pEBIU_AMBCTL = 0xFFC2FFC2;	/* Adjust the timing parameters in the EBIU_AMBCTL register */
    asm("ssync;nop;nop;nop;nop;nop;nop;nop;");

    return 0;
}

/**
 *   Удалить сектор, передаеца адрес сектора для удаления
 */
section("L1_code")
int FLASH_erase_page(u32 addr)
{
    int ErrorCode = 0;		//tells us if there was an error erasing flash

    // unlock this block
    WriteFlash(addr, 0x60);
    WriteFlash(addr, 0xD0);
    ErrorCode = PollToggleBit(addr, ERASE_TIMEOUT);

    // erase block
    WriteFlash(addr, 0x20);
    WriteFlash(addr, 0xD0);
    ErrorCode = PollToggleBit(addr, ERASE_TIMEOUT);

    WriteFlash(addr, 0xFF);

    return ErrorCode;
}

/**
 * Записать 16-ти битное число
 */
section("L1_code")
int FLASH_program_half_word(u32 addr, u16 data)
{
    int res;

    /* get flash start address from absolute address */

    // send the unlock command to the flash
    WriteFlash(addr, 0x60);
    WriteFlash(addr, 0xD0);
    res = PollToggleBit(addr, PROGRAM_TIMEOUT);

    WriteFlash(addr, 0x40);
    WriteFlash(addr, data);	// program our actual value now
    res = PollToggleBit(addr, PROGRAM_TIMEOUT);	// make sure the write was successful
    return res;
}



section("L1_code")
int FLASH_write_buf(u32 addr, u8 * buf, int len)
{
    int i;
    int res;

    for (i = 0; i < len; i += 2) {
	res = FLASH_program_half_word(addr + i,*(u16*)(buf + i));
    }

    return res;
}




/**
 *  	Write a value to an address in flash.
 */
section("L1_code")
static int WriteFlash(unsigned long ulAddr, unsigned short usValue)
{
    // disable interrupts before performing the load or store operation
    // [refer warning: page 6-71 BF533 HRM]
    unsigned int uiSaveInts = cli();
    unsigned short *pFlashAddr = (unsigned short *) (ulAddr);	// set the address
    *pFlashAddr = usValue;
    sti(uiSaveInts);		// Enable Interrupts
    return 0;			// ok
}




/**
 *  	Reads a value from an address in flash.
 */
section("L1_code")
static int ReadFlash(unsigned long ulAddr, unsigned short *pusValue)
{
    // disable interrupts before performing the load or store operation
    // [refer warning: page 6-71 BF533 HRM]
    u32 uiSaveInts = cli();
    u16 *pFlashAddr = (unsigned short *) (ulAddr);	// set our flash address to where we want to read
    *pusValue = (unsigned short) *pFlashAddr;	// read the value
    sti(uiSaveInts);		// Enable Interrupts
    return 0;			// ok
}

section("L1_code")
static bool IsStatusReady(unsigned long ulOffset)
{
    unsigned short status = 0;

    WriteFlash(ulOffset, 0x0070);
    ReadFlash(ulOffset, &status);

    if ((status & 0x80) == 0x80) {
	WriteFlash(ulOffset, 0x0050);
	return true;
    } else
	return false;
}

/**
 * Polls the toggle bit in the flash to see when the operation
 */
section("L1_code")
static int PollToggleBit(unsigned long ulOffset, unsigned long timeout)
{

    while (timeout) {

	if (IsStatusReady(ulOffset)) {
	    WriteFlash(ulOffset, 0xFF);
	    return 0;
	}
	timeout--;
    }

    return -1;
}
