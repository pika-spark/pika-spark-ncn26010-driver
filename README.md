`pika-spark-ncn26010-driver`
============================
Linux user space driver for the NCN26010 10BASE-T1S SPI-to-PHY driver.

### How-to-build
```bash
docker run -it -u 0 --device /dev/spidev0.0 --device /dev/spidev1.0 alpine:latest sh
apk add git g++ make cmake linux-headers
cd /tmp
git clone https://github.com/107-systems/pika-spark-ncn26010-driver && cd pika-spark-ncn26010-driver
mkdir build && cd build
cmake ..
make
```
