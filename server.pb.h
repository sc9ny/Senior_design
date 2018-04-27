/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Sun Apr 22 21:54:45 2018. */

#ifndef PB_SERVER_SERVER_PB_H_INCLUDED
#define PB_SERVER_SERVER_PB_H_INCLUDED
#include <pb.h>

#include "sensor.pb.h"

#include "motor.pb.h"

#include "emulator.pb.h"

#include "status.pb.h"

#include "controller.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _server_server {
    int32_t sequence_number;
    char uid[15];
    pb_size_t which_payload;
    union {
        sensor_color_sensor color_data;
        sensor_ir_sensor ir_data;
        motor_motor_ctl mCmd;
        motor_motor_data data;
        emulator_battle_state battle;
        emulator_attacks atk;
        status_request_resend again;
        status_request_complete done;
        status_device_status health;
        controller_controller_msg ctl;
    } payload;
/* @@protoc_insertion_point(struct:server_server) */
} server_server;

/* Default values for struct fields */

/* Initializer values for message structs */
#define server_server_init_default               {0, "", 0, {sensor_color_sensor_init_default}}
#define server_server_init_zero                  {0, "", 0, {sensor_color_sensor_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define server_server_color_data_tag             3
#define server_server_ir_data_tag                4
#define server_server_mCmd_tag                   5
#define server_server_data_tag                   6
#define server_server_battle_tag                 7
#define server_server_atk_tag                    8
#define server_server_again_tag                  9
#define server_server_done_tag                   10
#define server_server_health_tag                 11
#define server_server_ctl_tag                    12
#define server_server_sequence_number_tag        1
#define server_server_uid_tag                    2

/* Struct field encoding specification for nanopb */
extern const pb_field_t server_server_fields[13];

/* Maximum encoded size of messages (where known) */
#define server_server_size                       60

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define SERVER_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
