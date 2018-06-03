/*
 * message_ids.h
 *
 */

#ifndef SHARED_PROTOS_MESSAGE_IDS_H_
#define SHARED_PROTOS_MESSAGE_IDS_H_


/* Over the wire message IDs */
typedef enum {
  ISP_SERVICE = 1,
  ISP_MESSAGE = 2
}MessageID;

typedef struct APIMessage {
  MessageID id;
  uint16_t length;
  uint8_t* wire_buffer;
}APIMessage_t;

#endif /* SHARED_PROTOS_MESSAGE_IDS_H_ */
