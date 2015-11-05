#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define UART "/dev/ttyAMA0"
#define BAUDRATE B115200
int InitSerial()
{
	int u = -1;
	/*
	* O_RDONLY - Open for reading only.
	* O_RDWR - Open for reading and writing.
	* O_WRONLY - Open for writing only.
	* O_NDELAY - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	* O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	*/
	u = open(UART,O_RDWR | O_NOCTTY | O_NDELAY);
	if (u == -1)
	{
		fprintf(stderr,"Error open UART port.\n");
		return -1;
	}
	struct termios options;
	tcgetattr(u, &options);
	cfmakeraw(&options); //row mode
	cfsetispeed(&options, BAUDRATE);
	cfsetospeed(&options, BAUDRATE);
	options.c_cflag |=  (CLOCAL | CREAD);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag &= ~PARENB;//no odd & even check bit
	options.c_iflag &= ~INPCK;
	options.c_cflag &= ~CSTOPB; //one stop bit
	tcflush(u, TCIFLUSH);
	tcsetattr(u, TCSANOW, &options);
	return u;
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
	close(u);
}
