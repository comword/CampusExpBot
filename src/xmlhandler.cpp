/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "xmlhandler.h"
#include "tinyxml.h"

#include <fstream>
#include <iostream>
//xml_helper method
xml_helper::xml_helper(const char *file_path)
{
	ifstream fin(file_path);
	if (!fin){
		std::cout<<"xmlhandler.cpp:xml_helper INIT Failed: No such file or directory."<<std::endl;
		return -1;
	}
	doc=new TiXmlDocument(file_path);
	if(!(doc->LoadFile() == true)){
		std::cout<<"xmlhandler.cpp:xml_helper doc.LoadFile() Failed."<<std::endl;
        return -1;
	}
	

}
xml_helper::~xml_helper()
{
	delete doc;
}
