/* Copyright 2015 Jiajie Jing,Henorvell Ge 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "SQLiteCpp/SQLiteCpp.h" 
#include "Defines.h"

#include <string>
class sqlite_helper
{
	public:
		sqlite_helper(std::string db_path);
		virtual ~sqlite_helper();
		int db_add(DB_Row &row);
		int db_del_by_num(std::string number);
	protected:
		SQLite::Database *Db;
};
