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
Motor::Motor() :
Uart(conf->get_wiringPi_so()),
motors_config(conf->get_motors_conf()),
MOTOR_HEAD((const char*)0xffff)
{
}
Motor::~Motor()
{
}
int Motor::run(std::string id)
{
	return 0;
}
int Motor::run(std::string id,int ms)
{
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
