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

#ifndef SRC_ISP_FIRMWARE_PROTO_HANDLER_H_
#define SRC_ISP_FIRMWARE_PROTO_HANDLER_H_

#include "src/shared/protos/nanopb_gen/isp.pb.h"
#include "src/isp_firmware/types_isp.h"
#include "src/shared/protos/decoder_callbacks.h"


FunctionReturnCodes_t ExecuteAPICallback(const powerefi_app_isp_IspMessages* isp_msg, const struct BytesField *bf, WireMessageTXcallback_t tx_cb);
FunctionReturnCodes_t GetBitStreamDataRequest(uint32_t byte_offset, uint16_t payload_size);
FunctionReturnCodes_t RequestBitStreamData(uint16_t stream_length, uint32_t stream_offset);
APIMessage_t GetReply(void);
FunctionReturnCodes_t SendIspOperationResultNotification(uint16_t result_code, uint16_t error_code);

#endif /* SRC_ISP_FIRMWARE_PROTO_HANDLER_H_ */
