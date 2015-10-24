/* Copyright 2015 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <sqlite3.h>
class sqlite_helper
{
	public:
		sqlite_helper(const char *db_path);
		virtual ~sqlite_helper();
	protected:
		sqlite3 * DB;
		char* errorMsg;
};
