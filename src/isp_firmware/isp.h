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

#ifndef SRC_ISP_FIRMWARE_ISP_H_
#define SRC_ISP_FIRMWARE_ISP_H_

#include "src/isp_firmware/types_isp.h"
#include "drivers/mss_sys_services/mss_sys_services.h"


uint32_t page_read_handler(uint8_t const ** pp_next_page);
void isp_completion_handler(uint32_t value);
//dummy
void dummy_isp_completion_handler(uint32_t value);
uint32_t dummy_page_read_handler(uint8_t const ** pp_next_page);
uint8_t LaunchISP(uint8_t mode, comblk_page_handler_t page_read_handler,
    sys_serv_isp_complete_handler_t isp_completion_handler);
FunctionReturnCodes_t CopyToISPBuffer(const uint8_t *src, const uint16_t length,
    const uint32_t stream_offset);

#endif /* SRC_ISP_FIRMWARE_ISP_H_ */
