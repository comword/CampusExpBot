*Just run "apt-get install []"
libtinyxml-dev
libsqlite3-dev
python-dev
cmake
*bcm2835
Download the latest version of the library, say bcm2835-1.xx.tar.gz, then:
RUN:
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.48.tar.gz 
tar zxvf bcm2835-1.48.tar.gz
cd bcm2835-1.48
./configure
make
sudo make check
sudo make install

