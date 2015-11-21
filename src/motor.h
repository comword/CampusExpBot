/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "uart.h"

#include <map>
#include <string>
struct MotorsDef;
using MotorsMap = std::map<std::string*,MotorsDef*>;
class Motor : public Uart
{
public:
	Motor();
	int run(std::string id,int speed);
	int run(std::string id,int speed, int ms);
	int stop(std::string id);
	void do_uart_cycle();
	void set_Motor_mode(std::string id);
	void finish_checksum(char *buffer,size_t buf_size);
	~Motor();
protected:
	MotorsDef* find_motor_byid(std::string id);
private:
	MotorsMap motors_config;
	const int MOTOR_HEAD;
};
//only one motor control class
extern Motor *m;
