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
#include <errno.h>
#include <stdexcept>
timer_helper::timer_helper(){}
timer_helper::~timer_helper(){}
void timer_helper::timer_callback(int sig, siginfo_t *si,void* uc)
{
	struct arg_callback *message;
	message = (arg_callback *)si->si_value.sival_ptr;
//	if ( sig == SIGALRM || sig == SIGUSR1 ){
	if ( sig == SIGUSR1 ){
		
	}
}
/*
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
*/
void timer_helper::m_initTimer(timer_t *timerId, callback_func function)
{
	struct sigevent sev;
	struct sigaction sa;
	struct arg_callback *uc=new arg_callback;
	uc->timerID = timerId;
	uc->func = function;
	uc->mclass = this;
//	sev.sigev_notify = SIGEV_THREAD;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timer_helper::timer_callback;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGUSR1;
	sev.sigev_value.sival_ptr = uc;
//	sev.sigev_notify_function = function;
	sev.sigev_notify_attributes = NULL;
	if (timer_create(CLOCK_REALTIME, &sev, timerId) == -1){
		throw std::runtime_error(std::string("ERROR:timer_create@timer_helper::m_initTimer!\n"));
	}
	this->ft.insert(std::pair<timer_t *,arg_callback *>(timerId,uc));
}
void timer_helper::m_setTimer(timer_t *timerId, int ExpireMSec)
{
	struct itimerspec its;
//	its.it_value.tv_sec = ExpireMSec / 1000;
	its.it_value.tv_sec = ExpireMSec;
//	its.it_value.tv_nsec = (ExpireMSec % 1000) * 1000000;
	its.it_value.tv_nsec = ExpireMSec * 1000000;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 0;
	if (timer_settime(*timerId, 0, &its, NULL) == -1){
		throw std::runtime_error(std::string("ERROR:timer_settime@timer_helper::m_setTimer!\n"));
	}
}
void timer_helper::m_setTimer(timer_t *timerId, int ExpireMSec, int IntervalMSec)
{
	struct itimerspec its;
	its.it_value.tv_sec = ExpireMSec;
	its.it_value.tv_nsec = ExpireMSec * 1000000;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = IntervalMSec * 1000000;
	if (timer_settime(*timerId, 0, &its, NULL) == -1){
		throw std::runtime_error(std::string("ERROR:timer_settime@timer_helper::m_setTimer!\n"));
	}
}
void timer_helper::m_delTimer(timer_t *timerId)
{
	timer_delete(timerId);
	
}
inline timer_helper::arg_callback* timer_helper::find_timerarg_byID(timer_t *timerId)
{
	for (functables::iterator i=this -> ft.begin(); i!=this -> ft.end();i++){
		if(i->first == timerId )
			return i->second;
	}
	throw std::runtime_error(std::string("ERROR:timer_helper::find_timerarg_byID!"));
	return (timer_helper::arg_callback *)0;
}
