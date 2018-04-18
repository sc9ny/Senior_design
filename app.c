#include "app.h"
#include "FreeRTOS.h"
#include "queue.h"
//#include "color_sensor.h"
#include "system_config/default/framework/driver/usart/drv_usart_static.h"
#include "peripheral/usart/plib_usart.h"
#include "pixy.h"
APP_DATA appData;
#define QUEUE_LENGTH 1024
#define QUEUE_SIZE sizeof(sensor_msg)

// global handle
//QueueHandle_t msgQ;
TaskHandle_t task_handle;
DRV_HANDLE uart;
sensor_msg data;
//static void COLORSENSOR_TASK(void *pvParameters)
//{
//    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
//    SENSOR_TASK_INIT();
//    SYS_PRINT("HELLO1\n");
//
//    data.sensor_value = 0;
//    data.units[0] = 'n';
//    data.units[1] = 'a';
//    data.red =0;
//    data.green =0;
//    data.blue =0;
//    ReadColor col;
//    while(1) {
//        data = readFromQueue(msgQ);
//        SYS_PRINT("RED FROM COLOR SENSOR: %d\n", data.red);
//        SYS_PRINT("GREEN FROM COLOR SENSOR: %d\n", data.green);
//        SYS_PRINT("BLUE FROM COLOR SENSOR: %d\n", data.blue);
//        
//        
//        SYS_PRINT("Reflected color is: %s\n", whatColor());
//    }
//}
static void PIXY_TASK(void *pvParameters)
{
        configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
        setUpPixy();
        setUpADC();
        uint16_t prev_cc =0;
        uint16_t sec_prev_cc =0;
        while(1)
        {

            data = readFromQueue(msgQ);
            SYS_PRINT("IR Sensor val: %d\n",data.sensor_value);
            //SYS_PRINT(" CC_CODE %d\n", data.cc_code);
            if (data.cc_code == 10 && prev_cc == data.cc_code && sec_prev_cc == data.cc_code)
            {
                //SYS_PRINT("No change in direction\n");   
            }
            else if (data.cc_code == 11&& prev_cc == data.cc_code && sec_prev_cc == data.cc_code)
            {
                //SYS_PRINT("Right\n");
            }
            else if (data.cc_code == 19&& prev_cc == data.cc_code && sec_prev_cc == data.cc_code)
            {
                //SYS_PRINT("Left\n");                
            }
            sec_prev_cc = prev_cc;
            prev_cc =data.cc_code;
//            
        
            
        }
        
}
static void test(void *pvParameters)
{
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    //while(1)
    //SYS_PRINT("TEST\n");
}
void APP_Initialize ( void )
{
    //msgQ = xQueueCreate(QUEUE_LENGTH, QUEUE_SIZE);

    //xTaskCreate(COLORSENSOR_TASK, "sensor task", 100, NULL, 1, &task_handle);
    //initi2c();
    xTaskCreate(PIXY_TASK, "pixy task", 100, NULL, 1, &task_handle);
    //xTaskCreate(test, "test", 100, NULL, 1, &task_handle);
    
    
    
//    initi2c();
//    resetSensor();
//    sensorInit();
    
//    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
//    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_3);
//    DRV_TMR0_Start();
//    
//    dbgOutputEvent(DBG_ENTER_TASK);
//    dbgSetUpPins();
//    dbgOutputEvent(0);
//    DRV_ADC_Close();
//    DRV_ADC_Initialize();
//    DRV_ADC_Open();
//    uart = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
    
}

void APP_Tasks ( void )
{
    //readSignature(0x54);
    
    
//    // create messages
//    sensor_msg data;
//    data.sensor_value = 0;
//    data.units[0] = 'n';
//    data.units[1] = 'a';
//    data.red =0;
//    data.green =0;
//    data.blue =0;
//    uart_msg info;
//    info.state = 0;
//    info.sum = 0;
//    
//    //if(msgQ == NULL)  return;
//    dbgOutputEvent(DBG_ENTER_WHILE);
//    while(1) {
//        data = readFromQueue(msgQ);             // Read sensor val from msgQ
//        info = state_machine(info, data.sensor_value);   // State Machine
////      info.sum = 0x61;                        // Value designed for testing
//        //      info.sum = 0x61;                        // Value designed for testing
//        if(info.state == 0 && info.sum != 0) {
//            dbgOutputEvent(DBG_BEFORE_UART);
//            SYS_PRINT("IR SENSOR: %d cm\n", info.sum);
//            SYS_PRINT("RED FROM COLOR SENSOR: %d\n", data.red);
//            SYS_PRINT("GREEN FROM COLOR SENSOR: %d\n", data.green);
//            SYS_PRINT("BLUE FROM COLOR SENSOR: %d\n", data.blue);
//            dbgOutputEvent(DBG_SEND_UART);
//            info.sum = 0;     
//            // Clear all values
//            //dbgUARTVal(info.sum);  // Send average over UART
//        }
//        
//            
//            
//            
//            
//        
//        dbgOutputEvent(DBG_LEFT_UART);
//        
//   }
}
/*******************************************************************************
 End of File
 */
