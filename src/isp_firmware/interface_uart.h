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

#ifndef SRC_ISP_FIRMWARE_INTERFACE_UART_H_
#define SRC_ISP_FIRMWARE_INTERFACE_UART_H_

#include "types_isp.h"
#include "src/shared/third_party/ahdlc/src/lib/inc/frame_layer.h"

void InitUART(void);
size_t SendMessage(ahdlc_frame_encoder_t* enc_hdl, uint8_t* buffer, size_t msg_len);
API_t GetWireMessageUART0();
FunctionReturnCodes_t SendWireMessageUART0(APIMessage_t* wire_message);
void UARTreinit(void);

#endif /* SRC_ISP_FIRMWARE_INTERFACE_UART_H_ */
