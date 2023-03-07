<a href="https://pika-spark.io/"><img align="right" src="https://raw.githubusercontent.com/pika-spark/.github/main/logo/logo-pika-spark-bg-white.png" width="15%"></a>
:sparkles: `pika-spark-ncn26010-driver`
=======================================
[![Spell Check status](https://github.com/pika-spark/pika-spark-ncn26010-driver/actions/workflows/spell-check.yml/badge.svg)](https://github.com/pika-spark/pika-spark-ncn26010-driver/actions/workflows/spell-check.yml)

Linux user space driver for the [NCN26010](https://www.onsemi.com/products/interfaces/ethernet-controllers/ncn26010) 10BASE-T1S SPI-to-PHY driver.

<p align="center">
  <a href="https://pika-spark.io/"><img src="https://raw.githubusercontent.com/pika-spark/.github/main/logo/logo-pika-spark-bg-white-github.png" width="40%"></a>
</p>

### How-to-build
```bash
docker pull alpine:latest
docker run -it -u 0 --device /dev/spidev0.0 --device /dev/spidev1.0 alpine:latest sh
apk add git g++ make cmake linux-headers
cd /tmp
git clone https://github.com/107-systems/pika-spark-ncn26010-driver && cd pika-spark-ncn26010-driver
mkdir build && cd build
cmake ..
make
```
