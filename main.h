#ifndef _HALCONF_H_
#include "hal.h"
#endif
#ifndef _CH_H_
#include "ch.h"
#endif

#include "protocol.h"

#define CCA_EXP_IO_MODE_OUTPUT 0
#define CCA_EXP_IO_MODE_INPUT 1


static THD_WORKING_AREA(serialListen, 128);
static THD_WORKING_AREA(serialTalk, 128);

void init_IO(void);
void init_globals(void);
