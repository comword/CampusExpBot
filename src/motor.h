/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "wiringPi/wiringPi.h"
#include "xmlhandler.h"

#include <vector>

class Motor
{
friend class xml_helper;
public:
	Motor();
	int run(const char* id);
	int stop(const char* id);
	int load_motor_conf(MotorsMap& map);
	~Motor();
protected:
	bool conf_loaded;
	MotorsDef* find_motor_byid(const char *id);
private:
	MotorsMap motors_config;
};
