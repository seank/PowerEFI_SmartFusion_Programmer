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

#ifndef SRC_ISP_FIRMWARE_CONFIG_H_
#define SRC_ISP_FIRMWARE_CONFIG_H_

/* ISP does not use interrupts, making it easy to use the same buffer for RX and TX */
#define PAYLOAD_SIZE       (4096)
#define RX_TX_BUFFER_SIZE  (PAYLOAD_SIZE + 128)


#endif /* SRC_ISP_FIRMWARE_CONFIG_H_ */
