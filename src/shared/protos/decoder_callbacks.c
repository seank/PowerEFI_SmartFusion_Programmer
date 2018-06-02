/* Copyright [2018] <Sean Keys> */

#include "src/shared/protos/decoder_callbacks.h"
#include "src/shared/protos/nanopb_gen/isp.pb.h"

bool DecodeByteField(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  bool result = true;
  struct BytesField* bytes_field = (struct BytesField*)*arg;

  /* Only decode if we recognize the field ID */
  switch (field->tag) {
  case powerefi_app_isp_IspMessages_payload_tag:
    bytes_field->bytes= stream->state;
    bytes_field->length = stream->bytes_left;
    break;
  default:
    result = false;
    break;
  }
  /* Mark the buffer as consumed, to tell the caller that we are done */
  stream->bytes_left = 0;

  return result;
}

bool EncodeByteField(pb_ostream_t *stream, const pb_field_t *field, void * const * arg) {
  /* Check for NULL ptr */
  if (*arg == NULL) {
    return false;
  }

  struct BytesField* bytes_field = (struct BytesField *)*arg;

  if (!pb_encode_tag_for_field(stream, field))
    return false;

  return pb_encode_string(stream, bytes_field->bytes, bytes_field->length);
}

