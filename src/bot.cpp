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
	IRSensor = new GPIOClass("17");
	IRSensor->export_gpio();
	IRSensor->setdir_gpio("in");
}
bot::~bot()
{
	IRSensor->unexport_gpio();
	delete IRSensor;
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
	int obs = 0;
	switch (b_s) {
	case BOT_SELFCHECK:
/*		m->run("1",1023,1000);
		m->run("2",1023,1000);
		m->run("3",1023,1000);
		m->run("4",1023,1000);
*/		ID->registry_card(blue,
		[](int*) -> int {
			m->run("lock",1023,200);
			return 0;
			}
		);
/*		m->set_Motor_mode("1");
		m->set_Motor_mode("2");
		m->set_Motor_mode("3");
		m->set_Motor_mode("4");
		m->set_Motor_mode("lock");
*/		m->run("1",600);
		m->run("2",600);
		m->run("3",600);
		m->run("4",600);
		b_s = BOT_ONROAD;
	break;
	case BOT_SYNC:
	break;
	case BOT_ONROAD:
		IRSensor->getval_gpio(obs);
		if (obs == 1){
			m->IfNotRun_Run("1");
			m->IfNotRun_Run("2");
			m->IfNotRun_Run("3");
			m->IfNotRun_Run("4");
			break;
		}
		else {
			m->IfRun_Stop("1");
			m->IfRun_Stop("2");
			m->IfRun_Stop("3");
			m->IfRun_Stop("4");
		}
	break;
	case BOT_DELIVER:
	break;
	case BOT_BACK:
	break;
	case BOT_ERROR:
 	break;
	}; 
}
