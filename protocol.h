#ifndef _HALCONF_H_
#include "hal.h"
#endif
#ifndef _CH_H_
#include "ch.h"
#endif


/*Functions*/
void init_protocol(void);
void* rxListen(void*);
void *txWrite(void*);
