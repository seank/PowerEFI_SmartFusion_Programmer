/* Copyright 2018 Sean Keys

   Licensed under the Apache License, Version 2.0 (the "License")
   You may not use this file except in compliance with the License.
   You may obtain a copy of the License at
      https://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "src/isp_firmware/proto_handler.h"
#include "src/isp_firmware/utils.h"
#include "src/shared/third_party/nanopb/pb_decode.h"
#include "src/shared/protos/decoder_callbacks.h"
#include "src/isp_firmware/config.h"
#include "drivers/mss_sys_services/mss_sys_services.h"
#include "src/isp_firmware/isp.h"
#include "src/isp_firmware/globals.h"
#include "src/isp_firmware/msg_handler.h"

typedef FunctionReturnCodes_t (*rqst_fun)(const powerefi_app_isp_IspMessages * isp_msg, const struct BytesField *bf);

typedef struct RequestCallback {
  rqst_fun callback;
  uint16_t request_id;
}RequestCallback_t;


/* FIXME should be able to sizeof something in isp.h file. */
static uint8_t s_pb_enc_buffer[RX_TX_BUFFER_SIZE];

/* Set defaults */
static APIMessage_t s_wire_message = {.wire_buffer = s_pb_enc_buffer,
                                      .length = 0,
                                      .msg_id = 0};
static APIMessage_t s_bitstream_block;

/* Keep this off the stack, for now */
static powerefi_app_isp_IspMessages rx_proto_msg;
static powerefi_app_isp_IspMessages tx_proto_msg;

FunctionReturnCodes_t EncodeResponse(const powerefi_app_isp_IspMessages* msg) {
  FunctionReturnCodes_t ret_code = RET_SUCCESS;

  if (!msg->which_messages) {
    return ret_code;
  }

  /* Create a stream that will write to our buffer. */
  pb_ostream_t stream = pb_ostream_from_buffer(s_wire_message.wire_buffer + sizeof(uint32_t),
      RX_TX_BUFFER_SIZE - sizeof(uint32_t));

  bool status = pb_encode(&stream, powerefi_app_isp_IspMessages_fields, msg);

  /* Then just check for any errors.. */
  if (!status) {
    //printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
  } else {
    s_wire_message.msg_id = ISP_MESSAGE;
    s_wire_message.length =  stream.bytes_written + sizeof(uint32_t);
  }

  return ret_code;
}

FunctionReturnCodes_t GetStateRequestResponse(
    const powerefi_app_isp_IspMessages * isp_msg,
    const struct BytesField * bf) {

  tx_proto_msg.which_messages = powerefi_app_isp_IspMessages_get_state_response_tag;
  tx_proto_msg.messages.get_state_response.isp_state =
      powerefi_app_isp_IspState_IDLE;

  return EncodeResponse(&tx_proto_msg);
}

FunctionReturnCodes_t GetBitStreamDataRequest(uint32_t byte_offset,
                                              uint16_t payload_size) {

  tx_proto_msg.which_messages = powerefi_app_isp_IspMessages_get_bit_stream_data_tag;
  tx_proto_msg.messages.get_bit_stream_data.byte_offset = byte_offset;
  tx_proto_msg.messages.get_bit_stream_data.length = payload_size;
  FunctionReturnCodes_t ret_code = EncodeResponse(&tx_proto_msg);

  if (ret_code == RET_SUCCESS) {
    ret_code = SendWireMessage(&s_wire_message);
  }

  return ret_code;
}

FunctionReturnCodes_t SendIspOperationResultNotification(uint16_t result_code,
    uint16_t error_code) {

  tx_proto_msg.which_messages = powerefi_app_isp_IspMessages_isp_operation_result_notification_tag;
  tx_proto_msg.messages.isp_operation_result_notification.result = result_code;
  /* TODO add error code */
  FunctionReturnCodes_t ret_code = EncodeResponse(&tx_proto_msg);

  if (ret_code == RET_SUCCESS) {
    ret_code = SendWireMessage(&s_wire_message);
  }
  return ret_code;
}

FunctionReturnCodes_t BitStreamData(const powerefi_app_isp_IspMessages* isp_msg,
                                    const struct BytesField *bf) {

  /* TODO add code to set flag/vars that point to valid data */
  s_bitstream_block.length = isp_msg->messages.bit_stream_data.byte_offset;

  return CopyToISPBuffer(bf->bytes, bf->length,
                         isp_msg->messages.bit_stream_data.byte_offset);
}

