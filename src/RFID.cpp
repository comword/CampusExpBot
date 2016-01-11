/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "RFID.h"

#include <stdlib.h>
#include <vector>
#include <string.h>
#include <errno.h>
#include <stdexcept>
RFID::RFID()
{
	Py_Initialize();
	if (!Py_IsInitialized())
		throw std::runtime_error(std::string("RFID::assert !Py_IsInitialized()\n"));
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./utility/RC522/')");
	RC522Mod = PyImport_ImportModule("MFRC522");
	if (RC522Mod == nullptr)
		throw std::runtime_error(std::string("RFID::assert RC522Mod == nullptr\n"));
	PyRun_SimpleString("import RPi.GPIO as GPIO");
	PyRun_SimpleString("MIFAREReader = MFRC522.MFRC522()");
	read_once = PyObject_GetAttrString(RC522Mod, "read_once");
	if (read_once == nullptr)
		throw std::runtime_error(std::string("RFID::assert read_once == nullptr\n"));
}
RFID::~RFID()
{
	Py_DECREF(read_once);
	Py_DECREF(RC522Mod);
	Py_Finalize();
}
std::vector<int> RFID::read_card()
{
	using namespace std;
	vector<int> vec;
	PyObject* pResult = nullptr;
	pResult = PyEval_CallObject(read_once,nullptr);
	return vec;
}
