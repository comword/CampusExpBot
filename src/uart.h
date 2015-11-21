/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <string>
class xml_helper;
class Uart
{
public:
	int (*serialOpen) (char*,int);
	int (*serialClose) (int);
	int (*wiringPiSetup) ();
	int (*serialDataAvail) (int);
	int (*serialGetchar) (int);
	void (*serialPrintf) (int fd,char *message, ...);
	Uart(const char* wiringPi_path);
	virtual ~Uart();
	void do_uart_cycle();
	void change_mode();
	int put_in(std::string *buffer,const char* content);
	char* read_from(std::string *buffer);
	void clean_buffer(std::string *buffer);
	char get_sum_of_buffer(std::string *buffer);
	void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
	std::string *rbuffer,*wbuffer;
private:
	int port;
	bool serial_ok;
	void *wiringPi_handle;
	bool io = false;//true for in, false for out
};
