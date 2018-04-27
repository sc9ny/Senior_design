/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    motors.c
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

// Other useful libs
#include "motors.h"
#include "uart_queue.h"
#include "timers.h"
#include "debug.h"
#include "task.h"
#include "queue.h"
#include "parser.h"

// Standard C libs
#include "string.h"
#include <stdio.h>


// Generated Google Proto Headers
#include "motor.pb.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_common.h"
#include "server.pb.h"

// FreeRTOS configs
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"



// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// Callback variable every 1 second for motor data
uint8_t motorDataCallBackCounter = 0;

// My timer handle
TimerHandle_t myTimer;

// Global ticks and time counters
uint32_t ticksM1 = 0;
uint32_t ticksM2 = 0;
uint8_t time = 0;

// Counts number of ticks per specified timer callback (100ms)
uint8_t countM1 = 0;
uint8_t countM2 = 0;

// Error ticks per 100ms
uint8_t errorM1 = 0;
uint8_t errorM2 = 0;

// motor command flags
bool forwardFlag = false;
bool turnAroundFlag = false;
bool rightTurnFlag = false;
bool leftTurnFlag = false;
bool adjustLeftFlag = false;
bool adjustRightFlag = false;

enum motorStates {
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4,
    ADJUST_LEFT = 6,
    ADJUST_RIGHT = 7
} motorState, orientation;

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

MOTORS_DATA motorsData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// Timer used for PWM

void configuringTimer2AsACounter() {

    // 16-bit Synchronous Clock Counter 
    // 125 ms period
    T2CONbits.TCKPS = 2; // Timer2 prescaler N=4 (1:4)
    PR2 = 1999; // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
    TMR2 = 0; // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 500; // duty cycle = OC1RS/(PR2+1) = 25%
    OC1R = 500; // initialize before turning OC1 on; then it is read-only

    OC2CONbits.OCM = 0b110; // PWM mode without fault pin; other OC2CON bits are defaults
    OC2RS = 500; // duty cycle = OC2RS/(PR2+1) = 25%
    OC2R = 500; // initialize before turning OC2 on; then it is read-only

    T2CONbits.ON = 1; // turn on Timer2

}

// Counter used for storing encoder data from Motor1

void configuringTimer3AsACounter() {
    T3CON = 0x0; // Stop Timer and clear control register

    TMR3 = 0x0; // Clear timer register

    // Setting the external clock from TxCLK pin
    T3CONSET = 0x8002; // Start Timer and set the external clock from the

}

// Counter used for storing encoder data from Motor2

void configuringTimer4AsACounter() {
    T4CON = 0x0; // Stop Timer and clear control register

    TMR4 = 0x0; // Clear timer register

    T4CONSET = 0x8002; // Start Timer and set the external clock from the
    //T3CLK pin
}

// Timer used as a reference time in order to calculate units of rotations/sec.

void configuringTimer5AsACounter() {
    T5CON = 0x0; // Stop Timer and clear control register
    TMR5 = 0x0; // Clear timer register
    //PR5 = 1999; // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
    T5CONSET = 0x8000; // Start Timer 
}

void motorDataCallBack() {


    uint8_t buffer3[80] = {0};
    pb_ostream_t encoder3 = pb_ostream_from_buffer(buffer3, sizeof (buffer3));

    motor_motor_data myData;

    myData.left_ticks = countM1;
    myData.right_ticks = countM2;
    myData.left_err = errorM1;
    myData.right_err = errorM2;
    myData.cur_dir = 0;


    // This is the message from the Motors that the server will recieve every 100 milliseconds
    server_server motorDataToServer = server_server_init_zero;


    // Creating a message the will encapsulates my encoded message
    server_msg encodedMotorData = {0, 0,
        {0}};

    motorDataToServer.sequence_number = 999;
    sprintf(motorDataToServer.uid, "rover3");
    motorDataToServer.which_payload = server_server_mCmd_tag;
    motorDataToServer.payload.mCmd.which_msg = motor_motor_msg_data_tag;

    //  The Motors Data includes the following:
    //      - Current Direction orientation: U, D, L, or R
    //      - The Left Motors Error (ticks / 100 milliseconds)
    //      - The Right Motors Error (ticks / 100 milliseconds)
    //      - The Left Motors ticks per 100 milliseconds
    //      - The Right Motors ticks per 100 milliseconds

    motorDataToServer.payload.mCmd.msg.data.cur_dir = 2;
    motorDataToServer.payload.mCmd.msg.data.left_err = 2;
    motorDataToServer.payload.mCmd.msg.data.left_ticks = 2;
    motorDataToServer.payload.mCmd.msg.data.right_err = 2;
    motorDataToServer.payload.mCmd.msg.data.right_ticks = 2;

    // Encoding my Motors Message using Google Protobufs encoding.
    pb_encode_delimited(&encoder3, server_server_fields, &motorDataToServer);

    // Setting the length to include the header (3 bytes, ~~~) and footer (3 bytes, }}}) of message
    encodedMotorData.length = encoder3.bytes_written + 6;

    // Setting the index to the motor datas length.
    encodedMotorData.index = encodedMotorData.length;

    // Copying my the full length of my encocded Motor Data
    memset(encodedMotorData.msg, 0, sizeof (encodedMotorData.msg));

    // Setting my encoded Motor Data message to be the full server Motor Data message.
    sprintf(encodedMotorData.msg, "%s%s%s", "~~~", buffer3, "}}}");

    // Sending my Data to the WiFly Tx every 100 milliseconds
    writeToWiflyTx(encodedMotorData);

}

