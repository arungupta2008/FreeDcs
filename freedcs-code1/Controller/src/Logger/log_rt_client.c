#include "../../include/Controller/log_rt.h"
#include "../../include/Controller/log_linux.h" 
int main(){
	rt_print_auto_init(1);
	
	rt_printf("Working \n");
	log_message(LG_MSG, "Testing rt_logging");
	rt_log_message(LG_MSG, "Testing rt_logging");
	rt_log_message(LG_MSG, "LG_MSG --> MESSAGE");
	rt_log_message(LG_CRIT, "LG_CRIT --> CRITICAL");
	rt_log_message(LG_ERR, "LG_ERR --> ERROR");
	rt_log_message(LG_WARN, "LG_WARN --> WARNING");
	rt_log_message(LG_NOTICE, "LG_NOTICE --> NOTICE");
	rt_log_message(LG_INFO, "LG_INFO --> INFO");
	rt_log_message(LG_DEBUG, "LG_DEBUG --> DEBUG");
	rt_log_message(LG_MAX, "LG_MAX --> MAX");
	return 0;
	}