/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "uart.h"

#include <map>
struct MotorsDef;
using MotorsMap = std::map<std::string*,MotorsDef*>;
class Motor : public Uart
{
public:
	Motor(const char* wiringPi_path,MotorsMap map);
	int run(const char* id);
	int stop(const char* id);
	void do_uart_cycle();
	~Motor();
protected:
	MotorsDef* find_motor_byid(const char *id);
private:
	MotorsMap motors_config;
	const char* MOTOR_HEAD;
};
