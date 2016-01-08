#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#define UART "/dev/ttyAMA0"
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)  
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
void cfmakeraw(struct termios *t)
{
	t->c_iflag &= ~(IMAXBEL|IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	t->c_oflag &= ~OPOST;
	t->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	t->c_cflag &= ~(CSIZE|PARENB);
	t->c_cflag |= CS8;
}
int InitSerial()
{
	int u = -1;
	u = open(UART,O_RDWR | O_NOCTTY | O_NDELAY);
	if (u == -1)
	{
		fprintf(stderr,"Error open UART port.\n");
		return -1;
	}
	struct termios options;
	cfmakeraw(&options); //row mode
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
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
	float BAUDRATE = 115200;
	ss.flags = (ss.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST;
	ss.custom_divisor = ss.baud_base / BAUDRATE;
	printf("ss.baud_base:%d\nss.custom_divisor:%d\n",ss.baud_base,ss.custom_divisor);
	ioctl(u, TIOCSSERIAL, &ss);
	return u;
}
int UART_Send(int fd, char *send_buf,int data_len)
{
	for (int i = 0; i < data_len; i++){
		write(fd,send_buf+i,1);
		tcflush(fd,TCOFLUSH);
	}
}
int main()
{
	int u;
	u = InitSerial();
	if( u == -1 )
	{
		fprintf(stderr,"InitSerial Error!\n");
		return -1;
	}
	char *buffer = (char*)malloc(8*sizeof(char));
	*buffer = 0xff;
	*(buffer+1) = 0xff;
	*(buffer+2) = 0xfe;//broadcast
	*(buffer+3) = 0x04;
	*(buffer+4) = 0x03;//write data
	*(buffer+5) = 0x04;
	*(buffer+6) = 0x01;
	int sum;
	sum = 0xfe;
	sum += 0x04;
	sum += 0x03;
	sum += 0x04+1;
	sum = sum&0xff;
	sum = ~sum;
	char *chsum = (char*)malloc(sizeof(char));
	memcpy(chsum,&sum,sizeof(char));
	*(buffer+7) = *chsum;//checksum
	char todisplay[50];
	char * p_display=todisplay;
	memset(p_display,0,50*sizeof(char));
	ByteToHexStr(buffer,p_display,8*sizeof(char));
	printf("Sending:\n %s \n",p_display);
	UART_Send(u,buffer,8*sizeof(char));
	free(buffer);
	close(u);
}
