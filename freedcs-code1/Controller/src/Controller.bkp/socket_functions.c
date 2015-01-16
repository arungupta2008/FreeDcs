/**
 * Project: FreeDCS: A Free Distributed Control System under GPL license.
 * @author Robert Perez, Montevideo-Uruguay, Copyright (C) 2007-2013
 * @see The GNU Public License (GPL)
 *
 * This file contains the code that handles the ethernet communication to/from the controller via sockets.
 * It contains functions to send/receive individual data types or complex structures used for commands.
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "../../include/Controller/socket_functions.h"


/*send data int through socket
do error handling also.
returns -1 on failure.*/
int send_data(SSL *ssl, void* data, uint32_t data_size)
{
	int total_sent=0, ret;
	char *buffer = (char*) data;

	while( (uint32_t)total_sent < data_size)
	{
        if((ret = SSL_write(ssl,(void*) &buffer[total_sent], (int)data_size - total_sent)) > 0)
            total_sent += ret;
        else
        {
            mysyslog(LOG_ERR, "Error in SSL send function\n");
            return -1;
        }
	}

	return 0;
}

/*receive data int through socket
do error handling also.
returns -1 on failure.*/
int receive_data(SSL *ssl, void* data, uint32_t data_size)
{
	int total_received=0, ret;
	char *buffer = (char*) data;

	while((uint32_t)total_received < data_size)
	{
        if((ret = SSL_read(ssl,(void*) &buffer[total_received], (int)data_size - total_received)) > 0)
            total_received += ret;
        else
        {
            mysyslog(LOG_ERR, "Error in SSL recv function\n");
            return -1;
        }
	}
	return 0;
}





