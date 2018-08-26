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

#ifndef SRC_ISP_FIRMWARE_MSG_HANDLER_H_
#define SRC_ISP_FIRMWARE_MSG_HANDLER_H_

#include "stdint.h"

#include "src/isp_firmware/types_isp.h"
#include "src/shared/third_party/ahdlc/src/lib/inc/frame_layer.h"
#include "src/isp_firmware/proto_handler.h"

static inline APIMessage_t AHDLCDecodeWireFields(const ahdlc_frame_decoder_t* decoder_ctx) {

  APIMessage_t wire_message;
  uint32_t id = *((uint32_t*)decoder_ctx->pdu_buffer);

  #ifdef __BIG_ENDIAN__
  uint32_t b0, b1, b2, b3;

  b0 = msg_id >> 24u;
  b1 = (msg_id & 0x00ff0000) >> 8u;
  b2 = (msg_id & 0x0000ff00) << 8u;
  b3 = msg_id << 24u;

  wire_message.msg_id  = b0 | b1 | b2 | b3;
  #else
  wire_message.msg_id = id;
  #endif

  wire_message.length = decoder_ctx->frame_info.buffer_index - sizeof(id);
  wire_message.wire_buffer = decoder_ctx->pdu_buffer + sizeof(uint32_t);

  return wire_message;
}

FunctionReturnCodes_t ScanForAPIrequest(void);
FunctionReturnCodes_t DecodeProtoMessage(API_t * api);
FunctionReturnCodes_t SendWireMessage(APIMessage_t * wire_message);

#endif /* SRC_ISP_FIRMWARE_MSG_HANDLER_H_ */
