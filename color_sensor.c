//#include "color_sensor.h"
//
//
//void sensorInit()
//{
//    do{
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x00);
//    i2c_send(0x03); // set the enable and power on bits of the sensor
//    i2c_stop();
//
//    do{
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x01);
//    i2c_send(0xF6); // set integration time of sensor to 24ms
//    i2c_stop();
//
//    do{
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x0F);
//    i2c_send(0x01); // set the gain
//    i2c_stop();
//}
//void resetSensor()
//{
//    int i =0x04;
//    do{
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x00);
//    i2c_send(0x00); // set the enable and power on bits of the sensor
//    i2c_stop();
//    do{
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x01);
//    i2c_send(0xFF); // set the enable and power on bits of the sensor
//    i2c_stop();
//    do{
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x03);
//    i2c_send(0xFF); // set the enable and power on bits of the sensor
//    i2c_stop();
//    while (i <= 0x0F )
//    {
//        if (i!= 0x08 &&i != 0x09 && i != 0x0A && i != 0x0B && i != 0x0E){
//            do
//            {
//                i2c_start();
//            } while(!i2c_send(SENSOR_ADDR << 1));
//            i2c_send(i);
//            i2c_send(0x00); // set the enable and power on bits of the sensor
//            i2c_stop();
//            
//        }
//        i++;
//    }
//    
//}
//
//int getRed(){
//    int red;
//
//    do{ // initiate the communication with the sensor in write mode
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x16);
//    i2c_stop();
//    do{ // initiate the communication in read mode
//        i2c_start();
//    } while(!i2c_send((SENSOR_ADDR << 1) | 1));
//    red = i2c_recv();
//    i2c_nak();
//    i2c_stop();
//
//    do{ // initiate the communication with the sensor in write mode
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send( COMMAND_BIT|0x17);
//    i2c_stop();
//    do{ // initiate the communication in read mode
//        i2c_start();
//    } while(!i2c_send((SENSOR_ADDR << 1) | 1));
//    red |= (i2c_recv()<<8);
//    i2c_nak();
//    i2c_stop();
//    return red;
//}
//int getGreen(){
//    int green;
//
//    do{ // initiate the communication with the sensor in write mode
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send( COMMAND_BIT|0x18);
//    i2c_stop();
//    do{ // initiate the communication in read mode
//        i2c_start();
//    } while(!i2c_send((SENSOR_ADDR << 1) | 1));
//    green = i2c_recv();
//    i2c_nak();
//    i2c_stop();
//
//    do{ // initiate the communication with the sensor in write mode
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x19);
//    i2c_stop();
//    do{ // initiate the communication in read mode
//        i2c_start();
//    } while(!i2c_send((SENSOR_ADDR << 1) | 1));
//    green |= (i2c_recv()<<8);
//    i2c_nak();
//    i2c_stop();
//    return green;
//}
//
//int getBlue(){
//    int blue;
//
//    do{ // initiate the communication with the sensor in write mode
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT|0x1A);
//    i2c_stop();
//    do{ // initiate the communication in read mode
//        i2c_start();
//    } while(!i2c_send((SENSOR_ADDR << 1) | 1));
//    blue = i2c_recv();
//    i2c_nak();
//    i2c_stop();
//
//    do{ // initiate the communication with the sensor in write mode
//        i2c_start();
//    } while(!i2c_send(SENSOR_ADDR << 1));
//    i2c_send(COMMAND_BIT| 0x1B);
//    i2c_stop();
//    do{ // initiate the communication in read mode
//        i2c_start();
//    } while(!i2c_send((SENSOR_ADDR << 1) | 1));
//    blue |= (i2c_recv()<<8);
//    i2c_nak();
//    i2c_stop();
//    return blue;
//}
//void i2c_test(){ // just a temporary test function
//    int temp;
//
//    int i =0;
//    while(1){
//      do{
//          i2c_start();
//      } while(!i2c_send(SENSOR_ADDR << 1));
//      while(!DRV_I2C0_WaitForACKOrNACKComplete());
//      i2c_send(COMMAND_BIT |0x12); //0x16 is the sensor's register for the low byte of red
//      while(!DRV_I2C0_WaitForACKOrNACKComplete());
//      //i2c_stop();
//      while (!DRV_I2C0_MasterRestart());
//      while(!i2c_send((SENSOR_ADDR << 1) | 1));
//      while(!DRV_I2C0_WaitForACKOrNACKComplete());
////      do{
////          //i2c_restart();
////          DRV_I2C0_MasterRestart();
////      } while(!i2c_send((SENSOR_ADDR << 1) | 1)); //sends sensor address and LSB as "read bit" = 1
//    
//      temp = i2c_recv();
//      i2c_nak();
//      i2c_stop();
//      SYS_PRINT("result is: %d\n" , temp);
//      i++;
//    }
//}
