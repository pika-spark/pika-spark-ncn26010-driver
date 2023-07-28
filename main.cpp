/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2023 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/pika-spark-ncn26010-driver/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <cstdlib>

#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <stdexcept>

#include "spi.h"
#include "ncn26010.h"
#include "gpio-sysfs.h"

/**************************************************************************************
 * CONSTANT
 **************************************************************************************/

static int constexpr nRST_PIN =  85;
static int constexpr nIRQ_PIN = 132;

/**************************************************************************************
 * MAIN
 **************************************************************************************/

int main(int /* argc */, char ** /* argv */) try
{
  auto const gpio_nrst = std::make_shared<SysGPIO>(nRST_PIN);
  gpio_nrst->gpio_set_dir(true);
  gpio_nrst->gpio_set_value(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  gpio_nrst->gpio_set_value(1);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  auto spi = std::make_shared<SPI>("/dev/spidev1.0", SPI_MODE_0, 8, 1000000);
  auto ncn26010 = std::make_shared<NCN26010>(spi);

  auto idver = ncn26010->read_reg(NCN26010::MemoryMapSelect::MMS_0, NCN26010::RegAddr::IDVER);
  std::cout << "IDVER = 0x" << std::hex << idver.value() << std::dec << std::endl;

  auto phyid = ncn26010->read_reg(NCN26010::MemoryMapSelect::MMS_0, NCN26010::RegAddr::PHYID);
  std::cout << "PHYID = 0x" << std::hex << phyid.value() << std::dec << std::endl;

  return EXIT_SUCCESS;
}
catch (std::runtime_error const & err)
{
  std::cerr << "Exception (std::runtime_error) caught: " << err.what() << std::endl;
  std::cerr << "Terminating ..." << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "Unhandled exception caught." << std::endl;
  std::cerr << "Terminating ..." << std::endl;
  return EXIT_FAILURE;
}
