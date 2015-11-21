#include "protocol.h"

protocol_01 * recieveboxArea[16];
mailbox_t recievebox;

memory_pool_t packetPool;
protocol_01 protocol_01_buffer[32] __attribute__((aligned(sizeof(stkalign_t))));

/*
uint8_t * txMailboxArea[16];
mailbox_t serialMbox;

memory_pool_t mpool;
uint8_t protocol_01_buffer[64] __attribute__((aligned(sizeof(stkalign_t))));*/

//init
void init_protocol(void)
{
	
	/*palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));
	sdStart(&SD1, NULL);*///sd1 onun bunun çocuğu mu neden çalışmıyo insanı delirtiyo ?
	//chMBObjectInit(&serialMbox, txMailboxArea, 16);
	//chPoolObjectInit(&mpool, sizeof(void *), NULL);//uint8_t << void * thus we write void *
	//chPoolLoadArray(&mpool,protocol_01_buffer,64);//acayip yerlere gönderiyo 2.ve sonraki malloclar için
	
	chMBObjectInit(&recievebox, recieveboxArea, 16);
	chPoolObjectInit(&packetPool, sizeof(protocol_01), NULL);
	chPoolLoadArray(&packetPool,protocol_01_buffer,32);
	
	palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(7)); // used function : USART3_TX
	palSetPadMode(GPIOB, 11, PAL_MODE_ALTERNATE(7)); // used function : USART3_RX
	sdStart(&SD3, NULL);
	return;
}

protocol_01* new_protocol_01(uint32_t device_id)
{
	protocol_01 *self = (protocol_01 *)chPoolAlloc(&packetPool);
	self->super.protocol_id = 1;
	self->super.self_id = device_id;
	self->super.frame_start = SOH;
	self->super.frame_end = EOT;
	self->frame_size = 25;
	return self;
}


void del_protocol_01(protocol_01 *frame_to_del)
{
	chPoolFree(&packetPool, frame_to_del);
	return;
}


void *rxListen(void *arg)
{
	(void)(arg);
	uint8_t i = 0;
	char buffer[64];
	protocol_01 *temp;
	while(!0)//classic infinite thread loop
	{
		while(i < 25)
		{
			sdRead(&SD3, &buffer[i], 1);//wait until read
			i++;
		}
		//control if message valid
		if(buffer[0] != SOH || buffer[24] != EOT)//if its not a valid frame
		{
			uint8_t last = i, index;
			i = 0;
			while(buffer[i] != SOH)//find a start
				i++;
			for(index = 0; index < last; index++)//copy contents to head
				buffer[index] = buffer[i++];
			i = index;
			continue;
			/*Turn back to start of loop, maybe i is less than 25*/
		}
		else//valid message
		{
			/*Maybe first check if this packet to us
			 * if it isn't then we don't need to do all this stuff*/
			uint16_t crc = 0, index;
			temp = (protocol_01 *)chPoolAlloc(&packetPool);
			//0 is SOH
			temp->src = 0;//do i need to do this ?
			temp->dest = 0;
			temp->digital_io = 0;
			temp->analog_io[0] = 0;
			temp->analog_io[1] = 0;
			temp->analog_io[2] = 0;
			temp->analog_io[3] = 0;
			
			temp->src &= buffer[2] << 24;
			temp->src &= buffer[3] << 16;
			temp->src &= buffer[4] << 8;
			temp->src &= buffer[5];
			temp->dest &= buffer[6] << 24;
			temp->dest &= buffer[7] << 16;
			temp->dest &= buffer[8] << 8;
			temp->dest &= buffer[9];
			temp->digital_io &= buffer[10] << 24;
			temp->digital_io &= buffer[11] << 16;
			temp->digital_io &= buffer[12] << 8;
			temp->digital_io &= buffer[13];
			temp->analog_io[0] &= buffer[14] << 8;
			temp->analog_io[0] &= buffer[15];
			temp->analog_io[1] &= buffer[16] << 8;
			temp->analog_io[1] &= buffer[17];
			temp->analog_io[2] &= buffer[18] << 8;
			temp->analog_io[2] &= buffer[19];
			temp->analog_io[3] &= buffer[20] << 8;
			temp->analog_io[3] &= buffer[21];
			crc &= buffer[22] << 8;
			crc &= buffer[23];
			//24 is EOT
			/*CONTROL CRC*/
			//if(crc control)//no crc for this time
			//{
				/*push to rx buffer*/
				chMBPost(&recievebox, temp, TIME_INFINITE);//dikkat--zaman sonsuz mu olmalı
				
				/*move next part to start of the buffer*/
				for(index = 0; index + 25 < i; index++)
					buffer[index] = buffer[25 + index];
				i = i - 25;
			//}
			//else/*if crc fails remove prepared frame*/
			//{
				chPoolFree(&packetPool,temp);
				
				for(index = 0; index + 1 < i; index++)/*There may some other packet start within so just remove SOH*/
					buffer[index] = buffer[1 + index];
				i = i - 1; 
			//}
		}
	}
	return NULL;/*Never reach here*/
}


void *txWrite(void *arg)
{
	(void)(arg);
	protocol_01 *toSend;
	while(!0)
	{
		if(chMBFetch(&recievebox, &toSend, TIME_INFINITE) == MSG_OK)//semafora gerek yok kendi kendine waiting sırasına giriyo(içeride zaten semafor var)
		{
			sdWrite(&SD3, toSend,1);
			chPoolFree(&packetPool, toSend);
		}
	}
	return NULL;
}
/*
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
}*/
