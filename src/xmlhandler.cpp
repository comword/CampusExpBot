/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "xmlhandler.h"

#include <fstream>
#include <iostream>
#include <string>
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
		std::cerr<<exp<<std::endl;
		throw std::runtime_error(exp);
	}
	doc=new TiXmlDocument(file_path);
	if(!(doc->LoadFile() == true)){
		exp = "xmlhandler.cpp:xml_helper doc.LoadFile() Failed.";
		std::cerr<<exp<<std::endl;
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
	TiXmlElement *MotorsTree = docHandle.FirstChild("Motors").FirstChild("Motor").ToElement();
	while(MotorsTree){
		MotorsDef *def_motor=new MotorsDef();
		std::string *id = new std::string(MotorsTree->Attribute("name"));
		def_motor->id = *id;
		def_motor->protocol = MotorsTree->Attribute("protocol");
		def_motor->gpio = strtol(MotorsTree->Attribute("id"),NULL,10);
		motors_config->insert(std::pair<std::string*,MotorsDef*>(id,def_motor));
		MotorsTree=MotorsTree->NextSiblingElement();
	}
	TiXmlElement *SensorsTree = docHandle.FirstChild("Sensors").FirstChild("Sensor").ToElement();
	while(SensorsTree){
		SensorsDef *def_sensor=new SensorsDef();
		std::string *id = new std::string(SensorsTree->Attribute("id"));
		def_sensor->id = *id;
		def_sensor->protocol = SensorsTree->Attribute("protocol");
		def_sensor->gpio = strtol(SensorsTree->Attribute("gpio"),NULL,10);
		sensors_config->insert(std::pair<std::string*,SensorsDef*>(id,def_sensor));
		SensorsTree=SensorsTree->NextSiblingElement();
	}
	TiXmlElement *Get_so = docHandle.FirstChild("wiringPi").ToElement();
	this -> wiringPi_so = Get_so -> Attribute("so");
	return 0;
}
void xml_helper::unload_sys_conf()
{
	for (MotorsMap::iterator i=motors_config->begin(); i!=motors_config->end();){
		delete i->first;
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
	std::cout<<*i->first<<"   "<<(i->second)->gpio<<std::endl;
	}
}
