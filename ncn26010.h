/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2023 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/pika-spark-ncn26010-driver/graphs/contributors.
 */

#ifndef PIKA_SPARK_NCN26010_DRIVER_NCN26010_H
#define PIKA_SPARK_NCN26010_DRIVER_NCN26010_H

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <memory>
#include <optional>

#include "spi.h"

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class NCN26010
{
public:
  enum class MemoryMapSelect : uint8_t
  {
    MMS_0  =  0, /* Standard SPI Control and Status, PHY MIIM. */
    MMS_1  =  1, /* MAC registers. */
    MMS_2  =  2, /* PHY − PCS Registers (IEEE802.3 MMD3) */
    MMS_3  =  3, /* PHY − PMA/PMD Registers (IEEE802.3 MMD 1) */
    MMS_4  =  4, /* PHY – PCLA and vendor specific Registers (MMD 31) */
    MMS_12 = 16, /* Vendor Specific Registers */
  };
  enum class RegAddr : uint16_t
  {
    /* MMS_0 */
    IDVER = 0x0000,
    PHYID = 0x0001,
  };


  NCN26010(std::shared_ptr<SPI> const spi);


  std::optional<uint32_t> read_reg(MemoryMapSelect const mms, RegAddr const reg_addr);


private:
  std::shared_ptr<SPI> const _spi;

  typedef union
  {
    struct
    {
      uint32_t P    : 1;
      uint32_t LEN  : 7;
      uint32_t ADDR : 16;
      uint32_t MMS  : 4;
      uint32_t AID  : 1;
      uint32_t WNR  : 1;
      uint32_t HDRB : 1;
      uint32_t DNC  : 1;
    } bits;
    uint8_t bytes[4];
    uint32_t data;
  } CommandHeader;

  enum class CommandType : uint32_t
  {
    Control = 0,
    Data    = 1,
  };

  enum class RegAddrIncrement : uint32_t
  {
    Enable  = 0,
    Disable = 1,
  };

  enum class RegAccessType : uint32_t
  {
    Read  = 0,
    Write = 1,
  };

  static uint32_t calc_parity(CommandHeader const cmd_hdr);
};

#endif /* PIKA_SPARK_NCN26010_DRIVER_NCN26010_H */
