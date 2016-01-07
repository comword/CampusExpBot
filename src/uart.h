/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <vector>
#include "Defines.h"
class xml_helper;
class Uart
{
public:
	Uart();
	virtual ~Uart();
	void cfmakeraw(struct termios *t);
	int InitSerial();
	void do_uart_cycle();
//	void change_mode();
	int put_in(char *content,int leng);
//	char* read_from(std::string *buffer);
//	void clean_buffer(std::string *buffer);
	void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
	std::vector<wbchars*> wbuffer;
private:
	int port;
//	bool io = false;//true for in, false for out
	int *fd;
};
