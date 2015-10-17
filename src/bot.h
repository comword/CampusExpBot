/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

class bot;
//only one bot entity.
extern bot *b;
enum bot_status {
	BOT_SELFCHECK = 0,
	BOT_SYNC,
	BOT_ONROAD,
	BOT_DELIVER,
	BOT_BACK,
	BOT_ERROR
};
class bot
{
	public:
		bot();
		~bot();
};
