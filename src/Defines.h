/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#pragma once
#include <string>
enum bot_status : int {
	BOT_SELFCHECK = 0,
	BOT_SYNC,
	BOT_ONROAD,
	BOT_DELIVER,
	BOT_BACK,
	BOT_ERROR
};
struct MotorsDef{
	std::string name;
	std::string protocol;
	int id;
};
struct SensorsDef{
	std::string id;
	std::string protocol;
	int gpio;
};
struct DB_Row{
	std::string number;
	std::string express;
	std::string owner;
	std::string tel;
	std::string entry_date;
	std::string out_date;
	int times_of_delivery;
	int if_delivered;
};
enum MotorProtocol : int{
	MP_RAW = 0,
	MP_DEF1
};

