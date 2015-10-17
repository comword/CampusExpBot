/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <string>
struct MotorsDef{
	std::string id;
	std::string protocol;
	int gpio;
};
struct SensorsDef{
	std::string id;
	std::string protocol;
	int gpio;
};
