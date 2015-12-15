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
class timer_helper;
//only one timer_help entity.
extern timer_helper *t;
class timer_helper
{
public:
	timer_helper();
	virtual ~timer_helper();
	static void timer_callback( int sig, siginfo_t *si,void* uc );
	typedef std::function<int(timer_t *,siginfo_t *,void *)> callback_func;
	struct arg_callback{
		callback_func func;
		timer_t *timerID;
		timer_helper *mclass;
	};
//	using timetables = std::map<int,timer_t *>;
	using functables = std::map<timer_t *,arg_callback *>;
//	int m_settimer(int timerID,callback_func func,int expireMS, int intervalMS );
//	timetables tt;
	functables ft;
	void m_initTimer(timer_t *timerId, callback_func function);
	void m_setTimer(timer_t *timerId, int ExpireMSec);
	void m_setTimer(timer_t *timerId, int ExpireMSec, int IntervalMSec);
	void m_delTimer(timer_t *timerId);
	inline timer_helper::arg_callback* find_timerarg_byID(timer_t *timerId);
};
