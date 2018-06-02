// Copyright [2018] <Sean Keys>

#include "src/shared/protos/decoder_callbacks.h"

#include "gtest/gtest.h"

#include "src/shared/protos/nanopb_gen/isp.pb.h"
#include "src/shared/protos/isp.pb.h"
#include "src/shared/protos/message_ids.h"

using powerefi::app::isp::IspMessages;
using powerefi::app::isp::GetBitStreamDataResponse;

#define BUFFER_SIZE 4096

class NanopbCallbacksTests: public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  NanopbCallbacksTests() {
    // You can do set-up work for each test here.
    /* Test buffer initialized with bytes that roll over 0-0xFF */
    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
      _test_payload[i] = i % 0xFF;
    }
  }
  virtual ~NanopbCallbacksTests() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  uint8_t _test_payload[BUFFER_SIZE];
};


TEST_F(NanopbCallbacksTests, DecodeVariableByteArray) {
  // You can do set-up work for each test here.
  const uint16_t byte_offset = 32;

  /* Top-level Message */
  IspMessages message;

  /* Allocate and set message to desired sub message */
  GetBitStreamDataResponse* bit_stream_data = message.mutable_bit_stream_data();

  /* Set fields */
  bit_stream_data->set_byte_offset(byte_offset);
  message.set_payload(_test_payload, BUFFER_SIZE);

  std::string wire_buffer;
  message.SerializeToString(&wire_buffer);

  // Test decoding via nanopb
  powerefi_app_isp_IspMessages nano_message = powerefi_app_isp_IspMessages_init_zero;

  /* Create a stream that will write to our buffer. */
  pb_istream_t nano_stream = pb_istream_from_buffer(
      (const unsigned char*)wire_buffer.c_str(), wire_buffer.length());

  /* Register callback */
  struct BytesField bytes_field;
  nano_message.payload.funcs.decode = &DecodeByteField;
  nano_message.payload.arg = &bytes_field;

  /* Now we are ready to decode the message. */
  int status = pb_decode(&nano_stream, powerefi_app_isp_IspMessages_fields,
      &nano_message);

  EXPECT_EQ(1, status);
  /* which_messages Should match assigned value in proto file */
  EXPECT_EQ(2, nano_message.which_messages);
  EXPECT_EQ(byte_offset, nano_message.messages.bit_stream_data.byte_offset);
  EXPECT_EQ(BUFFER_SIZE, bytes_field.length);
  /* Test buffer should match decoded buffer */
  EXPECT_EQ(0, memcmp(_test_payload, bytes_field.bytes, BUFFER_SIZE));
}

TEST_F(NanopbCallbacksTests, EncodeVariableByteArray) {
  /* create a buffer that's almost 2x the size we need */
  uint8_t wire_buffer[BUFFER_SIZE * 2];
  powerefi_app_isp_IspMessages nano_message =
      powerefi_app_isp_IspMessages_init_zero;

  /* Set the oneof type */
  nano_message.which_messages =
      powerefi_app_isp_IspMessages_bit_stream_data_tag;
  nano_message.payload.funcs.encode = &EncodeByteField;

  /* Create a stream that will write to our buffer. */
  pb_ostream_t stream = pb_ostream_from_buffer(wire_buffer,
      sizeof(wire_buffer));

  nano_message.messages.bit_stream_data.byte_offset = 32;
  /* create bytes field for payload */
  struct BytesField bytes_field;

  bytes_field.bytes = _test_payload;
  bytes_field.length = BUFFER_SIZE;
  nano_message.payload.arg = &bytes_field;

  /* Now we are ready to encode the message! */
  auto status = pb_encode(&stream, powerefi_app_isp_IspMessages_fields, &nano_message);
  auto message_length = stream.bytes_written;

  /* Then just check for any errors.. */
  if (!status) {
    printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
  }
  EXPECT_GT(status, 0);

  /* Test to see if we can decode using CC interface */
  /* Top-level Message */
  IspMessages message;
  auto parse_code = message.ParseFromArray(wire_buffer, message_length);
  EXPECT_EQ(true, parse_code);

  /* Check message ID to extract correct one-of */
  /* Check that we have payload */
  EXPECT_EQ(true, message.has_bit_stream_data());
  const GetBitStreamDataResponse& bit_stream_data = message.bit_stream_data();
  EXPECT_EQ(32, bit_stream_data.byte_offset());
  /* Input size should match output */
  EXPECT_EQ(bytes_field.length, message.payload().length());
  EXPECT_EQ(0, memcmp(_test_payload, message.payload().c_str(), BUFFER_SIZE));

}
