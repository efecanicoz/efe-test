#include "protocol.h"

uint8_t * txMailboxArea[16];
mailbox_t serialMbox;

memory_pool_t mpool;
MEMORYPOOL_DECL(mpool, sizeof(uint8_t *), NULL);//sayı kaç olmalı belli değil
uint8_t protocol_01_buffer[64] __attribute__((aligned(sizeof(stkalign_t))));

void init_protocol(void)
{
	int i;
	chMBObjectInit(&serialMbox, txMailboxArea, 16);
	
	
	chPoolObjectInit(&mpool, sizeof(uint8_t *), NULL);
	/*for(i = 0; i < 64; i++)
	{
		chPoolAdd(&mpool, &protocol_01_buffer[i]);
	}*/
	chPoolLoadArray(&mpool,protocol_01_buffer,64);//acayip yerlere gönderiyo 2.ve sonraki malloclar için
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
	uint8_t *buffer;
	while(!0)
	{
		buffer = (uint8_t *)chPoolAlloc(&mpool);
		if(buffer != NULL)
		{
			sdRead(&SD3, buffer, 1);
			chMBPost(&serialMbox, buffer, TIME_INFINITE);//sonsuza kadar göndermek için beklesin mi yoksa bi yerden sonra pes etsin mi?
			chThdSleepMilliseconds(100);
		}
	}
	return NULL;
}

void *txWrite(void *arg)
{
	uint8_t *toSend;
	while(!0)
	{
		if(chMBFetch(&serialMbox, &toSend, TIME_INFINITE) == MSG_OK)
		{
			sdWrite(&SD3, toSend,1);
			chPoolFree(&mpool, toSend);
		}
		chThdSleepMilliseconds(500);
	}
	return NULL;
}