void vTimerCallback(myTimer) {

    /********************************************/
    /* Getting the count from the External CLK  */
    /********************************************/

    dbgOutputEvent(DBG_IN_SOFTWARE_TIMER);

    ticksM1 += TMR3;
    ticksM2 += TMR4;


    countM1 = TMR3;
    countM2 = TMR4;

    /********************************************/
    /* Converting to ticks / sec -> rot / sec   */
    /********************************************/
    countM1 = countM1 / (144);
    countM2 = countM2 / (144);

    TMR3 = 0;
    TMR4 = 0;

    // Get Direction of M1
    // Get Direction of M2

    // Send Direction of M1
    // Send Direction of M2

    time++; // Incrementing every second for debugging purposes

    //    PLIB_USART_TransmitterByteSend(USART_ID_1, 0x31);
    //    PLIB_USART_TransmitterByteSend(USART_ID_1, 0x20);
    //    PLIB_USART_TransmitterByteSend(USART_ID_1, countM1 + 0x30);
    //    PLIB_USART_TransmitterByteSend(USART_ID_1, 0x09);
    //
    //    PLIB_USART_TransmitterByteSend(USART_ID_1, 0x32); // 2
    //    PLIB_USART_TransmitterByteSend(USART_ID_1, 0x20); // [space]
    //    PLIB_USART_TransmitterByteSend(USART_ID_1, countM2 + 0x30); // M2 rot / sec
    //    PLIB_USART_TransmitterByteSend(USART_ID_1, 0x0A); // Carriage Return
    //   PLIB_USART_TransmitterByteSend(USART_ID_1, 0x0D); // Line Feed

    motorDataCallBackCounter++;

    // Each message the server recieves needs the following:
    //  - Sequence Number
    //  - UID
    //  - Which message payload and which message type.

}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void setMotor1(int speed, int direction) {
    OC1CONbits.ON = 1; // turn on OC1
    OC1RS = speed * 20;

    T2CONSET = 0x8000; // Enable Timer2, prescaler 1:1
    OC1CONSET = 0x8000; // Enable Output Compaterre Module 1

    PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0, 1);
    PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_14, direction);

}

void setMotor2(int speed, int direction) {
    OC2CONbits.ON = 1; // turn on OC2
    OC2RS = speed * 20;

    T2CONSET = 0x8000; // Enable Timer2, prescaler 1:1
    OC2CONSET = 0x8000; // Enable Output Compare Module 1

    PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1, 1);
    PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1, direction);

}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************/

bool forward() {
    forwardFlag = false;
    if (ticksM1 < 2000) {
        setMotor1(35, 1);
        setMotor2(35, 1);
    } else {
        setMotor1(0, 0);
        setMotor2(0, 0);
        forwardFlag = true;
    }
    return forwardFlag;
}

bool turnAround() {
    turnAroundFlag = false;
    if (ticksM1 < 1060) {
        setMotor1(70, 1);
        setMotor2(70, 0);
    } else {
        setMotor1(0, 0);
        setMotor2(0, 0);
        turnAroundFlag = true;
    }
    return turnAroundFlag;
}

