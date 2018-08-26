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

#include "src/isp_firmware/isp.h"
#include "drivers/mss_sys_services/mss_sys_services.h"
#include "mss_uart.h"
#include "src/isp_firmware/utils.h"
#include "mss_gpio.h"
#include "src/isp_firmware/globals.h"
#include "src/isp_firmware/interface_uart.h"
#include "src/isp_firmware/proto_handler.h"
#include "src/isp_firmware/msg_handler.h"


static uint8_t isp_payload[PAYLOAD_SIZE];
static uint32_t requested_bitstream_index;

static BitStreamDescriptor_t stream_desc;

FunctionReturnCodes_t CopyToISPBuffer(const uint8_t *src, const uint16_t length,
    const uint32_t stream_offset){

  FunctionReturnCodes_t ret_code = RET_SUCCESS;

  if (length > PAYLOAD_SIZE) {
    ret_code = RET_ERROR;
  } else {
    /* Copy buffer */
    memcpy(isp_payload, src, length);
    stream_desc.bitstream_offset = stream_offset;
    stream_desc.data = isp_payload;
    stream_desc.data_length = length;
    /* TODO explicitly set via proto field from host */
    if (stream_desc.data_length < PAYLOAD_SIZE) {
      stream_desc.end_of_file = 1;
    }
  }

  return ret_code;
}


uint8_t LaunchISP(uint8_t mode, comblk_page_handler_t page_read_handler,
    sys_serv_isp_complete_handler_t isp_completion_handler) {

  requested_bitstream_index = 0;
  bzero((void*)&stream_desc, sizeof(stream_desc));
  UARTreinit();
  return MSS_SYS_start_isp(mode, page_read_handler, isp_completion_handler);
}


uint32_t page_read_handler(uint8_t const ** pp_next_page) {

  FunctionReturnCodes_t ret_code;

  if (!stream_desc.end_of_file) {
    do {
      ret_code = GetBitStreamDataRequest(requested_bitstream_index, PAYLOAD_SIZE);
      if (ret_code == RET_SUCCESS) {
        ret_code = ScanForAPIrequest();
      }
    } while (requested_bitstream_index != stream_desc.bitstream_offset || stream_desc.data_length == 0);
  } else {
    stream_desc.data_length = 0;
  }

  *pp_next_page = stream_desc.data;
  requested_bitstream_index += stream_desc.data_length;

  return stream_desc.data_length;
}

/* ISP function to get status after completion of ISP operation. */
void isp_completion_handler(uint32_t value) {
  SendIspOperationResultNotification((uint16_t) value, 0);
  g_isp_busy = 0;
}
