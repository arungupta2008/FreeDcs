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

#ifndef STATCOM_H
#define STATCOM_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/socket.h>

#include "openssl/ssl.h"
#include "openssl/err.h"

#include <native/task.h>
#include <native/timer.h>
#include  <rtdk.h>

//Added for Semaphore support !!
#include <native/sem.h>


#include "socket_functions.h"
//#include "mysyslog.h"
//#ifndef SKEIN_LOG___
//#define SKEIN_LOG___
#include "../../include/Controller/log_linux.h"
#include "../../include/Controller/log_rt.h"
//#endif /* SKEIN_LOG___ */

#define ENGST_PRIORITY 	 	25
#define ENGST_STACK_SIZE 	100000
#define ENGST_CREATE_MODE 	0
#define NO_FREE_CONN		0xFFFF

#define ENGSTATION_SERVER_PORT    55555 /*51002*/
#define MAX_ENGSTATION_CLIENTS 	    100

#define CONNECTION_NOT_ALLOWED		255


uint8_t total_clients;
uint16_t ext_server_port;
uint16_t ext_max_clients;
char ext_module_name[32];

RT_TASK rt_task;
RT_TASK sched_task;     /* to communicate with the scheduler proxy to insert/remove cprogs. */
RT_TASK iosched_task;   /* to communicate with the IO scheduler to insert/remove io modules. */

struct used_port
{
	uint16_t ports;
	pid_t	 childpid;
};


uint16_t get_new_port(int connectSocket, struct used_port *ports);
void makerealtime(char *task_name);
int send_message(SSL* ssl, uint16_t err);
int statcom_main(uint16_t server_port, uint16_t max_clients, char* module_name, int (*ptr_cmd_handler_func) (SSL*,char *));
void child_terminated(void);
int release_dead_connection_entry(int child_pid, struct used_port *ports);
int closeconnection(int connectSocket, SSL* ssl, SSL_CTX* ctx, char *module_name);
int create_socket_and_listen(int server_port, int setupdone );

int create_socket_connect_verify(uint16_t server_port, uint16_t max_clients, char* module_name, int setupdone, int *connection_id);
SSL_CTX* initialize_SSL(uint16_t server_port, uint16_t max_clients, char* module_name);

void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);
int validate_client_server_cert(SSL_CTX* ctx, X509 *client_cert, const char *keyfile);

SSL_CTX* InitServerCTX(void);
void ShowCerts(SSL* ssl);

int verify_max_clients(int max_clients, int connectSocket, char* module_name);

SSL* do_ssl_handshake(SSL_CTX *ctx, int connectSocket);

#endif
