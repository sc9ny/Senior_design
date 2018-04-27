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

#ifndef _PARSER_H    /* Guard against multiple inclusion */
#define _PARSER_H

#include <stdio.h>
#include "app.h"
#include "debug.h"
#include <string.h>
#include "uart_queue.h"
#include "pb_encode.h" // protobuf files
#include "pb_decode.h"
#include "pb.h"
#include "pb_common.h"
#include "sensor.pb.h"
#include "server.pb.h"
#include "status.pb.h"
#include "motor.pb.h"
#include "emulator.pb.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    bool parseProto(server_msg *rx_msg);
    bool parserTest(server_msg *received_msg);  // TODO Get rid of
    bool serverLoopback(server_msg *received_msg);
    
    bool sendDoneMessage(int32_t sequence_number);
    bool requestResend(int32_t sequence_number);
    
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */