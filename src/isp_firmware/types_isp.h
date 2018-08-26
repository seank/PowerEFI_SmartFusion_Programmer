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

#ifndef SRC_ISP_FIRMWARE_TYPES_ISP_H_
#define SRC_ISP_FIRMWARE_TYPES_ISP_H_

#include "src/shared/protos/message_ids.h"
#include "src/isp_firmware/config.h"
#include "hal/CortexM3/GNU/cpu_types.h"
#include "stddef.h"
#include "string.h"


typedef enum FunctionReturnCodes {
  RET_SUCCESS = 0,
  RET_ERROR = 1
}FunctionReturnCodes_t;

typedef struct BitStreamDescriptor {
  uint8_t* data;
  uint16_t data_length;
  uint32_t bitstream_offset;
  uint8_t end_of_file;
}BitStreamDescriptor_t;

typedef FunctionReturnCodes_t (*WireMessageTXcallback_t)(APIMessage_t *);

typedef struct API {
  WireMessageTXcallback_t msg_tx_cb;
  APIMessage_t message;
}API_t;

#endif /* SRC_ISP_FIRMWARE_TYPES_ISP_H_ */
