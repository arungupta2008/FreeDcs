/**
 * Project: FreeDCS: A Free Distributed Control System under GPL license.
 * @author Robert Perez, Montevideo-Uruguay, Copyright (C) 2007-2010
 * @see The GNU Public License (GPL)
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

#ifndef SOCKET_FUNCTIONS_H
#define SOCKET_FUNCTIONS_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "openssl/ssl.h"
#include "openssl/err.h"

//#include "mysyslog.h"

extern struct cprog_header* cpheader;
int receive_data(SSL *ssl, void* data, uint32_t data_size);
int send_data(SSL *ssl, void* data, uint32_t data_size);



#endif
