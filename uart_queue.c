/////////////////////////////////////////////
//
//
//
//
//

#include "uart_queue.h"

// TODO change so there is a specific read/write for each queue

QueueHandle_t createQueue(int queue_length, unsigned int msg_size) {
    return xQueueCreate(queue_length, msg_size);
}

BaseType_t isQueueEmptyFromISR(QueueHandle_t handle) {
    return xQueueIsQueueEmptyFromISR(handle);
}

void writeToWiflyTx(server_msg msg) {
  // Before writing to queue.
    dbgOutputVal(msg.length);
    
    dbgOutputEvent(DBG_BEFORE_SEND_TO_QUEUE);
    if(xQueueSendToBack(wifly_Tx, &msg, 0) != pdPASS) {
        dbgOutputEvent(DBG_QUEUE_READ_FAIL);
    }
    SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
    dbgOutputEvent(DBG_AFTER_SEND_TO_QUEUE);
}

void writeToMotorQueue(motor_msg m) {
  // Before writing to queue.
    dbgOutputVal(m.dir);
    
    dbgOutputEvent(DBG_BEFORE_SEND_TO_QUEUE);
    if(xQueueSendToBack(motorQ, &m, 0) != pdPASS) {
        dbgOutputEvent(DBG_QUEUE_READ_FAIL);
    }
    SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
    dbgOutputEvent(DBG_AFTER_SEND_TO_QUEUE);
}

void writeToQueueFromISR(QueueHandle_t q, server_msg msg, BaseType_t *pxHigherPriorityTaskWoken) {
  // Before writing to queue.
    dbgOutputVal(msg.index);
    
    dbgOutputEvent(DBG_BEFORE_SEND_TO_QUEUE);
    
    if(xQueueSendToBackFromISR(q, &msg, pxHigherPriorityTaskWoken) != pdPASS) {
        dbgOutputEvent(DBG_QUEUE_READ_FAIL);
    }
    SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
    
    dbgOutputEvent(DBG_AFTER_SEND_TO_QUEUE);
}

server_msg readFromWiflyRx() {
    dbgOutputEvent(DBG_BEFORE_QUEUE);
    
    server_msg rx_msg = {0,0,{0}};
    if(xQueueReceive(wifly_Rx, &rx_msg, portMAX_DELAY) != pdPASS) {
        dbgOutputEvent(DBG_QUEUE_READ_FAIL);
    } 
    dbgOutputEvent(DBG_READ_QUEUE);
    return rx_msg;
}

motor_msg readFromMotorQueue() {
    dbgOutputEvent(DBG_BEFORE_QUEUE);
    
    motor_msg cmd = {0,0};
    if(xQueueReceive(motorQ, &cmd, portMAX_DELAY) != pdPASS) {
        dbgOutputEvent(DBG_QUEUE_READ_FAIL);
    } 
    dbgOutputEvent(DBG_READ_QUEUE);
    return cmd;
}


server_msg readFromQueueFromISR(QueueHandle_t msgQ, BaseType_t *pxHigherPriorityTaskWoken) {
    dbgOutputEvent(DBG_BEFORE_QUEUE);
    
    server_msg data = {0};
    if(xQueueReceiveFromISR(msgQ, &data, pxHigherPriorityTaskWoken) != pdPASS) {
        dbgOutputEvent(DBG_QUEUE_READ_FAIL);
    } 
    dbgOutputEvent(DBG_READ_QUEUE);
    return data;
}




/* *****************************************************************************
 End of File
 */