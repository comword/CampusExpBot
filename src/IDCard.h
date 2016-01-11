/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "RFID.h"

#include <map>
#include <functional>

typedef std::function<int(int*)> card_callback;
using CardsMap = std::map<int*,card_callback>;
class IDCard : public RFID
{
public:
	IDCard();
	virtual ~IDCard();
	void read_card_thread();
	void card_succss_callback(int* aCard);
	void registry_card(int* aCard, card_callback func);
	void unregistry_card(int* aCard);
	void check_CM(int* aCard);
protected:
	CardsMap *CM;
	int *last_card;
};
extern IDCard *ID;
