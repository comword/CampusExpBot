/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "gpio.h"

#include <fstream>
#include <iostream>
#include <sstream>
GPIOClass::GPIOClass()
{
}
GPIOClass::GPIOClass(std::string gnum)
{
    this->gpionum = gnum;  //Instatiate GPIOClass object for GPIO pin number "gnum"
}

GPIOClass::~GPIOClass()
{
}
int GPIOClass::export_gpio()
{
	using namespace std;
	string export_str = "/sys/class/gpio/export";
	ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
	if (exportgpio < 0){
		cerr << " OPERATION FAILED: Unable to export GPIO"<< this->gpionum <<" ."<< endl;
		return -1;
	}
	exportgpio << this->gpionum ; //write GPIO number to export
	exportgpio.close(); //close export file
	return 0;
}

int GPIOClass::unexport_gpio()
{
	using namespace std;
	string unexport_str = "/sys/class/gpio/unexport";
	ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
	if (unexportgpio < 0){
		cerr << " OPERATION FAILED: Unable to unexport GPIO"<< this->gpionum <<" ."<< endl;
		return -1;
	}
	unexportgpio << this->gpionum ; //write GPIO number to unexport
	unexportgpio.close(); //close unexport file
	return 0;
}

int GPIOClass::setdir_gpio(std::string dir)
{
	using namespace std;
	string setdir_str ="/sys/class/gpio/gpio" + this->gpionum + "/direction";
	ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
	if (setdirgpio < 0){
		cerr<< " OPERATION FAILED: Unable to set direction of GPIO"<< this->gpionum <<" ."<< endl;
		return -1;
	}
	setdirgpio << dir; //write direction to direction file
	setdirgpio.close(); // close direction file
	return 0;
}

int GPIOClass::setval_gpio(std::string val)
{
	using namespace std;
	string setval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
	ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
	if (setvalgpio < 0){
		cerr<< " OPERATION FAILED: Unable to set the value of GPIO"<< this->gpionum <<" ."<< endl;
		return -1;
	}
	setvalgpio << val ;//write value to value file
	setvalgpio.close();// close value file
	return 0;
}

int GPIOClass::getval_gpio(std::string& val){
	using namespace std;
	string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
	ifstream getvalgpio(getval_str.c_str());// open value file for gpio
	if (getvalgpio < 0){
		cerr << " OPERATION FAILED: Unable to get value of GPIO"<< this->gpionum <<" ."<< endl;
		return -1;
	}
	getvalgpio >> val ;  //read gpio value
	if(val != "0")
		 val = "1";
	else
		val = "0";
	getvalgpio.close(); //close the value file
	return 0;
}

std::string GPIOClass::get_gpionum(){
	return this->gpionum;
}
