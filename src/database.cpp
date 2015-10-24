/* Copyright 2015 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "database.h"

#include <sqlite3.h>
#include <string>
#include <stdexcept>
sqlite_helper::sqlite_helper(const char *db_path)
{
	std::string exp;
	int res = sqlite3_open(db_path,&(this->DB));
	if ( res ){
		exp = std::string("database.cpp:Can't open database:") + sqlite3_errmsg(this->DB);
		sqlite3_close(this -> DB);
		throw std::runtime_error(exp);
		return;
	}
}
sqlite_helper::~sqlite_helper(){}
//sqlite3 * DB;
//char* errorMsg;
