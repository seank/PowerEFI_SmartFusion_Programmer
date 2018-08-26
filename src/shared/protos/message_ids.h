/* Copyright 2018 Sean Keys

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
      https://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef SHARED_PROTOS_MESSAGE_IDS_H_
#define SHARED_PROTOS_MESSAGE_IDS_H_

#include "stdint.h"

/* Over the wire message IDs */
typedef enum MessageID {
  ISP_SERVICE = 1,
  ISP_MESSAGE = 2
}MessageID_t;

/* Wire frame format */
typedef struct APIMessage {
  MessageID_t msg_id;
  uint16_t length;
  uint8_t* wire_buffer;
}__attribute__((__packed__)) APIMessage_t;


#endif /* SHARED_PROTOS_MESSAGE_IDS_H_ */
