#include "protocol.h"


static msg_t txMailboxArea[16];
mailbox_t serialMbox;

/*MEMORYPOOL_DECL(mpool, 64, NULL);//sayı kaç olmalı belli değil
msg_t protocol_01_buffer[64];*/
msg_t buffer;

void init_protocol(void)
{
	buffer = 0;
	chMBObjectInit(&serialMbox, txMailboxArea, 16);
	//chPoolLoadArray(&mpool,protocol_01_buffer,64);
	palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));
	sdStart(&SD1, NULL);
	return;
}

void *rxListen(void *arg)
{
	//msg_t *buffer;
	while(!0)
	{
		//buffer = (msg_t *)chPoolAlloc(&mpool);
		sdRead(&SD1, &buffer, 2);
		chThdSleepMilliseconds(100);
		chMBPost(&serialMbox, buffer, TIME_IMMEDIATE);
	}
	return NULL;
}

void *txWrite(void *arg)
{
	msg_t toSend;
	while(!0)
	{
		if(chMBFetch(&serialMbox, &toSend, TIME_IMMEDIATE) == MSG_OK)
		{
			//palWriteGroup(GPIOD,0xFFFFU,0,toSend);
			uint16_t a = sdWrite(&SD1, &toSend,2);
			palWriteGroup(GPIOD,0xFFFFU,0,a);
		}
		//toSend = (msg_t)*ptrtoSend;
		//chPoolFree(&mpool, &toSend);
		chThdSleepMilliseconds(500);
		
	}
	return NULL;
}
