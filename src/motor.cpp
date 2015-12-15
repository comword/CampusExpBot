/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "motor.h"
#include "xmlhandler.h"
#include "timer.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>
#include <math.h>
Motor::Motor() :
Uart(conf->get_wiringPi_so()),
motors_config(conf->get_motors_conf()),
MOTOR_HEAD(0xffff)
{
}
Motor::~Motor()
{
}
int Motor::run(std::string id,int speed)
{
	//FF FF (ID) 04 03 20 (0000000000000000B) SM
	//                     | Speed  |D|
	//speed from -1023 to 1023
	if (abs(speed) > 1023)
		throw std::runtime_error(std::string("Motor::run speed value larger than 1023!\n"));
	int rid = find_motor_byid(id)->id;
	char *buf=(char *)malloc(9*sizeof(char));
	buf=(char *)memset(buf,0,9*sizeof(char));
	buf=(char *)memset(buf,MOTOR_HEAD,2*sizeof(char));
	rid &= 0xff;
	char *motorID = (char *)&rid;
	*(buf+2) = *motorID;
	*(buf+3) = 0x04;
	*(buf+4) = 0x03;
	*(buf+5) = 0x20;
	*(buf+6) = speed & 255;
	*(buf+7) = (speed > 0) ? 1024 + (speed & 256) : abs(speed) & 256;
	finish_checksum(buf,9);
	return 0;
}
int Motor::run(std::string id,int speed,int ms)
{
	this -> run (id);
	
	return 0;
}
int Motor::stop(std::string id)
{
	return 0;
}
MotorsDef* Motor::find_motor_byid(std::string id)
{
	for (MotorsMap::iterator i=this -> motors_config.begin(); i!=this -> motors_config.end();i++){
		if(*(i->first) == id )
			return i->second;
	}
	return (MotorsDef*)0;
}
void Motor::do_uart_cycle()
{
	Uart::do_uart_cycle();
}
void Motor::set_Motor_mode(std::string id)
{
	//FF FF (ID) 07 03 06 00 00 00 00 SM
	int rid = find_motor_byid(id)->id;
	char *buf=(char *)malloc(11*sizeof(char));
	buf=(char *)memset(buf,0,11*sizeof(char));
	buf=(char *)memset(buf,MOTOR_HEAD,2*sizeof(char));
	rid &= 0xff;
	char *motorID = (char *)&rid;
	*(buf+2) = *motorID;
	*(buf+3) = 0x07;
	*(buf+4) = 0x03;
	*(buf+5) = 0x06;
	*(buf+6) = 0x00;
	*(buf+7) = 0x00;
	*(buf+8) = 0x00;
	*(buf+9) = 0x00;
	finish_checksum(buf,11);
	this->put_in(wbuffer,buf);
	free(buf);
}
void Motor::finish_checksum(char *buffer,size_t buf_size)
{
	int sum = 0;
	for (int i = 2;i < buf_size - 1;i++)
		sum += *(buffer+i);
	sum = sum&0xff;
	sum = ~sum;
	char *p_sum = (char *)&sum;
	*(buffer+buf_size-1) = *p_sum;//checksum
}
