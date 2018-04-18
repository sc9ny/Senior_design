#include "i2c_protocol.h"
sensor_msg data;
void initi2c()
{
    I2C1CON = 0x0; // reset the i2c configuration register just in case
    I2C1BRG = 0x0c2; // set the i2c baud rate to ~200kHz by dividing 40MHz w/ 0xc2
    I2C1STAT = 0x0; 
    I2C1CONSET = 0x00002000; // set the module to stop in idle mode
    I2C1CONSET = 0x00008000; // set enable bit to 1 to start the module
}

void i2c_wait()
{
    while( (I2C1CON & 0x1f) || I2C1STAT & 0x00004000);
}
unsigned char i2c_send(unsigned char data)
{
    i2c_wait();
    I2C1TRN = data;
    i2c_wait();
    return !(I2C1STAT & 0x00008000);
}
unsigned char i2c_recv(void){
   i2c_wait();
   I2C1CONSET = 0x00000008; // turn on reception
   i2c_wait(); // wait for data to be received
   I2C1STATCLR = 0x00000040; // Reset rcv overflow bit
   return I2C1RCV; //receive data register (32 bit register, uses only indices 0-7)
}
// helper fuction for i2c_recv_pixy
uint16_t getWord()
{
    uint16_t word;
    uint8_t byte;
    i2c_ack();
       byte = i2c_recv();
       i2c_ack();
       word = i2c_recv();
       
       word <<=8;
       word |= byte;
       return word;
}
// Function to receive block data from pixy
int i2c_recv_pixy()
{
    uint16_t COLORCODE = 0xaa56;
    uint16_t word, lastw;
    lastw = 0xffff;
    uint8_t byte;
    int count = 0;
    i2c_wait();
    
   I2C1CONSET = 0x00000008; // turn on reception
   i2c_wait(); // wait for data to be received
   // wait until there is no byte receivable.
   while (1)
   {
       
       word = getWord();
       //SYS_PRINT("word %d: ,%d\n" ,count,word);
       
       count++;
       if (word == 0 && lastw==0)
       {
         //  SYS_PRINT ("CANT FIND\n");
           return 0;
       }
       else if (word == COLORCODE )
       {
           //SYS_PRINT ("FOUND!\n");
           data.sync = word;
           i2c_ack();
           byte = i2c_recv();
            i2c_ack();
            word = i2c_recv();
            word <<=8;
            word |= byte;
            data.checksum = word;
            i2c_ack();
            byte = i2c_recv();
            i2c_ack();
            word = i2c_recv();
            word <<=8;
            word |= byte;
            data.cc_code =word;
            i2c_ack();
            byte = i2c_recv();
            i2c_ack();
            word = i2c_recv();
            word <<=8;
            word |= byte;
            data.x_obj =word;
            i2c_ack();
            byte = i2c_recv();
            i2c_ack();
            word = i2c_recv();
            word <<=8;
            word |= byte;
            
            data.y_obj =word;
            i2c_ack();
            byte = i2c_recv();
            i2c_ack();
            word = i2c_recv();
            word <<=8;
            word |= byte;
            data.width =word;
            i2c_ack();
            byte = i2c_recv();
            i2c_ack();
            word = i2c_recv();
            word <<=8;
            word |= byte;
            data.height =word;
            i2c_ack();
            byte = i2c_recv();
            i2c_nak();
            word = i2c_recv();
            word <<=8;
            word |= word;
            data.angle =word;
            //i2c_nak();
           break;
       }
       else if (word == 0x56aa)
       {
           i2c_recv();          
       }
       lastw =word;
       
   }
   I2C1STATCLR = 0x00000040; // Reset rcv overflow bit

   return count;
}
void i2c_start(){
    i2c_wait();
    I2C1CONSET = 0x00000001; // set SEN to 1 to begin start condition with SCL and SDA
                                   // will be automatically reset by hardware (to 0)
    i2c_wait();
}

void i2c_stop(){
    i2c_wait();
        I2C1CONSET = 0x00000004; // set PEN to 1 to begin stop condition with SCL and SDA
                                   // will be automatically reset by hardware (to 0)
    i2c_wait();
}

void i2c_restart(){
    i2c_wait();
    I2C1CONSET = 0x00000002; // set the RSEN bit to 1 to restart connection with SCL and SDA
                                    // will be automatically reset by hardware (to 0)
    i2c_wait();
}

void i2c_ack(){ //note: so far not used at all
    i2c_wait();
    I2C1CONCLR = 0x00000020; // set ACKDT to 0 for ACK
    I2C1CONSET = 0x00000010; // set ACKEN to start ack sequence (automatically performed and reset to 0 by hardware)
                                     // transmits the value of the ACKDT bit (cleared above)
}

void i2c_nak(){
    i2c_wait();
    I2C1CONSET = 0x00000020; // set ACKDT to 1 for NAK
    I2C1CONSET = 0x00000010; // set ACKEN to start ack sequence (automatically performed and reset to 0 by hardware)
                                     // transmits the value of the ACKDT bit (set above)
}


