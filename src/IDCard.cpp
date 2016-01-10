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
#include <pthread.h>
#include <unistd.h>
#include <iostream>

IDCard::IDCard()
{
	this->start_thread();
}
IDCard::~IDCard(){}

void * IDCard::read_card_thread(void *ptr)
{
/*	Card_arg aCard;
	IDCard *orgclass = (IDCard *) ptr;
	while(1){
		sleep(1);
		if (orgclass->findCard(0x52,&aCard.card_type)==MI_OK){
			if (orgclass->anticoll(aCard.id)==MI_OK){
					aCard.id[5]=0;
					orgclass->card_succss_callback(aCard);
			}
		}
	}*/
}

void IDCard::start_thread()
{
	pthread_attr_t attr;
	pthread_t tid;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	int res = pthread_create(&tid, NULL, IDCard::read_card_thread, this);
	if (res)
		throw std::runtime_error(std::string("IDCard::pthread_create()@start_thread() Failed!\n"));
	//pthread_join(tid, NULL);
}
void IDCard::card_succss_callback(Card_arg &aCard)
{
	std::cout<<"CARD: "+std::string((const char *)aCard.id)<<std::endl;
}
