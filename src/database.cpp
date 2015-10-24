/* Copyright 2015 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "database.h"

#include <stdexcept>
sqlite_helper::sqlite_helper(std::string db_path)
{
	std::string exp;
	try{
		this->Db=new SQLite::Database(db_path);
	}
	catch (std::exception& e){
		exp = std::string("SQLite exception: ")+ e.what();
		throw std::runtime_error(exp);
		return;
	}
}
sqlite_helper::~sqlite_helper()
{
	delete(this->Db);
}
//sqlite3 * DB;
//char* errorMsg;
