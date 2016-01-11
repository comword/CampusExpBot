/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "RFID.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>
RFID::RFID()
{
	Py_Initialize();
	if (!Py_IsInitialized())
		throw std::runtime_error(std::string("RFID::assert !Py_IsInitialized()\n"));
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./utility/RC522')");
	RC522Mod = PyImport_ImportModule("read");
	if (RC522Mod == nullptr)
		throw std::runtime_error(std::string("RFID::assert RC522Mod == nullptr\n"));
	read_once = PyObject_GetAttrString(RC522Mod, "read_once");
	if (read_once == nullptr)
		throw std::runtime_error(std::string("RFID::assert read_once == nullptr\n"));
}
RFID::~RFID()
{
	Py_DECREF(read_once);
	read_once = PyObject_GetAttrString(RC522Mod, "end_call");
	PyEval_CallObject(read_once,nullptr);
	Py_DECREF(read_once);
	Py_DECREF(RC522Mod);
	Py_Finalize();
}
int* RFID::read_card()
{
	int* CardID=(int*)malloc(5*sizeof(int));
	PyObject* pResult = nullptr;
	pResult = PyEval_CallObject(read_once,nullptr);
	if (pResult == nullptr)
		return nullptr;
	for (int i=0; i<5; i++){
		PyObject* ptmp = PyList_GetItem(pResult,i);
		PyArg_Parse(ptmp, "i", CardID+i);
	}
	if (CardID[0]==0 && CardID[1]==0 && CardID[2]==0 && CardID[3]==0 && CardID[4]==0)
		return nullptr;
	return CardID;
}
