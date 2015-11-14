/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "timer.h"

#include <unistd.h>
#include <string>
#include <stdlib.h>
timer_helper::timer_helper(){}
timer_helper::~timer_helper(){}
void timer_helper::timer_callback( int sig, siginfo_t *si,void* uc)
{
	struct arg_callback *message;
	message = (arg_callback *)si->si_value.sival_ptr;
	if (sig == SIGALRM){
		
	}
}
int timer_helper::m_settimer(int timerID,callback_func func,int expireMS, int intervalMS)
{
	int res = -1;
	timer_t *t_timerid = new timer_t;
    struct sigevent evp;
	struct itimerspec its;
	struct sigaction sa;
	struct arg_callback *uc=new arg_callback;
	uc->timerid=timerID;
	uc->timerID=t_timerid;
	uc->mclass=this;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timer_helper::timer_callback;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	evp.sigev_value.sival_ptr = uc;
	evp.sigev_notify = SIGEV_SIGNAL;
	evp.sigev_signo = SIGUSR1;
	res = timer_create(CLOCK_REALTIME, &evp, t_timerid);
	this->tt.insert(std::pair<int,timer_t *>(timerID,t_timerid));
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = intervalMS * 1000000;
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = expireMS * 1000000;
	timer_settime(*t_timerid, 0, &its, NULL);
	return res;
}
