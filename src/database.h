/* Copyright 2015 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "SQLiteCpp/SQLiteCpp.h" 

#include <string>
class sqlite_helper
{
	public:
		sqlite_helper(std::string db_path);
		virtual ~sqlite_helper();
	protected:
		SQLite::Database *Db;
};
