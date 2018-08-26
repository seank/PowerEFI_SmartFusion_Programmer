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

#include "drivers/mss_sys_services/mss_sys_services.h"
#include "src/isp_firmware/utils.h"
#include "src/isp_firmware/msg_handler.h"
#include "src/isp_firmware/interface_uart.h"
#include "hw_reg_access.h"

#if (MSS_SYS_MDDR_CONFIG_BY_CORTEX == 1)
#error "Please turn off DDR initialization! See the comment in this file above."
#endif

/* See linker script for vector details. */
extern uint32_t const * const __vector_table_start;

void init(void) {
  /*  Set vector table offset, required since we are not use the eSRAM
   *  re-map feature.
   */
  HW_set_32bit_reg(0xE000ED08u, __vector_table_start[0]);
  /* Init uart */
  InitUART();
}

int main(void) {
  init();

  while (1) {
    ScanForAPIrequest();
  }
}
