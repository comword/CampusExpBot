/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "uart.h"
#include "xmlhandler.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <stdexcept>

Uart::Uart()
{
	fd = new int;
	*fd = this->InitSerial();
}
inline void Uart::cfmakeraw(struct termios *t)
{
    t->c_iflag &= ~(IMAXBEL|IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    t->c_oflag &= ~OPOST;
    t->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    t->c_cflag &= ~(CSIZE|PARENB);
    t->c_cflag |= CS8;
}
int Uart::InitSerial()
{
	int u = -1;
	u = open("/dev/ttyAMA0",O_RDWR | O_NOCTTY | O_NDELAY);
	if (u == -1){
		throw std::runtime_error("uart.cpp:Error open UART port.\n");
		return -1;
	}
	struct termios options;
	cfmakeraw(&options); //row mode
	cfsetispeed(&options, B1000000);
	cfsetospeed(&options, B1000000);
	options.c_cflag |=  (CLOCAL | CREAD);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_iflag |= IGNPAR;
	options.c_cflag &= ~CSTOPB; //one stop bit
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(u, TCIFLUSH);
	tcsetattr(u, TCSANOW, &options);
	struct serial_struct ss;
	ioctl(u, TIOCGSERIAL, &ss);
	float BAUDRATE = 1000000;
	ss.flags = (ss.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST;
	ss.custom_divisor = ss.baud_base / BAUDRATE;
//	printf("ss.baud_base:%d\nss.custom_divisor:%d\n",ss.baud_base,ss.custom_divisor);
	ioctl(u, TIOCSSERIAL, &ss);
	return u;
}
Uart::~Uart()
{
}
void Uart::do_uart_cycle()
{
	for (std::vector<wbchars*>::iterator it=wbuffer.begin();it != wbuffer.end();){
		wbchars* tmp = *it;
		char todisplay[50];
		char * p_display=todisplay;
		memset(p_display,0,50*sizeof(char));
		ByteToHexStr((const unsigned char*)tmp->buffer,p_display,tmp->length);
		std::cout<<p_display<<std::endl;
		for (int j=0; j < tmp->length; j++){
			write(*fd,tmp->buffer+j,1);
			tcflush(*fd,TCOFLUSH);
		}
		//clean buffer
		delete tmp->buffer;
		delete tmp;
		it = wbuffer.erase(it);
		usleep(50);
	}
}
/*void Uart::change_mode()
{
	if(io == false)
		io = true;
	else
		io = false;
}*/
int Uart::put_in(char *content,int leng)
{
	wbchars *tmp = new wbchars;
	tmp->buffer = new char[leng];
	memcpy(tmp->buffer,content,leng);
	tmp -> length = leng;
	wbuffer.push_back(tmp);
	return 0;
}
/*char* Uart::read_from(std::string *buffer)
{	
	int length=buffer->length();
	if(length != 0){
		char *tmp = new char[buffer->size() + 1];
		std::copy(buffer->begin(),buffer->end(),tmp);
		tmp[buffer->size()] = '\0';
		return tmp;
	}
	return nullptr;
}
void Uart::clean_buffer(std::string *buffer)
{
	*buffer = "";
}
*/
void Uart::ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)  
{
	short i;
	unsigned char highByte, lowByte;
	for (i = 0; i < sourceLen; i++){
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;
		highByte += 0x30;
		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;
		lowByte += 0x30;
		if (lowByte > 0x39)
			 dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	} 
	return ;  
}
