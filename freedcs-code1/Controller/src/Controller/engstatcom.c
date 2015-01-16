/**
 * Project: FreeDCS: A Free Distributed Control System under GPL license.
 * @author Robert Perez, Montevideo-Uruguay, Copyright (C) 2007-2013
 * @see The GNU Public License (GPL)
 *
 * This file contains the code that handles the engineering station communication with the controller.
 * It contains code to set/force/view variables in the controller, load control programs and start executing them,etc.
 * This is the main interface to the controller.
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.f
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/*! \file */
#include "../../include/Controller/engstatcom.h"
#include <sys/stat.h>

int main(void);
int validate_engstation(SSL *ssl);
//For forking the parent process 
static void skeleton_daemon();




int main(void)
{
	//For making engstatcom a daemon 
	skeleton_daemon();
	pid_t get_pid = getpid();
	char str[6]; // Max PID is 5 only
	sprintf(str, "%d", get_pid);
	//PID of daemon is written in /var/run/engstatcom.pid
	int pid_file = open("/var/run/engstatcom.pid", O_CREAT | O_RDWR, 0666);
	write(pid_file,str, strlen(str));
	int rc = flock(pid_file, LOCK_EX | LOCK_NB);
	if(rc) {
		if(EWOULDBLOCK == errno)
        printf("engstatcom already running"); // another instance is running
        exit(1);
	}
	else { 
		for(;;){
			char module_name[] = "engstatcom";
			//Rt_Logging init 
			rt_print_auto_init(1); // Please don't comment this 
			log_message(LG_INFO, "For Module %s logging working for Linux Logging ",module_name);
			rt_log_message(LG_INFO, "For Module %s logging working for Rt_logging",module_name);
			/*openlog("engstatcom", 0, LOG_USER);
		
			shmbuf = GetSharedMemory(1000, module_name); //size=1000
			assertsyslog(shmbuf);
		
			memset(shmbuf,0,1000);
			cpheader = (struct cprog_header*) shmbuf;
			cpheader->debug_level = 0;*/
			statcom_main(ENGSTATION_SERVER_PORT, MAX_ENGSTATION_CLIENTS, module_name, &handle_engstation_commads);
			}
			return 0; 	/*we should never reach here !*/
		}
}
/*
 * This function forks the engstatcom and parent pid is killed and child pid is retained
 * */
#define EXIT_FAILURE_  log_message(LG_MSG, "Error in forking of engstatcom"); exit(1);
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0 
static void skeleton_daemon(){
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0){
		EXIT_FAILURE_
        //exit(EXIT_FAILURE);
	}

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
	//Change Directory
    //If we cant find the directory we exit with failure.
    if((chdir("/home/freedcs")) < 0) { exit(EXIT_FAILURE); }

    /* Close all open file descriptors */
    //int x;
    //for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    //{
    //    close (x);
    //}
   
}

/** \brief This is a callback function called by statcom_main() once the SSL connection has been established.
           This function routes the commands based on the command number given in the first field of the packet.
 *
 * \param ssl SSL* pointer to SSL connection to send/receive data
 * \param ctx SSL_CTX* pointer to a SSL_CTX context, normally not used.
 * \param connection_id int Number of the connection being served, used for debugging when needed.
 * \return int This function returns when the client send the EXIT command. It could return errors through this number.
 *
 */
int handle_engstation_commads(SSL *ssl, char *task_name)
{
	uint32_t command=0;

	char answer[100];

	log_message(LG_INFO ,"at first in the handle function");
	makerealtime(task_name);

    log_message(LG_INFO ,"after some code in the handle function");

	/*while(!exit)
	{*/
        if(ssl)
        {
             SSL_read(ssl, &command, sizeof(command));
             sprintf(answer, "The client sent the command=%d", command);
        }
        else
            log_message(LG_INFO ,"SSL variable is NULL");

        SSL_write(ssl, answer, (int)strlen(answer));


 /*       switch(command)
        {
            case EXIT_STAT:
                exit = 1;
                break;

            case INSERT_CPROG:

            case REMOVE_CPROG:

                break;

            case INSERT_IOPROG:

            case REMOVE_IOPROG:

                break;

            case IS_CPROG_LOADED:

                break;

            case GET_CPROG_SHM:
                break;



            default:
                exit = 1;
        }
	}*/

	return -1;
}



