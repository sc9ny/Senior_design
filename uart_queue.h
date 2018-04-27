/* ************************************************************************** */
/** Descriptive File Name
  @Company
    Company Name
  @File Name
    filename.h
  @Summary
    Brief description of the file.
  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _UART_QUEUE_H    /* Guard against multiple inclusion */
#define _UART_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "debug.h"

#define headerString "~~~"        // could change
#define footerString "}}}"        // could change

extern QueueHandle_t wifly_Rx, wifly_Tx, motorQ;

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
// Holds a message that goes into wifly_Tx and wifly_Rx
typedef struct {
    unsigned int length;    // overall msg length
    unsigned int index;     // counts down for transmit (bytes_remaining) or up for receive 
    unsigned char msg[100];       // protobuf
} server_msg;

typedef struct {
  unsigned int seq_num;
  unsigned int dir;
} motor_msg;

void writeToWiflyTx(server_msg s_val);
void writeToMotorQueue(motor_msg m);
void writeToQueueFromISR(QueueHandle_t q, server_msg s_val, BaseType_t *pxHigherPriorityTaskWoken);

server_msg readFromWiflyRx();
motor_msg readFromMotorQueue();
server_msg readFromQueueFromISR(QueueHandle_t msgQ, BaseType_t *pxHigherPriorityTaskWoken);

QueueHandle_t createQueue(int queue_size, unsigned int queue_length);
BaseType_t isQueueEmptyFromISR(QueueHandle_t handle);

    
/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */