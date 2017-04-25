#ifndef _MY_DEFS_H
#define _MY_DEFS_H

#include <signal.h>
#include <stdint.h>
#include <time.h>


/* Включения для 504 блекфина, не 506!!!!  */
#ifndef _WIN32			/* Embedded platform */
#include <adi_types.h>
#include <sys/exception.h>
#include <cdefBF504f.h>
#include <defBF504f.h>
#include <ccblkfn.h>
#include <float.h>
#include "config.h"


/* These types must be 16-bit, 32-bit or larger integer */
typedef int INT;
typedef unsigned int UINT;

/* These types must be 8-bit integer */
typedef char CHAR;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;

/* These types must be 16-bit integer */
typedef short SHORT;
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;

/* These types must be 32-bit integer */
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned long DWORD;

#else				/* windows   */
#include <windows.h>
#include <tchar.h>
#endif

#ifndef u8
#define u8 unsigned char
#endif

#ifndef s8
#define s8 char
#endif

#ifndef c8
#define c8 char
#endif

#ifndef u16
#define u16 unsigned short
#endif


#ifndef s16
#define s16 short
#endif

#ifndef i32
#define i32  int
#endif


#ifndef u32
#define u32 unsigned long
#endif


#ifndef s32
#define s32 long
#endif


#ifndef u64
#define u64 uint64_t
#endif


#ifndef s64
#define s64 int64_t
#endif

/* long double не поддержываеца  */
#ifndef f32
#define f32 float32_t
#endif


#ifndef bool
#define bool u8
#endif


#ifdef true
#define true 1
#endif

#ifdef false
#define false 0
#endif

#ifndef IDEF
#define IDEF static inline
#endif

/* На этот адрес будет отвечать наше устройство */
#define 	BROADCAST_ADDR	0xff

/* Сколько пакетов в пачке  */
#define 	NUM_ADS1282_PACK	20

#define 	ADC_CHAN			4	/* число каналов  */
#define		MAGIC				0x4b495245


/*******************************************************************************
 * Состояние устройств для описания State machine
 *******************************************************************************/
typedef enum {
	DEV_POWER_ON_STATE = 0,
	DEV_CHOOSE_MODE_STATE = 1,
	DEV_INIT_STATE,
	DEV_GET_3DFIX_STATE,
	DEV_TUNE_Q19_STATE,	/* Первичная настройка кварца 19 МГц */
	DEV_SLEEP_AND_DIVE_STATE,	/* Погружение и сон перед началом работы */
	DEV_WAKEUP_STATE,
	DEV_REG_STATE,
	DEV_FINISH_REG_STATE,
	DEV_BURN_WIRE_STATE,
	DEV_SLEEP_AND_POPUP_STATE,
	DEV_WAIT_GPS_STATE,
	DEV_HALT_STATE,
	DEV_COMMAND_MODE_STATE,
	DEV_POWER_OFF_STATE,
	DEV_ERROR_STATE = -1	/* Авария */
} DEV_STATE_ENUM;


/**
 * В этих структурах записываются параметры таймеров
 * соответствует такой записи:
 * 16.10.12 17.15.22	// Время начала регистрации
 * 17.10.12 08.15.20	// Время окончания регистрации
 * 18.10.12 11.17.00	// Время всплытия, включает 5 мин. времени пережига
 * ....
 */
#pragma pack(4)
typedef struct {
	u32 wakeup_time;	/* время начала подстройки перед регистрацией */
	u32 start_time;		/* время начала регистрации  */
	u32 finish_time;	/* время окончания регистрации */
	u32 burn_time;		/* Время начала пережига проволки  */
	u32 popup_time;		/* Момент начала всплытия */
	u32 gps_time;		/* время включения gps после времени всплытия */
	u32 am3_popup_time;	/* аварийное время всплытия от модема   */
	u8  am3_h0_time;
	u8  am3_m0_time;
	u8  am3_h1_time;
	u8  am3_m1_time;
} TIME_RUN_STRUCT;



/* Структура, описывающая время, исправим c8 на u8 - время не м.б. отрицательным */
#pragma pack(1)
typedef struct {
	u8 sec;
	u8 min;
	u8 hour;
	u8 week;		/* день недели...не используеца */
	u8 day;
	u8 month;		/* 1 - 12 */
#define mon 	month
	u16 year;
} TIME_DATE;
#define TIME_LEN  8		/* Байт */


/**
 * Этой структуры нету в заголовках   
 */
#pragma pack(4)
struct timespec {
	time_t tv_sec;		/* Секунды  */
	long   tv_nsec;		/* Наносекунды */
};


/**
 * В эту стуктуру пишется минутный заголовок при записи на SD карту (pack по 1 байту!)
 */
#pragma pack(1)
typedef struct {
	c8 DataHeader[12];	/* Заголовок данных SeismicData\0  */
	c8 HeaderSize;		/* Размер заголовка - 80 байт */
	c8 ConfigWord;
	c8 ChannelBitMap;
	u16 BlockSamples;
	c8 SampleBytes;

	union {
		TIME_DATE time;
		u32 dword[2];
		u64 qword;
	} SampleTime;		/* Минутное время сампла */

	u16 Bat;
	u16 Temp;

	u8  Rev;  		/* Ревизия = 2 всегда  */
	u16 Board;

	u8  NumberSV;
	s16 Drift;

	union {
		TIME_DATE time;
		u32 dword[2];	/* Не используется пока  */
		u64 qword;
	} SedisTime;

	union {
		TIME_DATE time;	/* Время синхронизации */
		u32 dword[2];	/* Не используется пока  */
		u64 qword;
	} GPSTime;

	union {
		struct {
			c8 rsvd2;
			bool comp;	/* компас */
			c8 pos[23];	/* Позиция (координаты) */
			c8 rsvd3[3];
		} coord;	/* координаты */

		u32 dword[7];	/* Данные */
	} params;
} ADC_HEADER;


