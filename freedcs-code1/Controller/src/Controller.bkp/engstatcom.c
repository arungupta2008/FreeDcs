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
#include "../../include/Controller/log_linux.h" 
#include "../../include/Controller/log_rt.h"
#include "../../include/Controller/engstatcom.h"
#include <sys/stat.h>

int main(void);
int validate_engstation(SSL *ssl);

int main(void)
{
	char module_name[] = "engstatcom";
	/*openlog("engstatcom", 0, LOG_USER);

	shmbuf = GetSharedMemory(1000, module_name); //size=1000
    assertsyslog(shmbuf);

	memset(shmbuf,0,1000);
	cpheader = (struct cprog_header*) shmbuf;
	cpheader->debug_level = 0;*/

	statcom_main(ENGSTATION_SERVER_PORT, MAX_ENGSTATION_CLIENTS, module_name, &handle_engstation_commads);

	return 0; 	/*we should never reach here !*/
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
int handle_engstation_commads(SSL *ssl, int connection_id)
{
	char task_name[16];
	char syslogstring[100];
	uint32_t command=0;

	char answer[100];

	printf("at first in the handle function\n");

    assertsyslog(connection_id < 9999999);
	sprintf(task_name, "ENGSTCOM%d", connection_id);
	sprintf(syslogstring, "taskname = %s\n", task_name);
	log_message(LOG_INFO, "%s", syslogstring);

	makerealtime(task_name);

    printf("after some code in the handle function\n");

	/*while(!exit)
	{*/
        if(ssl)
        {
             SSL_read(ssl, &command, sizeof(command));
             sprintf(answer, "The client sent the command=%d\n", command);
        }
        else
            printf("SSL variable is NULL\n");

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



