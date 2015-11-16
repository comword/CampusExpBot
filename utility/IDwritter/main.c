#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
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
int UART_Send(int fd, char *send_buf,int data_len)
{
	for(int i=0;i<data_len;i++){
		int res;
		res = write(fd,(send_buf+i),data_len);
		if (data_len == res ){    
			continue;
		} else {
			tcflush(fd,TCOFLUSH);
			return -1;
		}
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
	int input;
	scanf("%d",&input);
	if (input < 253){
		input &= 0xff;
		unsigned char *inputID = (char *)&input;
		char *buffer = (char*)malloc(8*sizeof(char));
		*buffer = 0xff;
		*(buffer+1) = 0xff;
		*(buffer+2) = 0xfe;//broadcast
		*(buffer+3) = 0x04;
		*(buffer+4) = 0x03;//write data
		*(buffer+5) = 0x03;//write data
		*(buffer+6) = *inputID;
		int sum;
		sum = 0xfe;
		sum += 0x04;
		sum += 0x03;
		sum += 0x03+*inputID;
		sum = sum&0xff;
		sum = ~sum;
		char *chsum = (char*)malloc(sizeof(char));
		memcpy(chsum,&sum,sizeof(char));
		*(buffer+7) = *chsum;//checksum
		printf("Sending:\n %s \n",buffer);
		if(UART_Send(u,buffer,sizeof(buffer))==sizeof(buffer))
			printf("OK!\n");
		free(buffer);
	} else {
		fflush(stdin);
		fprintf(stderr,"Error:inputID cannot large than 253.\n");
	}
	close(u);
}
