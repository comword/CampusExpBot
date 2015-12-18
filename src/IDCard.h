/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "RFID.h"

class IDCard : public RFID
{
public:
	IDCard();
	virtual ~IDCard();
	static void *read_card_thread(void *ptr);
	void start_thread();
	struct Card_arg{
		unsigned char card_type;
		unsigned char id[6];
	};
	void card_succss_callback(Card_arg &aCard);
};
extern IDCard *ID;
