#include "main.h"

uint8_t cca_exp_io_mode;


int main(void) {
	halInit();
	chSysInit();
	thread_t *listener,*writer;
	
	
	palWriteGroup(GPIOD,0xFFFFU,0,0x0000U);//all ports low
	chThdSetPriority(NORMALPRIO);
	init_IO();
	init_protocol();
    listener = chThdCreateStatic(serialListen, sizeof(serialListen), LOWPRIO, rxListen, NULL);
    //writer = chThdCreateStatic(serialTalk, sizeof(serialTalk), LOWPRIO, txWrite, NULL);
    
	while(!0)
	{
		palWriteGroup(GPIOD,0xFFFFU,0,0xFFFFU);//all ports high
		chThdSleepMilliseconds(1000);
		palWriteGroup(GPIOD,0xFFFFU,0,0x0000U);
		chThdSleepMilliseconds(1000);
	}
	chThdWait(listener);
	return 0;/*never reach here*/
}


void init_IO(void)
{
	//adc_configure();
	//bütün b portlarını (16 tane) pull up input olarak aç
	palSetGroupMode(GPIOB, PAL_GROUP_MASK(16), 0, PAL_MODE_INPUT_PULLUP);
	
	//set_cca_exp_io_mode();
	
	if( cca_exp_io_mode == CCA_EXP_IO_MODE_OUTPUT)
	{
		palSetGroupMode(GPIOD, PAL_GROUP_MASK(16), 0, PAL_MODE_OUTPUT_PUSHPULL);
		palSetGroupMode(GPIOE, PAL_GROUP_MASK(16), 0, PAL_MODE_OUTPUT_PUSHPULL);
	}
	else if( cca_exp_io_mode == CCA_EXP_IO_MODE_INPUT)
	{
		palSetGroupMode(GPIOD, PAL_GROUP_MASK(16), 0, PAL_MODE_INPUT_PULLUP);
		palSetGroupMode(GPIOE, PAL_GROUP_MASK(16), 0, PAL_MODE_INPUT_PULLUP);
	}
	else
	{
		;// Never reach here
	}
	return;
}

void set_cca_exp_io_mode(void)
{
	//TODO: düzelt burayı
	cca_exp_io_mode = CCA_EXP_IO_MODE_OUTPUT;
	// if bit7 is 1: this is 1nput module
	// if bit7 is 0: this is 0utput module
	//cca_exp_io_mode = (map_addrsw_to_logical(~GPIOB->IDR) >> 7);
	return;
}

void init_globals(void)
{
	/*initialize mailboxes*/
	/*SOR:buffersize yukarıda makro oldu sıkıntı olur mu?*/
	//chMBObjectInit(txBuffer, txMailboxBuffer, BUFFERSIZE);
	//chMBObjectInit(rxBuffer, rxMailboxBuffer, BUFFERSIZE);
	return;
}
