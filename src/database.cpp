/* Copyright 2015 Jiajie Jing,Henorvell Ge 
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "database.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <ctime>
#include <stdlib.h>
sqlite_helper::sqlite_helper(std::string db_path)
{
	std::string exp;
	try{
		this->Db=new SQLite::Database(db_path, SQLITE_OPEN_READWRITE);
	}
	catch (std::exception& e){
		exp = std::string("SQLite exception: ")+ e.what();
		throw std::runtime_error(exp);
		return;
	}
}
sqlite_helper::~sqlite_helper()
{
}
int sqlite_helper::db_add(DB_Row &row)
{
	std::string time,deliver;
	std::stringstream s_time(time),s_deliver(deliver);
	s_time<<row.times_of_delivery;
	s_deliver<<row.if_delivered;
	this->Db->exec("INSERT INTO Items (Number,Express,Owner,Tel,Entry_Date,Out_date,Times_Of_Delivery,If_Delivered) VALUES (\'"+row.number+"\',\'"+row.express+"\',\'"+row.owner+"\',\'"+row.tel+"\',\'"+row.entry_date+"\',\'"+row.out_date+"\',\'"+s_time.str()+"\',\'"+s_deliver.str()+"\')");
	return 0;
}
int sqlite_helper::db_del_by_num(std::string number)
{
	this->Db->exec("DELETE FROM Items WHERE Number=\'"+number+"\'");
	return 0;
}

int sqlite_helper::db_mark_out(std::string number)
{
	time_t tt = time(NULL);
	tm *t = localtime(&tt);
	int year = t->tm_year + 1900;
	int month = t->tm_mon + 1;
	int day = t->tm_mday;
	std::string out_date;
	std::stringstream s_date(out_date);
	s_date<<year<<"/"<<month<<"/"<<day;
	this->Db->exec("UPDATE Items SET Out_Date = \'"+s_date.str()+"\',Times_Of_Delivery = Times_Of_Delivery+1 WHERE Number=\'"+number+"\'");
	return 0;
}

int sqlite_helper::db_mark_delivered(std::string number)
{
	this->Db->exec("UPDATE Items SET If_Delivered = '1' WHERE Id = \'"+number+"\'");	
	return 0;
}

int sqlite_helper::db_search_by_number(std::string number)
{
	std::string sql="SELECT * FROM Items WHERE Number=\'"+number+"\'";
	this->Query = new SQLite::Statement(*(this->Db),sql);
//	this->Query->bind(1,6);
	while(Query->executeStep())
	{
		DB_Row row;
		row.number = std::string((const char*)this->Query->getColumn(1));
		row.express = std::string((const char*)this->Query->getColumn(2));
		row.owner = std::string((const char*)this->Query->getColumn(3));
		row.tel = std::string((const char*)this->Query->getColumn(4));
		row.entry_date = std::string((const char*)this->Query->getColumn(5));
		row.out_date = std::string((const char*)this->Query->getColumn(6));
		row.times_of_delivery = atoi(this->Query->getColumn(7));
	        row.if_delivered = atoi(this->Query->getColumn(8));	
		std::cout<<row.owner<<std::endl;
	}
	return 0;
}

int sqlite_helper::db_search_by_owner(std::string owner)
{
	std::string sql="SELECT * FROM Items WHERE Owner=\'"+owner+"\'";
	this->Query = new SQLite::Statement(*(this->Db),sql);
//	this->Query->bind(1,6);
	while(Query->executeStep())
	{
		DB_Row row;
		row.number = std::string((const char*)this->Query->getColumn(1));
		row.express = std::string((const char*)this->Query->getColumn(2));
		row.owner = std::string((const char*)this->Query->getColumn(3));
		row.tel = std::string((const char*)this->Query->getColumn(4));
		row.entry_date = std::string((const char*)this->Query->getColumn(5));
		row.out_date = std::string((const char*)this->Query->getColumn(6));
		row.times_of_delivery = atoi(this->Query->getColumn(7));
	        row.if_delivered = atoi(this->Query->getColumn(8));	
		std::cout<<row.owner<<std::endl;
	}
	return 0;
}

/*int main() // test
{
	using namespace std;

        cout<<"1.Insert an item  2.Delete an item by number 3.Search items by number 4.Mark an item out 5.Mark an item delivered\n";
	int i=0;
	cin>>i;
	switch(i)
	{
		case 1:
	 {	sqlite_helper sqlite(db_path);
		DB_Row row;
		cout<<"number:\n";
		cin>>row.number;
		cout<<"express:\n";
		cin>>row.express;
		cout<<"owner:\n";
		cin>>row.owner;
		cout<<"tel:\n";
		cin>>row.tel;
		cout<<"entry date:\n";
		cin>>row.entry_date;
		row.out_date="0";
		row.times_of_delivery=0;
		row.if_delivered=0;
		sqlite.db_add(row);
		break;
	 } 
		case 2:
	 {
		cout<<"输入要删除的快递单号";
		string num;
		cin>>num;
		sqlite_helper sqlite(db_path);
	        sqlite.db_del_by_num(num);
		break;
	 }
	 	case 3:
	 {
		cout<<"输入要搜索的快递单号";
	 	string num;
		cin>>num;
		sqlite_helper sqlite(db_path);
		sqlite.db_search_by_number(num);
		break;
	 }
	 	case 4:
	 {
		cout<<"输入要标记为送出的快递单号";
		string num;
		cin>>num;
		sqlite_helper sqlite(db_path);
		sqlite.db_mark_out(num);
		break;
	 }
	 	case 5:
	 {
		cout<<"输入要标记为成功送达的快递单号";
		string num;
		cin>>num;
		sqlite_helper sqlite(db_path);
		sqlite.db_mark_delivered(num);
		break;
	 }
	}
	return 0;
}
*/
