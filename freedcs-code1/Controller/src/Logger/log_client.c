#include "../../include/Controller/log_linux.h"

int main( int argc, char *argv[] ){
	log_message(LG_MSG, "Testing linux logging");
	log_message(LG_MSG, "LG_MSG --> MESSAGE");
	log_message(LG_CRIT, "LG_CRIT --> CRITICAL");
	log_message(LG_ERR, "LG_ERR --> ERROR");
	log_message(LG_WARN, "LG_WARN --> WARNING");
	log_message(LG_NOTICE, "LG_NOTICE --> NOTICE");
	log_message(LG_INFO, "LG_INFO --> INFO");
	log_message(LG_DEBUG, "LG_DEBUG --> DEBUG");
	log_message(LG_MAX, "LG_MAX --> MAX");
	return 0;
	}