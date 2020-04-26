/*
 * Encoder.c
 *
 *  Created on: Apr 20, 2020
 *      Author: Giau
 */


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>
#include "dbg.h"
#include "dbg_uart.h"
#include "AppHardwareApi_JN516x.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define MOTOR_DIR_DIO			(12)
#define MOTOR_DIR_DIO_MASK		(1UL << MOTOR_DIR_DIO)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vTimer0ISR(uint32 u32DeviceId, uint32 u32ItemBitmap);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: AppColdStart
 *
 * DESCRIPTION:
 * Entry point for application from boot loader. Initializes system and runs
 * main loop.
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC void AppColdStart(void)
{

	// Wait until FALSE i.e. on XTAL  - otherwise uart data will be at wrong speed
	while (bAHI_GetClkSource() == TRUE);
	bAHI_SetClockRate(3); /* Move CPU to 32 MHz  vAHI_OptimiseWaitStates automatically called */

	/* Disable watch dog if enabled by default */
	#ifdef WATCHDOG_ENABLED
		vAHI_WatchdogStop();
	#endif

	/* Initialize the Hardware Interfaces */
    (void)u32AHI_Init();

    DBG_vUartInit(DBG_E_UART_0, DBG_E_UART_BAUD_RATE_115200);

    DBG_vPrintf(TRUE,"\n");
	DBG_vPrintf(TRUE, "***********************************************\n");
	DBG_vPrintf(TRUE, "* SMART-CURTAIN RESET                         *\n");
	DBG_vPrintf(TRUE, "***********************************************\n");

	/* Initiate Timer mode Counter */
	vAHI_TimerEnable(E_AHI_TIMER_0, 0, TRUE, TRUE, FALSE);
	vAHI_TimerClockSelect(E_AHI_TIMER_0, TRUE, FALSE);
	vAHI_Timer0RegisterCallback(vTimer0ISR);
	vAHI_TimerConfigureInputs(E_AHI_TIMER_0, FALSE, FALSE);

	/* Start timer encoder */
	vAHI_TimerStartSingleShot(E_AHI_TIMER_0, 65000, 65000);


	/* Initialize DIO's used to control LEDs on DR1199 expansion board and
	 * turn all LED's off */
	vAHI_DioSetDirection(0, MOTOR_DIR_DIO_MASK);
	vAHI_DioSetOutput(MOTOR_DIR_DIO_MASK, 0);

	/* Configure timer 1 to generate a PWM output on its output pin */
	vAHI_TimerEnable(E_AHI_TIMER_1, 4, FALSE, FALSE, TRUE);
	vAHI_TimerConfigureOutputs(E_AHI_TIMER_1, FALSE, TRUE);
	vAHI_TimerClockSelect(E_AHI_TIMER_1, FALSE, TRUE);
	/* set duty cycle 100/255 */
	vAHI_TimerStartRepeat(E_AHI_TIMER_1, 100, 255);

	/* Initialize adc */
	vAHI_ApConfigure(E_AHI_AP_REGULATOR_ENABLE,E_AHI_AP_INT_DISABLE,E_AHI_AP_SAMPLE_2,E_AHI_AP_CLOCKDIV_500KHZ,E_AHI_AP_INTREF);
	vAHI_AdcEnable(E_AHI_ADC_CONTINUOUS,E_AHI_AP_INPUT_RANGE_1,E_AHI_ADC_SRC_ADC_1);
	vAHI_AdcStartAccumulateSamples(E_AHI_ADC_ACC_SAMPLE_16);
	vAHI_AdcStartSample();

	static uint16_t u16Encoder = 0;

    /* Main loop, just flashes LED's */
    while(1)
    {
    	/* Read encoder */
    	if (u16Encoder != u16AHI_TimerReadCount(E_AHI_TIMER_0))
    	{
    		u16Encoder = u16AHI_TimerReadCount(E_AHI_TIMER_0);
    		DBG_vPrintf(TRUE, "New value: %d\n", u16Encoder);
    		DBG_vPrintf(TRUE, "ADC value: %d\n", u16AHI_AdcRead());
    	}
    }
}

/****************************************************************************
 *
 * NAME: AppWarmStart
 *
 * DESCRIPTION:
 * Entry point for application from boot loader. Simply jumps to AppColdStart
 * as, in this instance, application will never warm start.
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC void AppWarmStart(void)
{
    AppColdStart();
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
PRIVATE void vTimer0ISR(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	DBG_vPrintf(TRUE, "Interrupt timer0\n");
	/* restart timer */
	vAHI_TimerStartSingleShot(E_AHI_TIMER_0, 10, 10);

	if (E_AHI_TIMER_RISE_MASK & u32ItemBitmap)
	{
		// handle interrupt over shot threshold
		DBG_vPrintf(TRUE, "Interrupt rise timer\n");
	}
	else if (E_AHI_TIMER_PERIOD_MASK & u32ItemBitmap)
	{
		DBG_vPrintf(TRUE, "Interrupt period timer\n");
	}
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
