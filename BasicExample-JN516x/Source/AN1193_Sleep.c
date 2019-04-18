/****************************************************************************/
/*
 * MODULE              JN-AN-1193 JTAG Debugger
 *
 * DESCRIPTION         Demonstration of terminal with DBG_vPrintf()
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
#include "AppHardwareApi.h"
#include "dbg.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Define which DIO is used to drive LED3 on DR1199 */
#define LED_3_DIO			(0)
#define LED_3_DIO_MASK		(1UL << LED_3_DIO)



/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
uint32 u32DebugRegisters[AHI_STORE_DEBUG_REGISTER_COUNT];
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


PUBLIC void AppColdStart(void)
{
	volatile uint32 i;

    vAHI_WatchdogStop();
    (void)u32AHI_Init();

    /* Initialize DIO's used to control LEDs on DR1199 expansion board */
    vAHI_DioSetDirection(0, LED_3_DIO_MASK);

    /* LED off and delay to stay awake */
    vAHI_DioSetOutput(0, LED_3_DIO_MASK);
	/* Delay to provide LED on while awake*/
	for(i=0; i<1000000; i++)
	{

	}

    /* LED on and go to sleep for one second*/
    vAHI_DioSetOutput(LED_3_DIO_MASK, 0);

    vAHI_WakeTimerEnable(E_AHI_WAKE_TIMER_0, TRUE);
    vAHI_WakeTimerStartLarge(E_AHI_WAKE_TIMER_0, 32000);
    /* Save the debug registers before sleeping */
    vAHI_StoreDebug(u32DebugRegisters);
    vAHI_Sleep(E_AHI_SLEEP_OSCON_RAMON);
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
    vAHI_RestoreDebug(u32DebugRegisters);
    AppColdStart();
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
