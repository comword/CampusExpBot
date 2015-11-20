#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#define UART "/dev/ttyAMA0"
#define BAUDRATE B115200
void cfmakeraw(struct termios *t)
{
	t->c_iflag &= ~(IMAXBEL|IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	t->c_oflag &= ~OPOST;
	t->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	t->c_cflag &= ~(CSIZE|PARENB);
	t->c_cflag |= CS8;
}
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
	tcgetattr(u, &options);
	cfmakeraw(&options); //row mode
	cfsetispeed(&options, BAUDRATE);
	cfsetospeed(&options, BAUDRATE);
	options.c_cflag |=  (CLOCAL | CREAD);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_iflag |= IGNPAR;
	options.c_cflag &= ~CSTOPB; //one stop bit
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(u, TCIFLUSH);
	tcsetattr(u, TCSANOW, &options);
	return u;
}
int UART_Send(int fd, char *send_buf,int data_len)
{
	for(int i=0;i<data_len;i++){
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
	int input;
	scanf("%d",&input);
	if (input < 253){
		input &= 0xff;
		unsigned char *inputID = (char *)&input;
		char *buffer = (char*)malloc(11*sizeof(char));
		*buffer = 0xff;
		*(buffer+1)=0xff;
		*(buffer+2)=*inputID;
		*(buffer+3)=0x07;
		*(buffer+4)=0x03;
		*(buffer+5)=0x1e;
		*(buffer+6)=0x00;
		*(buffer+7)=0x02;
		*(buffer+8)=0x00;
		*(buffer+9)=0x02;
		int sum;
		sum = *inputID;
		sum += 0x07;
		sum += 0x03+0x1e;
		sum += 0x04;
		sum = sum&0xff;
		sum = ~sum;
		char *chsum = (char*)malloc(sizeof(char));
		memcpy(chsum,&sum,sizeof(char));
		//printf("%d",*chsum);
		*(buffer+10) = *chsum;//checksum
		char todisplay[50];
		char * p_display=todisplay;
		memset(p_display,0,50*sizeof(char));
		ByteToHexStr(buffer,p_display,11*sizeof(char));
		printf("Sending:\n %s \n",p_display);
		UART_Send(u,buffer,11*sizeof(char));
		free(buffer);
		free(chsum);
	} else {
		fflush(stdin);
		fprintf(stderr,"Error:inputID cannot large than 253.\n");
	}
	close(u);
}
