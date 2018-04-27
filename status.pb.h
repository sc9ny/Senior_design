/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Sun Apr 22 21:54:45 2018. */

#ifndef PB_STATUS_STATUS_PB_H_INCLUDED
#define PB_STATUS_STATUS_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _status_device_status {
    bool server_up;
    bool controller_up;
    bool trainer_up;
    bool pikachu_up;
    bool emulator_up;
/* @@protoc_insertion_point(struct:status_device_status) */
} status_device_status;

typedef struct _status_request_complete {
    uint32_t sequence_number;
/* @@protoc_insertion_point(struct:status_request_complete) */
} status_request_complete;

typedef struct _status_request_resend {
    uint32_t sequence_number;
/* @@protoc_insertion_point(struct:status_request_resend) */
} status_request_resend;

typedef struct _status_status_msg {
    pb_size_t which_msg;
    union {
        status_request_complete done;
        status_request_resend again;
        status_device_status stats;
    } msg;
/* @@protoc_insertion_point(struct:status_status_msg) */
} status_status_msg;

/* Default values for struct fields */

/* Initializer values for message structs */
#define status_request_complete_init_default     {0}
#define status_request_resend_init_default       {0}
#define status_device_status_init_default        {0, 0, 0, 0, 0}
#define status_status_msg_init_default           {0, {status_request_complete_init_default}}
#define status_request_complete_init_zero        {0}
#define status_request_resend_init_zero          {0}
#define status_device_status_init_zero           {0, 0, 0, 0, 0}
#define status_status_msg_init_zero              {0, {status_request_complete_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define status_device_status_server_up_tag       1
#define status_device_status_controller_up_tag   2
#define status_device_status_trainer_up_tag      3
#define status_device_status_pikachu_up_tag      4
#define status_device_status_emulator_up_tag     5
#define status_request_complete_sequence_number_tag 1
#define status_request_resend_sequence_number_tag 1
#define status_status_msg_done_tag               1
#define status_status_msg_again_tag              2
#define status_status_msg_stats_tag              3

/* Struct field encoding specification for nanopb */
extern const pb_field_t status_request_complete_fields[2];
extern const pb_field_t status_request_resend_fields[2];
extern const pb_field_t status_device_status_fields[6];
extern const pb_field_t status_status_msg_fields[4];

/* Maximum encoded size of messages (where known) */
#define status_request_complete_size             6
#define status_request_resend_size               6
#define status_device_status_size                10
#define status_status_msg_size                   12

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define STATUS_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif