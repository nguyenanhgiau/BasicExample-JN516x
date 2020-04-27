/****************************************************************************/
/*
 * MODULE              JN-AN-1193 JTAG Debugger
 *
 * DESCRIPTION         Simple LED blinking application.
 */
/*****************************************************************************
 *
 * This software is owned by NXP B.V. and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on NXP products [NXP Microcontrollers such as JN5168, JN5164,
 * JN5161, JN5148, JN5142, JN5139].
 * You, and any third parties must reproduce the copyright and warranty notice
 * and any other legend of ownership on each copy or partial copy of the
 * software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Copyright NXP B.V. 2012. All rights reserved
 *
 ****************************************************************************/
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi.h>
#include "dbg.h"
#include "dbg_uart.h"
#include "AppHardwareApi_JN516x.h"
#include "twi_drv.h"
#include "sht31.h"


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* Define which DIO is used to drive LED3 on DR1199 */
#define RELAY_1_PIN			(11)
#define RELAY_2_PIN			(12)
#define RELAY_3_PIN			(13)
#define RELAY_4_PIN			(17)

#define ZERROCROSSS_PIN		(8)

#define PIN_MASK(x)			(1UL << x)

#define TURNON_ALL_RL
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
//PRIVATE void vTimer0ISR(uint32 u32DeviceId, uint32 u32ItemBitmap);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
//static bool bOn = FALSE;
//const uint16 DUTY_CYCLE = 100;
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

	/* Disable watchdog if enabled by default */
	#ifdef WATCHDOG_ENABLED
		vAHI_WatchdogStop();
	#endif

	/* Initialize the Hardware Interfaces */
    (void)u32AHI_Init();

    DBG_vUartInit(DBG_E_UART_0, DBG_E_UART_BAUD_RATE_115200);

    DBG_vPrintf(TRUE,"\n");
	DBG_vPrintf(TRUE, "***********************************************\n");
	DBG_vPrintf(TRUE, "* SWITCH-LIGHT RESET                          *\n");
	DBG_vPrintf(TRUE, "***********************************************\n");

    /* Initialize DIO's used to control LEDs on DR1199 expansion board and
     * turn all LED's off */
//    vAHI_DioSetDirection(0, PIN_MASK(RELAY_1_PIN));
    vAHI_DioSetDirection(0, PIN_MASK(4));
    vAHI_DioSetDirection(0, PIN_MASK(5));
    vAHI_DioSetOutput(PIN_MASK(4), 0);
    vAHI_DioSetOutput(PIN_MASK(5), 0);

//    vAHI_DioSetDirection(PIN_MASK(ZERROCROSSS_PIN), 0);
//
//    vTWIBusInit();
//
//    if (sht31_init())
//    	DBG_vPrintf(TRUE, "Init SHT31 successful\n");
//    else
//    	DBG_vPrintf(TRUE, "Init SHT31 fail\n");
//
//
//    float temp, humi;


    /* Initiate Timer*/
    // clock is 16MHz so the time period is 0.0625micro seconds
    // we are using pre-scaler value of 8 so it will divide the
    // clock by 2^8 = 256 hence multiplying the time period by 256
    // so the timer clock will be 0.0625 * 256 = 16 ,micro sec
//    vAHI_TimerEnable(E_AHI_TIMER_0, 8, FALSE, TRUE, FALSE);
//    vAHI_TimerClockSelect(E_AHI_TIMER_0, FALSE, FALSE);
//    // Registering the callback function
//    vAHI_Timer0RegisterCallback(vTimer0ISR);
//    // Starting the timer for single shot
//    vAHI_TimerStartSingleShot(E_AHI_TIMER_0, 500, 62);

    /* Main loop, just flashes LED's */
    while(1)
    {
    	vAHI_DioSetOutput(PIN_MASK(4), 0);
		vAHI_DioSetOutput(PIN_MASK(5), 0);
    	sht31_delay_ms(1000);
    	vAHI_DioSetOutput(0, PIN_MASK(4));
		vAHI_DioSetOutput(0, PIN_MASK(5));
		sht31_delay_ms(1000);
//    	if (sht31_read_value(&temp, &humi)) {
//    		DBG_vPrintf(TRUE, "========================================\n");
//			DBG_vPrintf(TRUE, "Temperature: %d.%d\n", (uint16)temp, ((uint16)(temp*100))%100);
//			DBG_vPrintf(TRUE, "Humidity: %d.%d\n", (uint16)humi, ((uint16)(humi*100))%100);
//		}
//		else {
//			DBG_vPrintf(TRUE, "Checksum fail");
//		}
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
//PRIVATE void vTimer0ISR(uint32 u32DeviceId, uint32 u32ItemBitmap) {
//     if (bOn) {
//    	 vAHI_TimerStartSingleShot(E_AHI_TIMER_0, 500, 563);
//    	 vAHI_DioSetOutput(PIN_MASK(RELAY_1_PIN), 0);
//     }
//     else {
//    	 vAHI_TimerStartSingleShot(E_AHI_TIMER_0, 500, 62);
//    	 vAHI_DioSetOutput(0, PIN_MASK(RELAY_1_PIN));
//     }
//     bOn = !bOn;
//}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
