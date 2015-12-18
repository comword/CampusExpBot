/* Copyright 2015 Jiajie Jing,Henorvell Ge 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#pragma once
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
		int db_mark_out(std::string number);
		int db_mark_delivered(std::string number);
		int db_search_by_number(std::string number);
		int db_search_by_owner(std::string owner);
	protected:
		SQLite::Database *Db;
		SQLite::Statement *Query;
};
extern sqlite_helper *d;
