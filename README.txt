             Simple remote controlled powerswitch

Features

* based on cheap esp hw
* standard IP multicast based communication protocol, similar to MQTT
  but here it is a brokerless, multicasted variant.
* communication secured using mbedtls.
* linux simulator as example.


Linux simulator :

[per@localhost load-switch]$ LD_LIBRARY_PATH=../../simple-iot ./load-control
    initializing mbedtls
    parsing public key
requesting channel 1 on
led 1 on
requesting channel 1 off
led 1 off
requesting channel 1 on
led 1 on
requesting channel 1 off
led 1 off
requesting channel 1 on
led 1 on
requesting channel 1 off
led 1 off
...

[per@localhost load-switch]$ LD_LIBRARY_PATH=../../simple-iot ./load-switch
    initializing mbedtls
    parsing public key
mosfet 1 on
mosfet 1 off
mosfet 1 on
mosfet 1 off
mosfet 1 on
mosfet 1 off
mosfet 1 on
mosfet 1 off
...
