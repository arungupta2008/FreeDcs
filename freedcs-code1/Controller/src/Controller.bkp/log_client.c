#include "../../include/Controller/log_linux.h"

#define EXIT_FAILURE  log_message(LG_MSG, "Incorrect arguments logd not started"); exit(1);
int main( int argc, char *argv[] ){
	unsigned Sleep_time = SLEEP_TIME;
	const char *log_file="/log/Controller.log";
	int fd = open_log_file(log_file);
	if ( argc < 2 &&  argc != 0 ) /* argc should be 2 for correct execution */
    {
        EXIT_FAILURE;
    }else{
			// argv[0] should be --option always
			if(argv[0] != '--option'){
				EXIT_FAILURE;
				}
			// argv[1] always a size of the logfile
			set_log_size(argv[1]);
			
			if( argv[2] != NULL){
				Sleep_time = argv[2];
				}
			
		}
	for(;;){
		 sleep(Sleep_time);
		 log_message(LG_MSG, "Checking Log file Size");
		 monitor_logsize();
		}
	return 0;
	}