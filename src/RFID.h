/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <python2.7/Python.h>
#include <vector>
class RFID
{
public:
	RFID();
	virtual ~RFID();
	std::vector<int> read_card();
protected:
	PyObject *RC522Mod = nullptr;
	PyObject *read_once = nullptr;
};