FunctionReturnCodes_t RequestBitStreamData(uint16_t stream_length,
                                           uint32_t stream_offset) {
  FunctionReturnCodes_t ret_code;

  tx_proto_msg.which_messages = powerefi_app_isp_IspMessages_get_bit_stream_data_tag;
  tx_proto_msg.messages.get_bit_stream_data.byte_offset = stream_offset;
  tx_proto_msg.messages.get_bit_stream_data.length = stream_length;

  ret_code = EncodeResponse(&tx_proto_msg);

  if (ret_code) {
    ret_code = SendWireMessage(&s_wire_message);
  }

  return ret_code;
}


FunctionReturnCodes_t LaunchISPmode(const powerefi_app_isp_IspMessages* isp_msg, const struct BytesField *bf) {
  FunctionReturnCodes_t ret_code = RET_SUCCESS;

  uint8_t mss_ret;
  MSS_SYS_init(MSS_SYS_NO_EVENT_HANDLER);

  switch (isp_msg->messages.set_isp_mode_request.isp_mode) {
  case  powerefi_app_isp_IspMode_AUTHENTICATE:
    mss_ret = LaunchISP(MSS_SYS_PROG_AUTHENTICATE, page_read_handler, isp_completion_handler);
    break;
  case powerefi_app_isp_IspMode_PROGRAM:
    mss_ret = LaunchISP(MSS_SYS_PROG_PROGRAM, page_read_handler, isp_completion_handler);
    break;
  case powerefi_app_isp_IspMode_VERIFY:
    mss_ret = LaunchISP(MSS_SYS_PROG_VERIFY, page_read_handler, isp_completion_handler);
    break;
  case powerefi_app_isp_IspMode_DUMMY_PROGRAM:
    break;
  default:
    break;
  }
  /* If Init of MSS_SYS was successful set busy flag, as transactions will
   * be ISR driven, until operation in complete.
   */
  if (mss_ret == MSS_SYS_SUCCESS) {
    g_isp_busy = 1;
  } else {
    ret_code = RET_ERROR;
  }

  while (g_isp_busy) {
    /* ISP operations are ISR driven */
  }

  return ret_code;
}


static RequestCallback_t request_callbacks[] = {
    {GetStateRequestResponse, powerefi_app_isp_IspMessages_get_state_request_tag},
    {BitStreamData, powerefi_app_isp_IspMessages_bit_stream_data_tag},
    {LaunchISPmode, powerefi_app_isp_IspMessages_set_isp_mode_request_tag}
};

static const uint16_t callback_table_rows = TABLE_ROWS(request_callbacks, RequestCallback_t);


/*  Find and execute the registered callback.
 *
 *  TODO (skeys) there are a couple ways to improve this:
 *    1. We could use a binary search if this array gets much bigger
 *    2. We could modify the nanoPB generator script to add a field to the
 *       message, and just map the callback at compile time.
 */
FunctionReturnCodes_t ExecuteAPICallback(
    const powerefi_app_isp_IspMessages* isp_msg, const struct BytesField *bf,
    WireMessageTXcallback_t tx_cb) {

  size_t i;
  uint16_t key = isp_msg->which_messages;
  FunctionReturnCodes_t ret_code = RET_ERROR;

  /* Executes all matching functions */
  for (i = 0; i < callback_table_rows; ++i) {
    if (request_callbacks[i].request_id == key) {
      bzero(&tx_proto_msg, sizeof(tx_proto_msg));
      ret_code = request_callbacks[i].callback(isp_msg, bf);
      /* If callback was successful, send the reply. */
      if (ret_code == RET_SUCCESS) {
        tx_cb(&s_wire_message);
      } else {
        break;
      }
    }
  }

  return ret_code;
}


/*
 * Decodes a ISP proto message.
 */
FunctionReturnCodes_t DecodeProtoMessage(API_t * api) {

  FunctionReturnCodes_t ret_code = RET_SUCCESS;

  bzero(&rx_proto_msg, sizeof(rx_proto_msg));

  /* Create a stream that will write to our buffer. */
  pb_istream_t nano_stream = pb_istream_from_buffer(
      (const unsigned char*) api->message.wire_buffer, api->message.length);

  /* Register callback */
  struct BytesField bytes_field;
  rx_proto_msg.payload.funcs.decode = &DecodeByteField;
  rx_proto_msg.payload.arg = &bytes_field;

  /* Now we are ready to decode the message. */
  int status = pb_decode(&nano_stream, powerefi_app_isp_IspMessages_fields,
      &rx_proto_msg);

  if (status) {
    ret_code = ExecuteAPICallback(&rx_proto_msg, &bytes_field, api->msg_tx_cb);
  } else {
    ret_code = RET_ERROR;
  }

  return ret_code;
}
