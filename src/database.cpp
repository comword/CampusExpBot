/* Copyright 2015 Jiajie Jing,Henorvell Ge 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "database.h"

#include <stdexcept>
#include <sstream>
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
int sqlite_helper::db_add(DB_Row &row)
{
	std::string time,deliver;
	std::stringstream s_time(time),s_deliver(deliver);
	s_time<<row.times_of_delivery;
	s_deliver<<row.if_delivered;
	this->Db->exec("INSERT INTO Items (Number,Express,Owner,Tel,Entry_Date,Out_date) VALUE(\'"+row.number+"\',\'"+row.owner+"\',\'"+row.tel+"\',\'"+row.entry_data+"\',\'"+row.out_data+"\',\'"+s_time.str()+"\',\'"+s_deliver.str()+"\')");
	return 0;
}
int sqlite_helper::db_del_by_num(std::string number)
{
	return 0;
}
