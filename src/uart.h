/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "wiringPi/wiringPi.h"
class xml_helper;
class Uart
{
public:
	int (*serialOpen) (char*,int);
	int (*serialClose) (int);
	int (*wiringPiSetup) ();
	Uart(xml_helper &conf);
	virtual ~Uart();
	void do_uart_cycle();
	void change_mode();
private:
	int port;
	bool serial_ok;
	void *wiringPi_handle;
	bool io = false;//true for in, false for out
	char ** rbuffer,wbuffer;
};
