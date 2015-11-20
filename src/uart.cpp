/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "uart.h"
#include "xmlhandler.h"

#include <fstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdexcept>
Uart::Uart(const char* wiringPi_path)
{
	wbuffer = new std::string();
	std::string exp;
	std::ifstream fin(wiringPi_path);
	if (!fin){
		exp="uart.cpp:Uart INIT Failed: No such file or directory.";
		throw std::runtime_error(exp);
		return;
	}
	wiringPi_handle=dlopen(wiringPi_path, RTLD_LAZY);
	if (wiringPi_handle == nullptr){
		exp=std::string("uart.cpp:dlopen failed:")+dlerror();
		throw std::runtime_error(exp);
		return;
	}
	dlerror();
	serialOpen = (int(*)(char*,int))dlsym(wiringPi_handle, "serialOpen"); 
	serialClose = (int(*)(int))dlsym(wiringPi_handle, "serialClose"); 
	wiringPiSetup = (int(*)())dlsym(wiringPi_handle, "wiringPiSetup"); 
	serialDataAvail = (int(*)(int))dlsym(wiringPi_handle, "serialDataAvail"); 
	serialPrintf = (void(*)(int,char*,...))dlsym(wiringPi_handle, "serialPrintf"); 
	if((this -> port = serialOpen ("/dev/ttyAMA0", 115200)) < 0){
		exp=std::string("uart.cpp:Unable to open serial device:")+strerror(errno);
		dlclose(wiringPi_handle);
		throw std::runtime_error(exp);
		return;
	}
	if(wiringPiSetup () == -1){
		exp=std::string("uart.cpp:Unable to start wiringPi:")+strerror(errno);
		serialClose(this->port);
		dlclose(wiringPi_handle);
		throw std::runtime_error(exp);
		return;
	}
}
Uart::~Uart()
{
	serialClose(this->port);
	dlclose(wiringPi_handle);
	delete wbuffer;
}
void Uart::do_uart_cycle()
{
	//TODO:just ignore servo reply
	//if (io){//true for in, false for out
	//in
	//int ava = serialDataAvail(this->port);
	//}
	//else{
	//out
	int length=wbuffer->length();
	if(length != 0){
		char *tmp = new char[wbuffer->size() + 1];
		std::copy(wbuffer->begin(),wbuffer->end(),tmp);
		tmp[wbuffer->size()] = '\0';
		serialPrintf(port,tmp);
		delete[] tmp;
		*wbuffer="";
	}
	//}
}
void Uart::change_mode()
{
	if(io == false)
		io = true;
	else
		io = false;
}
int Uart::put_in(std::string *buffer,const char* content)
{
	buffer -> append(content);	
	return 0;
}
char* Uart::read_from(std::string *buffer)
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
