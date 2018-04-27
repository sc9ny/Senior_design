/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c
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

#include "app.h"
#include "debug.h"
#include "parser.h"
#include "uart_queue.h"
#include "peripheral/usart/plib_usart.h"
#include <stdio.h>
#include <string.h>

//#include "motors.h"
#include "timers.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"




/******************************************************************/

/******************************************************************/


/******************************************************************/


APP_DATA appData;
#define QUEUE_LENGTH 50
#define MSG_SIZE sizeof(server_msg)
QueueHandle_t wifly_Rx, wifly_Tx, xMotorQueue;
server_msg data, read_from_queue;



void APP_Initialize ( void )
{
    
//    char txMessage_0 = '1';
//    char txMessage_1 = '2';
//    char txMessage_2 = '3';
//    char txMessage_3 = '4';
//    char txMessage_4 = '4';
//    char txMessage_5 = '3';
//    char txMessage_6 = '2';
//    char txMessage_7 = '1';
//    
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_0, ( TickType_t ) 0 );
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_1, ( TickType_t ) 0 );
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_2, ( TickType_t ) 0 );
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_3, ( TickType_t ) 0 );
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_4, ( TickType_t ) 0 );
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_5, ( TickType_t ) 0 );
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_6, ( TickType_t ) 0 );
//    xQueueSend( xMotorQueue, ( void * ) &txMessage_7, ( TickType_t ) 0 );

    
    dbgOutputEvent(DBG_ENTER_TASK);
    uart = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
    wifly_Rx = createQueue(QUEUE_LENGTH, MSG_SIZE);
    wifly_Tx = createQueue(QUEUE_LENGTH, MSG_SIZE);
    xMotorQueue = createQueue(QUEUE_LENGTH, sizeof(char));
    dbgOutputEvent(DBG_LEAVING_TASK);
    SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT);
}


void APP_Tasks ( void )
{   
        dbgOutputEvent(DBG_ENTER_TASK);

//    // Buffer to write out message to
//    uint8_t buffer[80] = {0};
//    pb_ostream_t encoder = pb_ostream_from_buffer(buffer, sizeof(buffer));
//    
//    // Create a test motor protobuf message
//    server_server test_proto_msg;
//    test_proto_msg.sequence_number = 0;
//    sprintf(test_proto_msg.uid, "rover2");
//    test_proto_msg.which_payload = server_server_mCmd_tag;
//    test_proto_msg.payload.mCmd.which_msg = motor_motor_msg_ctl_msg_tag;
//    test_proto_msg.payload.mCmd.msg.ctl_msg.direction = 2;
//    
//    pb_encode_delimited(&encoder, server_server_fields, &test_proto_msg);       //  vTaskDelete(NULL); 
//    
//    data.length = encoder.bytes_written+6;
//    data.index = data.length;
//    memset(data.msg, 0, sizeof(data.msg));
//    sprintf(data.msg, "~~~");
//    strncat(data.msg, buffer, data.length-6);
//    strcat(data.msg, "}}}");
////
//    // create messages
//    int i = 0;
//    writeToWiflyTx(data);
//    while(i < 100000) { i = i + 1; }
//    i = 0;
//    
	 /* We expect no more than 128 tokens */ //TODO MAKE THIS LESS  
    dbgOutputEvent(DBG_ENTER_WHILE);
    //while(1) {
        read_from_queue = readFromWiflyRx();
        dbgOutputEvent(DBG_READ_DATA_FROM_QUEUE);
        if(read_from_queue.msg[0] != '\0' && read_from_queue.length != 0) {
            dbgOutputEvent(DBG_BEFORE_PROTOPARSE);
            if(parseProto(&read_from_queue)) {
                dbgOutputEvent(DBG_PARSE_PROTO_COMPLETE);
            } 
        }
        
    //}
}

/*******************************************************************************
 End of File
 */