#include "../../include/Controller/log_rt.h"

//#define EXIT_FAILURE  rt_log_message(LG_MSG, "Incorrect arguments logd not started"); exit(1);
int main(){
	//const char *log_file="/log/Controller.log";
	//rt_file = fopen (log_file,"a+");
	rt_print_auto_init(1);
	//rt_file = fopen (log_file,"a+");
	//printf("Get Address of varable %d\n",get_add_());
	//printf("Get rt_file : %d\n",get_rt_file());
	//set_rt_file(200);
	//printf("Get rt_file : %d\n",get_rt_file());
	//rt_printf("\nPrinting test = %d\n",test);
	
	if(open_rt_file(log_file_rt) == SUCCESS){
		rt_fprintf(rt_file,"\nLog printed Successfully\n");
		rt_printf("rt Log is working \n");
		rt_log_message(LG_MSG, "rt_logging working By Arun");
		}else{
		rt_printf("Failed To open Log file\n");
		}
	//if ( argc < 2 &&  argc != 0 ) /* argc should be 2 for correct execution */
    //{
    //    EXIT_FAILURE;
    //}else{
	//		// argv[0] should be --option always
	//		if(argv[0] != '--option'){
	//			EXIT_FAILURE;
	//			}
	//		// argv[1] always a size of the logfile
	//		set_log_size(argv[1]);
	//		
	//		if( argv[2] != NULL){
	//			Sleep_time = argv[2];
	//			}
	//		
	//	}
	//for(;;){
	//	 sleep(Sleep_time);
	//	 log_message(LG_MSG, "Checking Log file Size");
	//	 monitor_logsize();
	//	}
	return 0;
	}