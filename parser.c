#include "parser.h"

// This is the function for testing the parser. Should perform a local loopback
// Make sure the Tx is physically tied to Rx without the Arduino Shield.
//bool parserTest(server_msg *received_msg) {
//    uint8_t return_buffer[100] = {0};
//    dbgOutputEvent(DBG_START_PROTOPARSE);
//
//    pb_istream_t d = pb_istream_from_buffer(received_msg->msg, received_msg->length);
//    bool worked = false;
//    
//    server_server receive = server_server_init_zero;
//    dbgOutputEvent(DBG_BEFORE_DECODE);
//    if(!pb_decode_delimited(&d, server_server_fields, &receive)) {
//        dbgOutputEvent(DBG_AFTER_DECODE_FAILED);
//        return false;
//    }
//    dbgOutputEvent(DBG_AFTER_DECODE);
//    
//    if(strcmp(receive.uid, "server") != 0 && strcmp(receive.uid, "rover2") != 0) return false;
//    dbgOutputEvent(DBG_AFTER_DECODE_DATA_VALID);
//    
//    // For testing, this loopback should print out both the encoded and decoded versions of the message uid
//    server_msg t;
//    strncat(t.msg, receive.uid, strlen(receive.uid));
//    strncat(t.msg, "loop\n", 5);
//    t.length = strlen(t.msg);
//    t.index = t.length;
//    writeToWiflyTx(t);
//    
//    // send response message
//    server_server response_msg = server_server_init_zero;
//    sprintf(response_msg.uid, "%s", "rover2");
//    response_msg.sequence_number = receive.sequence_number;
//    response_msg.which_payload = server_server_done_tag;
//    response_msg.payload.done.sequence_number = receive.sequence_number;
//
//    pb_ostream_t en = pb_ostream_from_buffer(return_buffer, sizeof(return_buffer));
//    worked = pb_encode_delimited(&en, server_server_fields, &receive);  // loop back, sends exact data it received
//    if(!worked) return worked;
//    
//    // Send the loop message
//    received_msg->length = en.bytes_written+6;
//    received_msg->index = received_msg->length;
//    memset(received_msg->msg, 0, sizeof(received_msg->msg));
//    sprintf(received_msg->msg, "%s%s%s", "~~~", return_buffer, "}}}");
//    writeToWiflyTx(*received_msg);
//    return worked;
//}


bool serverLoopback(server_msg *received_msg) {
    uint8_t return_buffer[100] = {0};
    dbgOutputEvent(DBG_START_PROTOPARSE);

    pb_istream_t d = pb_istream_from_buffer(received_msg->msg, received_msg->length);
    bool worked = false;
    
    server_server receive = server_server_init_zero;
    dbgOutputEvent(DBG_BEFORE_DECODE);
    if(!pb_decode_delimited(&d, server_server_fields, &receive)) {
        dbgOutputEvent(DBG_AFTER_DECODE_FAILED);
        return false;
    }
    dbgOutputEvent(DBG_AFTER_DECODE);
    
    if(strcmp(receive.uid, "server") != 0 && strcmp(receive.uid, "rover2") != 0) return false;
    dbgOutputEvent(DBG_AFTER_DECODE_DATA_VALID);

    // send response message
    worked = sendDoneMessage(receive.sequence_number);

    return worked;
}


// This is the function to call for parsing the proto messages in the official server
bool parseProto(server_msg *received_msg) {
    static int last_sn_recv = 0;
    uint8_t return_buffer[100] = {0};
    dbgOutputEvent(DBG_START_PROTOPARSE);

    pb_istream_t d = pb_istream_from_buffer(received_msg->msg, received_msg->length);
    bool worked = false;
    
    server_server receive = server_server_init_zero;
    dbgOutputEvent(DBG_BEFORE_DECODE);
    if(!pb_decode_delimited(&d, server_server_fields, &receive)) {
        dbgOutputEvent(DBG_AFTER_DECODE_FAILED);
//        requestResend(last_sn_recv+1);
        
        return false;
    }
    dbgOutputEvent(DBG_AFTER_DECODE);
    
    if(strcmp(receive.uid, "server") != 0 && strcmp(receive.uid, "Trainer") != 0) return false;
    dbgOutputEvent(DBG_AFTER_DECODE_DATA_VALID);
    
    last_sn_recv = receive.sequence_number;
//    worked = sendDoneMessage(receive.sequence_number);
    
    // Got a motor message? Send it out to the motorQ
    if(receive.which_payload == server_server_mCmd_tag) {
        
            // If bad data, resend
            if(receive.payload.mCmd.direction < 1 || receive.payload.mCmd.direction > 9) {
                requestResend(receive.sequence_number);
            } else {
                motor_msg m;
                m.dir = receive.payload.mCmd.direction;
                m.seq_num = receive.sequence_number;
                writeToMotorQueue(m);
            }
        
    }
    return true;
}


bool sendDoneMessage(int32_t sequence_number) {
    uint8_t return_buffer[100] = {0};
    bool worked = false; 
    
    // send response message
    server_server response_msg = server_server_init_zero;
    sprintf(response_msg.uid, "%s", "Trainer");
    response_msg.sequence_number = sequence_number;
    response_msg.which_payload = server_server_done_tag;
    response_msg.payload.done.sequence_number = sequence_number;

    pb_ostream_t en = pb_ostream_from_buffer(return_buffer, sizeof(return_buffer));
    worked = pb_encode_delimited(&en, server_server_fields, &response_msg);
    if(!worked) return worked;
    
    server_msg response;
    response.length = en.bytes_written+6;
    response.index = response.length;
    memset(response.msg, 0, sizeof(response.msg));
    sprintf(response.msg, "%s%s%s", "~~~", return_buffer, "}}}");
    writeToWiflyTx(response);
    
    return true;
}


bool requestResend(int32_t sequence_number) {
    uint8_t return_buffer[100] = {0};
    bool worked = false; 
    
    // send a resend request
    server_server response_msg = server_server_init_zero;
    sprintf(response_msg.uid, "%s", "Trainer");
    response_msg.sequence_number = sequence_number;
    response_msg.which_payload = server_server_done_tag;
    response_msg.payload.again.sequence_number = sequence_number;

    pb_ostream_t en = pb_ostream_from_buffer(return_buffer, sizeof(return_buffer));
    worked = pb_encode_delimited(&en, server_server_fields, &response_msg);
    if(!worked) return worked;
    
    server_msg response;
    response.length = en.bytes_written+6;
    response.index = response.length;
    memset(response.msg, 0, sizeof(response.msg));
    sprintf(response.msg, "%s%s%s", "~~~", return_buffer, "}}}");
    writeToWiflyTx(response);
    
    return true;
}
