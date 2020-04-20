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

    /* Initialize DIO's used to control LEDs on DR1199 expansion board and
     * turn all LED's off */

    /* Initiate Timer mode Counter */
	vAHI_TimerEnable(E_AHI_TIMER_0, 8, TRUE, FALSE, FALSE);
	vAHI_TimerClockSelect(E_AHI_TIMER_0, TRUE, FALSE);
	vAHI_Timer0RegisterCallback(vTimer0ISR);
	vAHI_TimerConfigureInputs(E_AHI_TIMER_0, FALSE, FALSE);

	/* Start timer encoder */
	vAHI_TimerStartSingleShot(E_AHI_TIMER_0, 10000, 10000);

	static uint16_t u16Encoder = 0;

    /* Main loop, just flashes LED's */
    while(1)
    {
    	/* Read encoder */
    	if (u16Encoder != u16AHI_TimerReadCount(E_AHI_TIMER_0))
    	{
    		u16Encoder = u16AHI_TimerReadCount(E_AHI_TIMER_0);
    		DBG_vPrintf(TRUE, "New value: %d\n", u16Encoder);
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

}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
