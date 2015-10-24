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
Uart::Uart(xml_helper &conf)
{
	std::string exp;
	std::ifstream fin(conf.wiringPi_so);
	if (!fin){
		exp="uart.cpp:Uart INIT Failed: No such file or directory.";
		throw std::runtime_error(exp);
		return;
	}
	wiringPi_handle=dlopen(conf.wiringPi_so, RTLD_LAZY);
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
}
void Uart::do_uart_cycle()
{
	if (io)//true for in, false for out{
		//in
		int ava = serialDataAvail(this->port);
	}
	else{
		//out
	}
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
	return 0;
}
char* Uart::read_from(std::string *buffer)
{
	return 0;
}
void Uart::clean_buffer(std::string *buffer)
{
	*buffer = "";
}

