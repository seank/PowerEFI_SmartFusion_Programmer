/* Copyright [2018] <Sean Keys> */


#ifndef SRC_SHARED_PROTOS_DECODER_CALLBACKS_H_
#define SRC_SHARED_PROTOS_DECODER_CALLBACKS_H_

#include "pb_decode.h"
#include "pb_encode.h"

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct BytesField {
  uint32_t length;
  uint8_t* bytes;
};


bool DecodeByteField(pb_istream_t *stream, const pb_field_t *field, void **arg);
bool EncodeByteField(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);

#ifdef __cplusplus
}
#endif

#endif /* SRC_SHARED_PROTOS_DECODER_CALLBACKS_H_ */
