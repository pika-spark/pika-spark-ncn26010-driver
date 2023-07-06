#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

if [ "$(id -u)" != "0" ]; then
  echo "This script must be run as root."
  exit 1
fi

# SYSFS_GPIO_NUMBER = ((GPIO_PORT - 1) * 32) + GPIO_PIN
# NCN26010_nIRQ = MX8MM_IOMUXC_SPDIF_RX_GPIO5_IO4
# NCN26010_nRST = MX8MM_IOMUXC_SAI5_RXD0_GPIO3_IO21
GPIO_NIRQ_NUM=132
GPIO_NRST_NUM=85

function finish {
  echo $GPIO_NIRQ_NUM > /sys/class/gpio/unexport
  echo $GPIO_NRST_NUM > /sys/class/gpio/unexport
}
trap finish EXIT

echo $GPIO_NIRQ_NUM > /sys/class/gpio/export
echo $GPIO_NRST_NUM > /sys/class/gpio/export

echo in > /sys/class/gpio/gpio$GPIO_NIRQ_NUM/direction
echo out > /sys/class/gpio/gpio$GPIO_NRST_NUM/direction

modprobe spidev
chmod ugo+rw /dev/spidev0.0

sudo -u fio docker run -it --rm -u 0 --privileged --device /dev/spidev0.0 -v /sys/class/gpio:/sys/class/gpio pika_spark_ncn26010_driver
