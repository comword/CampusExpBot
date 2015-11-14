/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "bot.h"
#include "motor.h"
bot *b;
bot::bot()
{
}
bot::~bot()
{
}
int bot::self_check()
{
	return 0;//no problem
}
void bot::go()
{
	switch (b_s) {
	case BOT_SELFCHECK:
		m->run("1");
		m->run("2");
		m->run("3");
		m->run("4");
		return;
	case BOT_SYNC:
		return;
	case BOT_ONROAD:
		return;
	case BOT_DELIVER:
		return;
	case BOT_BACK:
		return;
	case BOT_ERROR:
 		return;
	}; 
}
