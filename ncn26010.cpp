/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2023 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/pika-spark-ncn26010-driver/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "ncn26010.h"

#include <cstring>

#include <stdexcept>

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

NCN26010::NCN26010(std::shared_ptr<SPI> const spi)
: _spi{spi}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

std::optional<uint32_t> NCN26010::read_reg(MemoryMapSelect const mms, RegAddr const reg_addr)
{
  CommandHeader cmd_header_tx;

  cmd_header_tx.bits.DNC  = static_cast<uint32_t>(CommandType::Control);
  cmd_header_tx.bits.HDRB = 0;
  cmd_header_tx.bits.WNR  = static_cast<uint32_t>(RegAccessType::Read);
  cmd_header_tx.bits.AID  = static_cast<uint32_t>(RegAddrIncrement::Disable); /* Read only one byte. */
  cmd_header_tx.bits.MMS  = static_cast<uint32_t>(mms);
  cmd_header_tx.bits.ADDR = static_cast<uint32_t>(reg_addr);
  cmd_header_tx.bits.LEN  = 0; /* Read only one byte. */
  cmd_header_tx.bits.P    = 0;
  cmd_header_tx.bits.P    = calc_parity(cmd_header_tx);

  size_t constexpr IGNORED_ECHO_BYTES = 4;
  size_t constexpr TX_BUF_SIZE = sizeof(cmd_header_tx.data) + IGNORED_ECHO_BYTES + 4 /* requested reg is received here */;
  size_t constexpr RX_BUF_SIZE = TX_BUF_SIZE;
  uint8_t const tx_buf[TX_BUF_SIZE] =
  {
    cmd_header_tx.bytes[3],
    cmd_header_tx.bytes[2],
    cmd_header_tx.bytes[1],
    cmd_header_tx.bytes[0],
    0,0,0,0,
    0,0,0,0
  };
  uint8_t rx_buf[RX_BUF_SIZE] = {0};

  /* Do the actual SPI transfer. */
  _spi->transfer(tx_buf, rx_buf, TX_BUF_SIZE);

  CommandHeader cmd_header_rx;
  memmove(cmd_header_rx.bytes, (rx_buf + IGNORED_ECHO_BYTES), sizeof(cmd_header_rx.data));
  /* Convert to big endian. */
  cmd_header_rx.data = __builtin_bswap32(cmd_header_rx.data);

  if (cmd_header_rx.bits.HDRB == 1) /* HDRB will be 1 in case of parity error. */
    throw std::runtime_error("Header parity error while reading from MACPHY.");

  if (cmd_header_tx.bits.LEN == 0)
  {
    uint32_t reg_val = 0;
    memmove((uint8_t *)&reg_val, rx_buf + IGNORED_ECHO_BYTES + sizeof(cmd_header_rx.data), sizeof(reg_val));
    reg_val = __builtin_bswap32(reg_val);
    return reg_val;
  }

  return std::nullopt;
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

uint32_t NCN26010::calc_parity(CommandHeader const cmd_hdr)
{
  size_t bit_set_cnt = 0;

  for(size_t bit = 0; bit < 32; bit++)
  {
    auto isBitSet = [](uint32_t const data, size_t const bit)->bool { return ((data & (1<<bit)) == static_cast<uint32_t>(1<<bit)); };
    if (isBitSet(cmd_hdr.data, bit))
      bit_set_cnt++;
  }

  bool is_bit_set_cnt_even = (bit_set_cnt % 2) == 0;

  if (is_bit_set_cnt_even)
    return 1;
  else
    return 0;
};
