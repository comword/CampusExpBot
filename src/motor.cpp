/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "motor.h"
Motor::Motor(const char* wiringPi_path,MotorsMap map) :
Uart(wiringPi_path),
motors_config(map),
MOTOR_HEAD((char*)0xffff)
{
}
Motor::~Motor()
{
}
int Motor::run(const char* id)
{
	return 0;
}
int Motor::stop(const char* id)
{
	return 0;
}
MotorsDef* Motor::find_motor_byid(const char *id)
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
