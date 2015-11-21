#include "protocol.h"


static msg_t txMailboxArea[16];
mailbox_t serialMbox;

/*MEMORYPOOL_DECL(mpool, 64, NULL);//sayı kaç olmalı belli değil
msg_t protocol_01_buffer[64];*/
uint8_t buffer[2];

void init_protocol(void)
{
	chMBObjectInit(&serialMbox, txMailboxArea, 16);
	//chPoolLoadArray(&mpool,protocol_01_buffer,64);
	/*palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));
	sdStart(&SD1, NULL);*///sd1 onun bunun çocuğu mu neden çalışmıyo insanı delirtiyo ?
	palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(7)); // used function : USART3_TX
	palSetPadMode(GPIOB, 11, PAL_MODE_ALTERNATE(7)); // used function : USART3_RX

	sdStart(&SD3, NULL);
	return;
}

void *rxListen(void *arg)
{
	//msg_t *buffer;
	while(!0)
	{
		//buffer = (msg_t *)chPoolAlloc(&mpool);
		sdRead(&SD3, buffer, 2);
		chThdSleepMilliseconds(100);
		sdWrite(&SD3, buffer,2);
		chThdSleepMilliseconds(100);
		//chMBPost(&serialMbox, buffer, TIME_IMMEDIATE);
	}
	return NULL;
}

void *txWrite(void *arg)
{
	uint8_t *toSend;
	while(!0)
	{
		//sdWrite(&SD1, (uint8_t *)buffer,2);
		
		if(chMBFetch(&serialMbox, toSend, TIME_IMMEDIATE) == MSG_OK)
		{
		}
		//toSend = (msg_t)*ptrtoSend;
		//chPoolFree(&mpool, &toSend);
		chThdSleepMilliseconds(500);
		
	}
	return NULL;
}
