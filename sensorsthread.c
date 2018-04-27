/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    sensorsthread.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "sensorsthread.h"
#include "timers.h"
#include "pixy.h"
#include "queue.h"
#include "sensor_queue.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************


// My timer handle
TimerHandle_t sensorTimer;

sensor_msg data;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

SENSORSTHREAD_DATA sensorsthreadData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

void sensorTimerCallback(sensorTimer) {
    SYS_PRINT("In the Sensor Timer \n");
    
    readSignature(0x54);
    //PLIB_ADC_SampleAutoStartEnable(0);
    convertIR();
    writeToQueue(data);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SENSORSTHREAD_Initialize ( void )

  Remarks:
    See prototype in sensorsthread.h.
 */

void SENSORSTHREAD_Initialize ( void )
{
       sensorTimer = xTimerCreate("sensorTimer", // Just a text name, not used by the kernel.
            pdMS_TO_TICKS(100), // The timer period in ticks.
            pdTRUE, // The timers will auto-reload themselves when they expire.
            (void *) 0, // Assign each timer a unique id equal to its array index.
            sensorTimerCallback // Each timer calls the same callback when it expires.
            );

    xTimerStart(sensorTimer, 0);
}


/******************************************************************************
  Function:
    void SENSORSTHREAD_Tasks ( void )

  Remarks:
    See prototype in sensorsthread.h.
 */

void SENSORSTHREAD_Tasks ( void )
{

   configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
        setUpPixy();
        setUpADC();
        uint16_t prev_cc =0;
        uint16_t sec_prev_cc =0;
        char * Sensor_Msg;
        while(1)
        {

            data = readFromQueue(msgQ);
            SYS_PRINT("IR Sensor val: %d\n",data.sensor_value);
            SYS_PRINT(" CC_CODE %d\n", data.cc_code);
            if (data.cc_code == 10 && prev_cc == data.cc_code && sec_prev_cc == data.cc_code)
            {
                SYS_PRINT("No change in direction\n");   
            }
            else if (data.cc_code == 11&& prev_cc == data.cc_code && sec_prev_cc == data.cc_code)
            {
                SYS_PRINT("Right\n");
            }
            else if (data.cc_code == 19&& prev_cc == data.cc_code && sec_prev_cc == data.cc_code)
            {
                SYS_PRINT("Left\n");                
            }
            sec_prev_cc = prev_cc;
            prev_cc =data.cc_code;
//            
        
            
        }
}

 

/*******************************************************************************
 End of File
 */
