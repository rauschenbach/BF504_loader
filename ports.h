#ifndef _PORTS_H
#define _PORTS_H

#include "globdefs.h"
#include "rele.h"

void init_bf_ports(void);
void init_atmega_ports(void);
void select_sdcard_to_bf(void);
void unselect_ports(void);

#endif /* portc.h  */