//////////////////////////////////////////////////////////////////////////////////////////
//                        На отправление
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * Пакет данных на отправление - 20 измерений акселерометра -  240 байт, упакован на 1!!!
 */
#pragma pack(1)
typedef struct {
	u8 len;			/* Длина пакета без контрольной суммы */
	u8 adc;			/* Номер работающих АЦП - четыре бита подряд */
	u16 msec;		/* Миллисекунда первого измерения */
	u32 sec;		/* UNIX TIME первого измерения */

	struct {		/* 3-х байтный пакет (* 4) */
		unsigned x:24;
		unsigned y:24;
		unsigned z:24;
		unsigned h:24;
	} data[NUM_ADS1282_PACK];

	u16 rsvd;		/* Выравнивание */
	u16 crc16;		/* Контрольная сумма пакета  */
} ADS1282_PACK_STRUCT;



/** 
 *  Статус и ошибки устройств на отправление 64 байт. Короткий статус посылается при ошибке (только 2 байта + 1 len + 2 CRC16)
 */
#pragma pack(4)
typedef struct {
	u8  len;		/* Длина пакета без контрольной суммы */
	u8  st_main;            /* главный: нет времени, нет констант, ошибка в команде, не вовремя, исполняется, переполнение памяти, остановлен, режим работы */
	u8  st_test;            /* Самотестирование и ошибки: 0 - часов, 1 - датчик T&P, 2 - Акселер/ компас, 3 - модем, 4 - GPS, 5 - EEPROM, 6 - карта SD, 7 - flash */
	u8  st_reset;		/* Причина сброса */

	u32  rsvd0[5];		/* Резерв */

	u32  eeprom;		/* Статус EEPROM */

	s16  am_power_volt;	/* На 1 ноге-сразу 4 канала напряжения */
	s16  burn_ext_volt;	

	s16  burn_volt;
	s16  regpwr_volt;

	s16  iam_sense;	        /* 3 канала тока */
	s16  ireg_sense;	

	s16  iburn_sense;
	s16  temper;		/* Температура = t * 10 */

	u32  press;		/* Давление Па */

	s16  acc_x;		/* Акселерометр - X */
	s16  acc_y;		/* Акселерометр - Y */

	s16  acc_z;		/* Акселерометр - Z */
	s16  comp_x;		/* Компас - X */

	s16  comp_y;		/* Компас - Y */
	s16  comp_z;		/* Компас - Z */

	u16   rsvd1;
	u16   crc16;		/* Контрольная сумма пакета  */
} DEV_STATUS_STRUCT;


/**
 * Время работы прибора на отправление
 */
#pragma pack(1)
typedef struct {
	u8 len;			/* Число байт  */
	u32 time_work;		/* Время работы в режиме сбора данных */
	u32 time_cmd;		/* Время работы в командном режиме */
	u32 time_modem;		/* Время работы модема */
	u16 crc16;		/* CRC16  */
} DEV_WORK_TIME_STRUCT;


/* Имя и адрес */
#pragma pack(2)
typedef struct {
	u8  len;		/* Длина */
	u8  rev;		/* Ревизия */
	u16 addr;		/* Адрес платы */
	u32 time;		/* Время компиляции */
	u16 crc16;
} DEV_ADDR_STRUCT;


/**
 *  константы для любого канала. не будем экономить на offset : 24
 */
#pragma pack (1)
typedef struct {
	u8  len;
	u8  chan;	/* Номер канала */
	u32 offset;	/* коэффициент 1 - смещение */
	u32 gain;	/* коэффициент 2 - усиление */
	u16 crc16;
} DEV_CHAN_STRUCT;


/**
 * Внешняя команда пришла с UART
 */
#pragma pack (4)
typedef struct {
    u8  cmd;
    u8  rsvd[3];

    union {
      u8  cPar[16];
      u16 wPar[8];
      u32 lPar[4];
      f32 fPar[4];
    } u;
} DEV_UART_CMD;

/**
 * Что будем хранить в eeprom в виде u32
 * Внутри eeprom овских функций - адрес * 2 и + 1 2-е слово
 * Дописывать ТОЛЬКО с предпоследнего места!!!
 * Или стирать послностью флеш перед работой!
 */
enum eeprom_id {
   EEPROM_MOD_ID = 0,
   EEPROM_RSVD0,
   EEPROM_RSVD1,
   EEPROM_TIME_WORK,
   EEPROM_TIME_CMD,
   EEPROM_TIME_MODEM,
   EEPROM_DAC19_COEF,
   EEPROM_DAC4_COEF,
   EEPROM_RSVD2,
   EEPROM_ADC_OFS0,
   EEPROM_ADC_FSC0,
   EEPROM_ADC_OFS1,
   EEPROM_ADC_FSC1,
   EEPROM_ADC_OFS2,
   EEPROM_ADC_FSC2,
   EEPROM_ADC_OFS3,
   EEPROM_ADC_FSC3,
   EEPROM_RESET_CAUSE,
   EEPROM_END_OF_VAR
};


#pragma pack(4)

#endif				/* my_defs.h */
