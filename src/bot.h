/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "Defines.h"

class bot;
//only one bot entity.
extern bot *b;
class bot
{
public:
	bot();
	~bot();
	bot_status b_s = BOT_SELFCHECK;
	void setMode(bot_status status);
	int self_check();
	void go();
	int *blue;
private:
};
