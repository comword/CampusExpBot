#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#define UART "/dev/ttyAMA0"
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
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	options.c_cflag |=  (CLOCAL | CREAD);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_iflag |= IGNPAR;
	options.c_cflag &= ~CSTOPB; //one stop bit
	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_ispeed=1000000;
	options.c_ospeed=1000000;
	tcflush(u, TCIFLUSH);
	tcsetattr(u, TCSANOW, &options);
	return u;
}
int UART_Send(int fd, char *send_buf,int data_len)
{
	for (int i = 0; i < data_len; i++){
		int res;
		res = write(fd,(send_buf+i),1);
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
	int input;
	scanf("%d",&input);
	if (input < 253){
		input &= 0xff;
		unsigned char *inputID = (char *)&input;
		char *buffer = (char*)malloc(4*sizeof(char));
		*buffer = 0xff;
		*(buffer+1) = 0xff;
		*(buffer+2) = 0xfe;//broadcast
		*(buffer+3) = 0x04;
		UART_Send(u,buffer,sizeof(buffer));
		*(buffer) = 0x03;//write data
		*(buffer+1) = 0x03;//write data
		*(buffer+2) = *inputID;
		int sum;
		sum = 0xfe;
		sum += 0x04;
		sum += 0x03;
		sum += 0x03+*inputID;
		sum = sum&0xff;
		sum = ~sum;
		char *chsum = (char*)malloc(sizeof(char));
		memcpy(chsum,&sum,sizeof(char));
		printf("%d\n",*chsum);
		*(buffer+3) = *chsum;//checksum
		printf("Sending:\n %s \n",buffer);
		UART_Send(u,buffer,sizeof(buffer));
		free(buffer);
	} else {
		fflush(stdin);
		fprintf(stderr,"Error:inputID cannot large than 253.\n");
	}
	close(u);
}
