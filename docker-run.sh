#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

# SYSFS_GPIO_NUMBER = ((GPIO_PORT - 1) * 32) + GPIO_PIN
# NCN26010_nIRQ = MX8MM_IOMUXC_SPDIF_RX_GPIO5_IO4
GPIO_NIRQ_PORT = 5
GPIO_NIRQ_PIN = 4
GPIO_NIRQ_NUM = $(($GPIO_NIRQ_PORT - 1) * 32 + $GPIO_NIRQ_PIN)
# NCN26010_nRST = MX8MM_IOMUXC_SAI5_RXD0_GPIO3_IO21
GPIO_NRST_PORT = 3
GPIO_NRST_PIN = 21
GPIO_NRST_NUM = $(($GPIO_NRST_PORT - 1) * 32 + $GPIO_NRST_PIN)

function finish {
  echo $GPIO_NIRQ_NUM > /dev/gpio/unexport
  echo $GPIO_NRST_NUM > /dev/gpio/unexport
}
trap finish EXIT

echo $GPIO_NIRQ_NUM > /dev/gpio/export
echo $GPIO_NRST_NUM > /dev/gpio/export

modprobe spidev
sudo chmod ugo+rw /dev/spidev0.0

docker run -it -u 0 --device /dev/spidev0.0 sh