bool leftTurn() {
    leftTurnFlag = false;
    if (ticksM1 < 615) {
        setMotor1(70, 0);
        setMotor2(70, 1);
    } else {
        setMotor1(0, 0);
        setMotor2(0, 0);
        leftTurnFlag = true;
    }
    return leftTurnFlag;
}

bool rightTurn() {
    rightTurnFlag = false;
    if (ticksM1 < 615) {
        setMotor1(70, 1);
        setMotor2(70, 0);
    } else {
        setMotor1(0, 0);
        setMotor2(0, 0);
        rightTurnFlag = true;
    }
    return rightTurnFlag;
}

bool adjustLeft() {
    adjustLeftFlag = false;
    if (ticksM1 < 60) {
        setMotor1(70, 0);
        setMotor2(70, 1);
    } else {
        setMotor1(0, 0);
        setMotor2(0, 0);
        adjustLeftFlag = true;
    }
    return adjustLeftFlag;
}

bool adjustRight() {
    adjustRightFlag = false;
    if (ticksM1 < 60) {
        setMotor1(70, 1);
        setMotor2(70, 0);
    } else {
        setMotor1(0, 0);
        setMotor2(0, 0);
        adjustRightFlag = true;
    }
    return adjustRightFlag;
}


/*******************************************************************************
  Function:
    void MOTORS_Initialize ( void )
  Remarks:
    See prototype in motors.h.
 */

/******************************************************************/


void MOTORS_Initialize(void) {

    dbgOutputEvent(DBG_ENTER_MOTOR_TASK);

    DRV_TMR0_Initialize();
    DRV_TMR1_Initialize();
    DRV_TMR2_Initialize();

    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_2);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_3);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_4);

    uart = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);

    configuringTimer2AsACounter();
    configuringTimer3AsACounter();
    configuringTimer4AsACounter();
    configuringTimer5AsACounter();


    myTimer = xTimerCreate("Timer", // Just a text name, not used by the kernel.
            pdMS_TO_TICKS(100), // The timer period in ticks.
            pdTRUE, // The timers will auto-reload themselves when they expire.
            (void *) 0, // Assign each timer a unique id equal to its array index.
            vTimerCallback // Each timer calls the same callback when it expires.
            );
    dbgOutputEvent(DBG_SETTING_SOFTWARE_TIMER);
    xTimerStart(myTimer, 0);
}

/******************************************************************************
  Function:
    void MOTORS_Tasks ( void )
  Remarks:
    See prototype in motors.h.
 */

void MOTORS_Tasks(void) {

    motor_msg m;

    m = readFromMotorQueue();

    // Initializing a start orientation of UP.
    orientation = UP;

    while (1) {

        if (motorDataCallBackCounter < 10) {
            motorDataCallBack();
        } else {
            motorDataCallBackCounter = 0;
        }

        switch (m.dir) {

            case UP:
                switch (orientation) {
                    case UP:
                        if (!forward()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case DOWN:
                        if (!turnAround()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }

                        break;
                    case RIGHT:
                        if (!leftTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }

                        break;
                    case LEFT:
                        if (!rightTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                }
                break;

            case DOWN:
                switch (orientation) {
                    case UP:
                        if (!turnAround()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case DOWN:
                        if (!forward()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case RIGHT:
                        if (!rightTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case LEFT:
                        if (!leftTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                }
                break;
            case RIGHT:
                switch (orientation) {
                    case UP:
                        if (!rightTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case DOWN:
                        if (!leftTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case RIGHT:
                        if (!forward()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case LEFT:
                        if (!turnAround()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                }
                break;
            case LEFT:
                switch (orientation) {
                    case UP:
                        if (!leftTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case DOWN:
                        if (!rightTurn()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case RIGHT:
                        if (!turnAround()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                    case LEFT:
                        if (!forward()) {
                        } else {
                            orientation = m.dir;
                            ticksM1 = 0;
                            sendDoneMessage(m.seq_num);
                            m = readFromMotorQueue();
                            break;
                        }
                        break;
                }
                break;

            case ADJUST_LEFT:
                if (!adjustLeft()) {
                } else {
                    ticksM1 = 0;
                    sendDoneMessage(m.seq_num);
                    m = readFromMotorQueue();
                    break;
                }
                break;

            case ADJUST_RIGHT:
                if (!adjustRight()) {
                } else {
                    ticksM1 = 0;
                    sendDoneMessage(m.seq_num);
                    m = readFromMotorQueue();
                    break;
                }
                break;
        }
    }
}


/*******************************************************************************
 End of File
 */
