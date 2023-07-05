# Copyright Alexander Entinger, MSc / LXRobotics GmbH
# This docker file allows building and running of 10BASE-T1S user space driver for the onsemi NCN26010 SPI-MAC-PHY.
FROM alpine:3.18

RUN apk add git g++ make cmake linux-headers && \
    cd /tmp && \
    git clone https://github.com/107-systems/pika-spark-ncn26010-driver && \
    cd pika-spark-ncn26010-driver && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make
