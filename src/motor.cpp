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
Uart(),
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
	char *buf=(char *)malloc(12*sizeof(char));
	buf=(char *)memset(buf,0,12*sizeof(char));
	buf=(char *)memset(buf,MOTOR_HEAD,2*sizeof(char));
	rid &= 0xff;
	char *motorID = (char *)&rid;
	*(buf+2) = *motorID;
	*(buf+3) = 0x07;
	*(buf+4) = 0x03;
	*(buf+5) = 0x1E;
	*(buf+6) = 0xfe;
	*(buf+7) = 0xf3;
	*(buf+8) = speed & 255;
	*(buf+9) = (speed > 0) ? (1024 + (speed & 512)) >>9 : (abs(speed) & 512) >> 9;
	finish_checksum(buf,11);
	this->put_in(buf,11);
	free(buf);
	find_motor_byid(id)->running = true;
	return 0;
}
int Motor::run(std::string id,int speed,int delayms)
{
	this -> run(id,speed);
	timer_t *t_timerid = new timer_t;
	t->m_initTimer(t_timerid,[t_timerid,id](timer_t *Timer_ID,siginfo_t *,void *)-> int {
		if (t_timerid == Timer_ID){
			m->stop(id);
			t->m_delTimer(t_timerid);
			return 1;//Handled.
		}
		return 0;
	});
	t->m_setTimer(t_timerid,delayms);
	return 0;
}
int Motor::stop(std::string id)
{
	//FF FF (ID) 04 03 20 (0000000000000000B) SM
	//                     | Speed  |D|
	int rid = find_motor_byid(id)->id;
	rid &= 0xff;
	char *motorID = (char *)&rid;
	char *buf=(char *)malloc(10*sizeof(char));
	buf=(char *)memset(buf,0,10*sizeof(char));
	buf=(char *)memset(buf,MOTOR_HEAD,2*sizeof(char));
	*(buf+2) = *motorID;
	*(buf+3) = 0x05;
	*(buf+4) = 0x03;
	*(buf+5) = 0x20;
	*(buf+6) = 0;
	*(buf+7) = 0;
	finish_checksum(buf,9);
	this->put_in(buf,9);
	free(buf);
	find_motor_byid(id)->running = false;
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
	char *buf=(char *)malloc(12*sizeof(char));
	buf=(char *)memset(buf,0,12*sizeof(char));
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
	this->put_in(buf,11);
	free(buf);
}
void Motor::finish_checksum(char *buffer,size_t buf_size)
{
	int sum = 0;
	for (size_t i = 2;i < buf_size - 1;i++)
		sum += *(buffer+i);
	sum = sum&0xff;
	sum = ~sum;
	char *p_sum = (char *)&sum;
	*(buffer+buf_size-1) = *p_sum;//checksum
}
void Motor::IfNotRun_Run(std::string id)
{
	if (find_motor_byid(id)->running == false)
		m->run(id,1023);
}
void Motor::IfRun_Stop(std::string id)
{
	if (find_motor_byid(id)->running == true)
		m->stop(id);
}
