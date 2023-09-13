/*
===============================================================================
 Name        : 18_Day43_20102021_ADC003.c

 Description : Program demonstrate ADC Interrupt method hardware controlled
 	 	 	   start of conversion burst mode enabled and displaying on LCD

 

 Layered Architecture used for this project
 ************************************
 Application layer-18_Day43_20102021_ADC003.c
 ************************************
 Board layer - lcd.c/.h, configboard.h
 ************************************
 Low level drivers or chip level - pinmux.c/.h,adc.c/.h, gpio.c/.h
 ************************************
 Hardware
 ************************************

===============================================================================
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
#include "pinmux.h"
#include "adc.h"
#include "lcd.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define DISPTIMEOUT 	0xFFFFF
#define CONVTIMEOUT	    0x2FFFF
#define CLEARTIMEOUT	0x1FFFF
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
/**
  * @brief  Initialize all the hardware connected
  * @retval none
  */
void vAppHardwareInit(void)
{
	vPinmuxInitialize();
	vLCDInitialize();
	vLCDInitialCommands();
	vADCInitialize();

}

/**
  * @brief  Crude Delay
  * @retval none
  */
void vAppDelay(uint32_t count)
{
	int i,j;
	for(i=0;i<count;i++)
		for(j=0;j<0xA00;j++);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
	uint32_t delaycount=0,readADCGDRdata=0,channelno=0;
	uint32_t adcresult[8]={0};

  /* Initialize all configured peripherals */
   vAppHardwareInit();

   vADCMultiChannelSel(ADCCHAN0);
  // vADCMultiChannelSel(ADCCHAN1);
  // vADCMultiChannelSel(ADCCHAN6);
  // vADCMultiChannelSel(ADCCHAN7);

   /* Enabling the burst mode as we are going to use hardware controlled starting mechanism */
   vADCBurstMode(BURSTENABLE);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)  // for(;;)
  {

  delaycount++;
		switch (delaycount) {
		case CLEARTIMEOUT:
			vLCDClear();
			break;
		case DISPTIMEOUT:
			/* Reading the global data register into a variable */
			readADCGDRdata = LPC_ADC->ADGDR;

			/* Extracting the channel no from the global data register which is in 26:24 bits */
			channelno = (readADCGDRdata >> 24) & 0x07;

			/* Extracting the adc result from the global data register which is in 15:4 bits */
			adcresult[channelno] = (readADCGDRdata >> 4) & 0xFFF;

			/* Displaying the result on the LCD */
			vLCDGotoxy(LINE0,COL0);
			vLCDPrintf("%d=%d",0,adcresult[0]);

/*			vLCDGotoxy(LINE0,COL8);
			vLCDPrintf("%d=%d",1,adcresult[1]);

			vLCDGotoxy(LINE1,COL0);
			vLCDPrintf("%d=%d",6,adcresult[6]);

			vLCDGotoxy(LINE1,COL8);
			vLCDPrintf("%d=%d",7,adcresult[7]);
			delaycount=0;*/


			break;

		default:
			/* DO NOTHING */
			break;
		}

  }
  /* End of Application entry point */
}




