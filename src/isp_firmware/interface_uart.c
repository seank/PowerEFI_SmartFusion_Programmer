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

#include "src/isp_firmware/interface_uart.h"
#include "mss_uart.h"
#include "src/shared/third_party/ahdlc/src/lib/inc/frame_layer.h"
#include "src/shared/third_party/ahdlc/src/lib/inc/crc_16.h"
#include "src/isp_firmware/msg_handler.h"
#include "src/isp_firmware/types_isp.h"
#include "mss_gpio.h"

static mss_uart_instance_t * const s_uart0 = &g_mss_uart0;
static uint8_t uart_initialized = 0;
static uint8_t ahdlc_initialized = 0;

static ahdlc_frame_encoder_t s_encoder_handle;
static ahdlc_frame_decoder_t s_decoder_handle;
static uint8_t s_rx_tx_buffer[RX_TX_BUFFER_SIZE];

void UARTreinit(void) {
  uart_initialized = 0;
}

void InitUART(void) {
  if (!uart_initialized) {
    MSS_UART_init(s_uart0, MSS_UART_57600_BAUD,
    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    uart_initialized = 1;
  }

  if (!ahdlc_initialized) {
    s_decoder_handle.pdu_buffer = s_rx_tx_buffer;
    s_decoder_handle.buffer_len = RX_TX_BUFFER_SIZE;

    s_encoder_handle.frame_buffer = s_rx_tx_buffer;
    s_encoder_handle.buffer_len = RX_TX_BUFFER_SIZE;

    ahdlc_op_return decoder_ret;
    decoder_ret = AhdlcDecoderInit(&s_decoder_handle, CRC16, NULL);

    if (decoder_ret != AHDLC_OK) {
      /* TODO assert error */
    }

    ahdlc_op_return encoder_ret;
    encoder_ret = ahdlcEncoderInit(&s_encoder_handle, CRC16);

    if (encoder_ret != AHDLC_OK) {
      /* TODO assert error */
    }

    ahdlc_initialized = 1;
  }
}

size_t UART_Polled_Rx(uint8_t * rx_buff, size_t rx_size) {

  size_t spin_retries = 0xFFFF0;
  size_t rx_bytes = 0U;

  if (!uart_initialized) {
    InitUART();
  }

  while (rx_bytes < rx_size) {
    while (((s_uart0->hw_reg->LSR) & 0x1) != 0U) {
      rx_buff[rx_bytes] = s_uart0->hw_reg->RBR;
      ++rx_bytes;
      spin_retries = 0xFFFF0;
      if (rx_size == rx_bytes) {
        break;
      }
    }
    /* If we fail to read, toggle LED. */
    if (!--spin_retries) {
      uint32_t gpio_state = MSS_GPIO_get_outputs();
      gpio_state ^= (MSS_GPIO_1_MASK & MSS_GPIO_2_MASK);
      MSS_GPIO_set_outputs(gpio_state);
      break;
    }
  }

  return rx_bytes;
}

ahdlc_op_return GetUARTahdlcFrame() {

  ahdlc_op_return ret = AHDLC_OK;
  uint8_t byte;

  while (1) {
    size_t rx_bytes = UART_Polled_Rx(&byte, sizeof(byte));

    if (rx_bytes) {
      ret = DecodeFrameByte(&s_decoder_handle, byte);
      if (ret == AHDLC_COMPLETE) {
        break;
      } else {
        if (ret != AHDLC_OK) {
          break;
        }
      }
    } else {
      break;
    }
  }

  return ret;
}

FunctionReturnCodes_t SendWireMessageUART0(APIMessage_t* wire_message) {
  FunctionReturnCodes_t ret_code = RET_SUCCESS;

  if (EncodeNewFrame(&s_encoder_handle) == AHDLC_OK) {
    *((uint32_t *)wire_message->wire_buffer) = wire_message->msg_id;
    if (EncodeBuffer(&s_encoder_handle, wire_message->wire_buffer, wire_message->length) == AHDLC_OK) {
      if (!uart_initialized) {
        InitUART();
      }
      MSS_UART_polled_tx(s_uart0, s_encoder_handle.frame_buffer, s_encoder_handle.frame_info.buffer_index);
    }
  }

  return ret_code;
}

API_t GetWireMessageUART0() {
  /* TODO add more sources of wire messages */
  API_t api = {};
  if (GetUARTahdlcFrame() == AHDLC_COMPLETE) {
    api.message  = AHDLCDecodeWireFields(&s_decoder_handle);
    api.msg_tx_cb = SendWireMessageUART0;
  }

  return api;
}
