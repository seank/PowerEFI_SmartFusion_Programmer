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

#include "src/isp_firmware/msg_handler.h"
#include "src/isp_firmware/proto_handler.h"
#include "src/isp_firmware/interface_uart.h"


FunctionReturnCodes_t APIHandler(API_t * api) {
  FunctionReturnCodes_t ret_code;

  switch (api->message.msg_id) {
  case ISP_MESSAGE:
    ret_code = DecodeProtoMessage(api);
    break;
  default:
    /* TODO (skeys) send invalid message type error. */
    break;
  }

  return ret_code;
}

FunctionReturnCodes_t ScanForAPIrequest(void) {
  API_t api;
  FunctionReturnCodes_t ret_code = RET_ERROR;

  api = GetWireMessageUART0();
  if (api.message.length) {
    ret_code = APIHandler(&api);
  }
  /* TODO (skeys) Add other interfaces here (USB OTG, BT, eNet). */
  return ret_code;
}

FunctionReturnCodes_t GetBitStreamBlock(uint16_t stream_length, uint16_t stream_offset) {
  FunctionReturnCodes_t ret_code = RequestBitStreamData(stream_length, stream_offset);

  if (ret_code == RET_SUCCESS) {
    ret_code = ScanForAPIrequest();
  }

  return ret_code;
}

FunctionReturnCodes_t SendWireMessage(APIMessage_t* wire_message) {
  /* TODO use registered callback vs hard coding it to UART0 */
  return SendWireMessageUART0(wire_message);
}
