/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "IDCard.h"
#include "database.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>
#include <unistd.h>
#include <iostream>

IDCard::IDCard()
{
	CM = new CardsMap();
}
IDCard::~IDCard()
{
	delete CM;
}

void IDCard::read_card_thread()
{
	int* Cardid;
	Cardid = this->read_card();
	if (Cardid != nullptr){
		if (last_card == nullptr){
			this->card_succss_callback(Cardid);
			last_card = Cardid;
			return;
		}
		if (*last_card != *Cardid && *(last_card+1) != *(Cardid+1) && *(last_card+2) != *(Cardid+2) && *(last_card+3) != *(Cardid+3) && *(last_card+4) != *(Cardid+4))
			this->card_succss_callback(Cardid);
		last_card = Cardid;
	}
}

void IDCard::card_succss_callback(int* aCard)
{
	std::cout<<"CARD: ";
	for (int i=0; i<5; i++){
		std::cout<<*(aCard+i)<<" ";
	}
	std::cout<<std::endl;
	check_CM(aCard);
	free(aCard);
}
void IDCard::registry_card(int* aCard, card_callback func)
{
	CM->insert(std::pair<int*,card_callback>(aCard,func));
}
void IDCard::unregistry_card(int* aCard)
{
	for (CardsMap::iterator i=CM->begin(); i!=CM->end();){
		if (*i->first == *aCard && *(i->first+1) == *(aCard+1) && *(i->first+2) == *(aCard+2) && *(i->first+3) == *(aCard+3) && *(i->first+4) == *(aCard+4)){
			delete i->first;
			CM->erase(i++);
		}
	}
}
void IDCard::check_CM(int* aCard)
{
	for (CardsMap::iterator i=CM->begin(); i!=CM->end();i++){
		if (*i->first == *aCard && *(i->first+1) == *(aCard+1) && *(i->first+2) == *(aCard+2) && *(i->first+3) == *(aCard+3) && *(i->first+4) == *(aCard+4)){
			i->second(aCard);
		}
	}
}