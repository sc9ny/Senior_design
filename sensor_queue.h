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

#ifndef SENSOR_QUEUE_H    /* Guard against multiple inclusion */
#define SENSOR_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"
//#include "debug.h"
//#include "sensor_state.h"
extern QueueHandle_t msgQ;
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct  {
    unsigned int sensor_value;
    char units[2];
    uint16_t sync;
    uint16_t checksum;
    uint16_t cc_code;
    uint16_t x_obj;
    uint16_t y_obj;
    uint16_t width;
    uint16_t height;
    uint16_t angle;
} sensor_msg;


    
void writeToQueue(sensor_msg s_val);

sensor_msg readFromQueue(QueueHandle_t msgQ);

QueueHandle_t createQueue(int queue_size, unsigned int queue_length);

    
/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
