/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "Defines.h"

#include <map>
#include <signal.h>
#include <time.h>
#include <functional>
class timer_helper
{
public:
	timer_helper();
	virtual ~timer_helper();
	static void timer_callback( int sig, siginfo_t *si,void* uc );
	typedef std::function<int(void*)> callback_func;
	struct arg_callback{
		int timerid;
		timer_t *timerID;
		timer_helper *mclass;
	};
	using timetables = std::map<int,timer_t *>;
	using functables = std::map<timer_t *,callback_func>;
	int m_settimer(int timerID,callback_func func,int expireMS, int intervalMS );
	timetables tt;
	functables ft;
};
