/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "gpio.h"
#include "xmlhandler.h"

#include <vector>

using motors_info = std::vector<MotorsDef>;
class Motor : public GPIOClass
{
public:
	Motor();
	int run(const char& id);
	int stop(const char& id);
	~Motor();
protected:
	MotorProtocol motor_pro;
private:
	motors_info *infos;
};
