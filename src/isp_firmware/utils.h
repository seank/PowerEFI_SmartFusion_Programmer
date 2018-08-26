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

#ifndef SRC_ISP_FIRMWARE_UTILS_H_
#define SRC_ISP_FIRMWARE_UTILS_H_

#include "types_isp.h"

#define TABLE_ROWS(TABLE, TABLE_ELEMENT) (sizeof(TABLE)/sizeof(TABLE_ELEMENT))

static inline void SpinDelay(volatile uint64_t n) {
  while (n)
    --n;
}

#endif /* SRC_ISP_FIRMWARE_UTILS_H_ */
