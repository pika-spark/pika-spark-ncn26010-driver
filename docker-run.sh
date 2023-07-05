#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

# SYSFS_GPIO_NUMBER = ((GPIO_PORT - 1) * 32) + GPIO_PIN
# NCN26010_nIRQ = MX8MM_IOMUXC_SPDIF_RX_GPIO5_IO4
# NCN26010_nRST = MX8MM_IOMUXC_SAI5_RXD0_GPIO3_IO21
GPIO_NIRQ_NUM = 132
GPIO_NRST_NUM = 85

function finish {
  sudo echo $GPIO_NIRQ_NUM > /sys/class/gpio/unexport
  sudo echo $GPIO_NRST_NUM > /sys/class/gpio/unexport
}
trap finish EXIT

sudo echo $GPIO_NIRQ_NUM > /sys/class/gpio/export
sudo echo $GPIO_NRST_NUM > /sys/class/gpio/export

modprobe spidev
sudo chmod ugo+rw /dev/spidev0.0

docker run -it --rm -u 0 --device /dev/spidev0.0 -v /sys/class/gpio:/sys/class/gpio sh
