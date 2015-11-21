#include "protocol.h"

msg_t txMailboxArea[16];
mailbox_t serialMbox;

/*MEMORYPOOL_DECL(mpool, 64, NULL);//sayı kaç olmalı belli değil
msg_t protocol_01_buffer[64];*/

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
	uint8_t buffer;
	while(!0)
	{
		//buffer = (msg_t *)chPoolAlloc(&mpool);
		sdRead(&SD3, &buffer, 1);
		chMBPost(&serialMbox, (msg_t) buffer, 10);
		chThdSleepMilliseconds(100);
	}
	return NULL;
}

void *txWrite(void *arg)
{
	msg_t *toSend;
	while(!0)
	{
		if(chMBFetch(&serialMbox, toSend, 1000) == MSG_OK)
		{
			sdWrite(&SD3, (uint8_t *)toSend,1);
		}
		//toSend = (msg_t)*ptrtoSend;
		//chPoolFree(&mpool, &toSend);
		chThdSleepMilliseconds(500);
	}
	return NULL;
}
