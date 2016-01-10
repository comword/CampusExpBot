# MFRC522 RFID Card Reader

#### MFRC522 Python Library for Raspberry Pi
  * Preparation
  ```
  $ sudo nano /etc/modprobe.d/raspi-blacklist.conf
  #blacklist spi-bcm2708
  ```
  ```
  $ sudo apt-get install python-dev
  $ git clone https://github.com/lthiery/SPI-Py.git
  $ cd SPI-Py
  $ sudo python setup.py install
  ```

  * Using Example
  ```
  $ git clone https://github.com/rasplay/MFRC522-python.git
  $ cd MFRC522-python
  $ sudo python read.py
  ```

* [GeraintW Online Blog-RFID and Raspberry PI](http://geraintw.blogspot.kr/2014/01/rfid-and-raspberry-pi.html)

* [Hardware SPI as a C Extension for Python](https://github.com/lthiery/SPI-Py)

* [A small class to interface with the NFC reader Module MFRC522](https://github.com/mxgxw/MFRC522-python)
