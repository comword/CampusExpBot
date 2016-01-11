/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "bot.h"
#include "motor.h"
#include "IDCard.h"

bot *b;
bot::bot()
{
		blue = (int*)malloc(5*sizeof(int));
		blue[0]=162;
		blue[1]=188;
		blue[2]=136;
		blue[3]=111;
		blue[4]=249;
}
bot::~bot()
{
}
int bot::self_check()
{
	return 0;//no problem
}
void bot::setMode(bot_status status)
{
	b_s = status;
}
void bot::go()
{
	switch (b_s) {
	case BOT_SELFCHECK:
		m->run("1",1023,1000);
		m->run("2",1023,1000);
		m->run("3",1023,1000);
		m->run("4",1023,1000);
		ID->registry_card(blue,
		[](int*) -> int {
			m->run("1",1023,1000);
			return 0;
			}
		);
		b_s = BOT_SYNC;
		break;
	case BOT_SYNC:
		break;
	case BOT_ONROAD:
		break;
	case BOT_DELIVER:
		break;
	case BOT_BACK:
		break;
	case BOT_ERROR:
 		break;
	}; 
}
