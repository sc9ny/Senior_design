#include "pixy.h"
QueueHandle_t msgQ;
#define QUEUE_LENGTH 1024
#define QUEUE_SIZE sizeof(sensor_msg)
sensor_msg data;
TimerHandle_t xTimers;
int IRState = 0, average = 0;
void vTimerCallback(xTimers)
{
    
    readSignature(0x54);
    convertIR();
    writeToQueue(data);
}
void setUpPixy(void)
{
    initi2c();
    msgQ = xQueueCreate(QUEUE_LENGTH, QUEUE_SIZE);
    // timer expires every 20ms since pixy gathers 50 frames per second. = 1 frame per 20 ms
    xTimers = xTimerCreate(  "Timer", pdMS_TO_TICKS(50) , pdTRUE, ( void * ) 0,vTimerCallback );
    xTimerStart(xTimers,0);
    //if (xTimers == NULL)
        //SYS_PRINT("TIMER NULL\n");
    //else
        //SYS_PRINT("TIMER CREATED\n");
}
uint16_t readSignature(unsigned int addr)
{
    //SYS_PRINT("NEW CALL \n");
    data.sync = 0;
    data.checksum =0;
    data.cc_code =0;
    data.x_obj =0;
    data.y_obj =0;
    data.width =0;
    data.height =0;
    data.angle =0;
    int z =0;
    do{ // initiate the communication with the sensor in write mode
        i2c_start();
    } while(!i2c_send(addr << 1));
    //i2c_send(addr);
    //i2c_restart();
    do{ // initiate the communication in read mode
        i2c_restart();
    } while(!i2c_send((addr<< 1) | 1));
    z = i2c_recv_pixy();

    i2c_stop();
    //SYS_PRINT("count: %d\n", z);
    return data.cc_code;
}
void setUpADC(void)
{
    DRV_ADC_Close();
    DRV_ADC_Initialize();
    DRV_ADC_Open();
    
}
unsigned int readIR()
{
    DRV_ADC_Start();
    uint16_t result = 0;
    result = DRV_ADC_SamplesRead(0);
    return result;
    DRV_ADC_Stop();
}
unsigned int convertIR()
{
    unsigned int IR = readIR();
    float volts = IR *0.00322265625;
    //float volts = IR*0.0048828125;
    //unsigned int distance = 13*pow(volts, -1);
    // only valid for voltages b/w 0.4 and 1.75
    //if (volts >0.4 && volts < 1.75)
    unsigned int distance = 2076/(volts*200 -11);
    switch (IRState)
    {
        case 0:
            average = distance;
            IRState++;
            
            break;
        case 1:
            average += distance;
            IRState++;
            
            break;
        case 2:
             average += distance;
            IRState++;
            
            break;
        case 3:
            average += distance;
            average = average >>2;
            data.sensor_value = average;
            IRState = 0;
            
            break;
    }
        //data.sensor_value = 2076/(volts*200 -11);
//    else
//        data.sensor_value = 0;
    
    return data.sensor_value;
}