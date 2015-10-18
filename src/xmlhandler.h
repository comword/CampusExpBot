/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "Defines.h"
#include "tinyxml.h"

#include <map>
#include <memory>
struct MotorsDef;
struct SensorsDef;
typedef std::map<std::string*,MotorsDef*> MotorsMap;
typedef std::map<std::string*,SensorsDef*> SensorsMap;
class xml_helper
{
	public:
		// Initialization
		xml_helper(const char *file_path);
		~xml_helper();
		int load_sys_config();
		void unload_sys_conf();
		bool get_load_sus();
		MotorsMap get_motors_conf();
		SensorsMap get_sensors_conf();
		void test_print_MotorsMap(MotorsMap Map);
	private:
		TiXmlDocument *doc;
		std::unique_ptr<MotorsMap> motors_config;
		std::unique_ptr<SensorsMap> sensors_config;
		bool load_sus;
};
