/* 
 * File:   i2c_protocol.h
 * Author: seungleechoi
 *
 * Created on March 21, 2018, 6:28 PM
 */

#ifndef I2C_PROTOCOL_H
#define	I2C_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif
#define COMMAND_BIT 0x80

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "peripheral/i2c/plib_i2c.h"
#include "peripheral/ports/plib_ports.h"
void initi2c();
void i2cwait();
unsigned char i2c_send(unsigned char data);
unsigned char i2c_recv(void);
void i2c_start();
void i2c_restart();
void i2c_stop();
void i2c_ack();
void i2c_nak();
void i2c_test();



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_PROTOCOL_H */

