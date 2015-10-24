/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <map>
struct MotorsDef;
using MotorsMap = std::map<std::string*,MotorsDef*>;
class Motor
{
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
