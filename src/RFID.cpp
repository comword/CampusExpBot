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
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./utility/RC522/')");
	RC522Mod = PyImport_ImportModule("MFRC522");
}
RFID::~RFID()
{
	Py_Finalize();
}
