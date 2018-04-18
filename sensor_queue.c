#include "sensor_queue.h"

void writeToQueue(sensor_msg sensor_val) {
    
  // Before writing to queue.
//    dbgOutputVal(sensor_val.red);
    
    //dbgOutputEvent(DBG_BEFORE_SEND_TO_QUEUE);
    BaseType_t *pxHigherPriorityTaskWoken;
    xQueueSendToBackFromISR(msgQ, &sensor_val, pxHigherPriorityTaskWoken);
    
    //dbgOutputEvent(DBG_AFTER_SEND_TO_QUEUE);
}

sensor_msg readFromQueue(QueueHandle_t msgQ) {
    //dbgOutputEvent(DBG_BEFORE_QUEUE);
    
    sensor_msg data;
    if(xQueueReceive(msgQ, &data, portMAX_DELAY) != pdPASS) {
       // dbgOutputEvent(DBG_QUEUE_READ_FAIL);
    } 
    //dbgOutputEvent(DBG_READ_QUEUE);
    //dbgOutputVal(data.red);
    return data;
}

QueueHandle_t createQueue(int queue_length, unsigned int queue_size) {
    return xQueueCreate(queue_length, sizeof(unsigned int));
}


/* *****************************************************************************
 End of File
 */
