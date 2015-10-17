/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <cstring>
#include <functional>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
void exit_processer(int n);
namespace {
 
struct arg_parameter {
  typedef std::function<int(int, const char **)> arg_callback;
 
  const char *flag;  //!< The commandline parameter to handle (e.g., "--seed").
  const char *param_documentation;  //!< Human readable description of this arguments parameter.
  arg_callback handler;  //!< The callback to be invoked when this argument is encountered.
};

//void printHelpMessage(const arg_parameter *argument, size_t num_arguments);
}

int main(int argc, char *argv[])
{
	bool exit = false;
	const char *config_path;
	const arg_parameter arg_proc[]={
		{"--help","Display help tp this program.",
		[](int,const char **) -> int {
			std::cout<<"Help:"<<std::endl;
			std::exit(0);
			return 0;
			}
		},
		{"--configure","Import program config file.",[&config_path](int num_args, const char **params) -> int {
				if(num_args < 1) return -1;
				config_path=params[0];
				return 1;
			}
		}
	};
	//process arguments
	const size_t num_arguments = sizeof(arg_proc) / sizeof (arg_proc[0]);
//	const char *program_name = argv[0];
//	int now_argc = --argc;
	argc--;
	argv++;//pass program_name
	while(argc)
	{
		bool handled = false;
		for (size_t i = 0; i < num_arguments; i++){
			auto &arg = arg_proc[i];
			if(!strcmp(argv[0], arg.flag)){
				argc--;
            	argv++;
				int used_parameter = arg.handler(argc, (const char **)argv);
				if (used_parameter < 0){
					std::cout<<"Failed reading parameter "<<*(argv - 1)<<std::endl;
					std::exit(1);
				}
				argc -= used_parameter;
				argv += used_parameter;
				handled = true;
				break;
			}
		}
		if(!handled){
			//just skip
			argc--;
			argv++;
		}
	}
	//Start INIT
	std::system("clear"); // Clear screen
	struct sigaction sigHandler;
    sigHandler.sa_handler = exit_processer;
    sigemptyset(&sigHandler.sa_mask);
    sigHandler.sa_flags = 0;
    sigaction(SIGINT, &sigHandler, NULL);
	//Main Loop
	do{
		
	} while (!exit);
	exit_processer(2);
	return 0;
}
void exit_processer(int n)
{
	if (n == 2){
		std::system("clear");
		int status = 0;
		std::exit(status);
	}
}
