/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "xmlhandler.h"

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <stdexcept>
//xml_helper method
xml_helper::xml_helper(const char *file_path) :
motors_config(new MotorsMap()),
sensors_config(new SensorsMap())
{
	std::string exp;
	std::ifstream fin(file_path);
	if (!fin){
		exp = "xmlhandler.cpp:xml_helper INIT Failed: No such file or directory.";
		throw std::runtime_error(exp);
	}
	doc=new TiXmlDocument(file_path);
	if(!(doc->LoadFile() == true)){
		exp = "xmlhandler.cpp:xml_helper doc.LoadFile() Failed.";
		throw std::runtime_error(exp);
	}
}
xml_helper::~xml_helper()
{
	this->unload_sys_conf();
	delete doc;
}
int xml_helper::load_sys_config()
{
	TiXmlHandle docHandle(doc);
	TiXmlElement *Tree = docHandle.FirstChild("Motors").FirstChild("Motor").ToElement();
	while(Tree){
		MotorsDef *def=new MotorsDef();
		def->name = Tree->Attribute("name");
		def->protocol = Tree->Attribute("protocol");
		def->id = strtol(Tree->Attribute("id"),NULL,10);
		motors_config->insert(std::pair<std::string*,MotorsDef*>(&def->name,def));
		Tree=Tree->NextSiblingElement();
	}
	Tree = docHandle.FirstChild("Sensors").FirstChild("Sensor").ToElement();
	while(Tree){
		SensorsDef *def=new SensorsDef();
		def->id = Tree->Attribute("id");
		def->protocol = Tree->Attribute("protocol");
		def->gpio = strtol(Tree->Attribute("gpio"),NULL,10);
		sensors_config->insert(std::pair<std::string*,SensorsDef*>(&def->id,def));
		Tree=Tree->NextSiblingElement();
	}
	Tree = docHandle.FirstChild("wiringPi").ToElement();
	this -> wiringPi_so = Tree -> Attribute("so");
	Tree = docHandle.FirstChild("DataBase").ToElement();
	this -> database_path = Tree -> Attribute("path");
	return 0;
}
void xml_helper::unload_sys_conf()
{
	for (MotorsMap::iterator i=motors_config->begin(); i!=motors_config->end();){
		delete i->second;
		motors_config->erase(i++);
	}
	for (SensorsMap::iterator i=sensors_config->begin(); i!=sensors_config->end();){
		delete i->first;
		delete i->second;
		sensors_config->erase(i++);
	}
}
MotorsMap xml_helper::get_motors_conf()
{
	return *motors_config;
}
SensorsMap xml_helper::get_sensors_conf()
{
	return *sensors_config;
}
void xml_helper::test_print_MotorsMap(MotorsMap Map)
{
	for (MotorsMap::iterator i=Map.begin(); i!=Map.end();i++){
	std::cout<<*i->first<<"   "<<(i->second)->id<<std::endl;
	}
}
const char *xml_helper::get_database_path()
{
	return this -> database_path;
}
const char *xml_helper::get_wiringPi_so()
{
	return this -> wiringPi_so;
}
