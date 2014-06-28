/**
 * Project: FreeDCS: A Free Distributed Control System under GPL license.
 * @author Robert Perez, Montevideo-Uruguay, Copyright (C) 2007-2013
 * @see The GNU Public License (GPL)
 *
 * This file contains the code that handles common station communication.
 * It contains code to receive a new client (engineering or operator station) and forks a new process
 * to handle that client.
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

 /*! \file */
#include "../../include/Controller/statcom.h"

#define OK      0
#define NOTOK   -1
#define ERROR   NOTOK

static struct used_port used_ports[MAX_ENGSTATION_CLIENTS]; /*to keep track of each client using wich port.*/

/*Sends a short custom made message via ethernet to a connected client*/
int send_message(SSL *ssl, uint16_t err)
{
	SSL_write(ssl, &err, sizeof(err));
	return 0;
}


/** \brief child_terminated() function is connected via a signal to the termination of a child process.
 *        The function releases the connection in the master connection's table kept by the parent.
 * \param None
 * \return None
 *
 */
void child_terminated(void)
{
	pid_t child_pid;
	int status;

	child_pid = wait(&status);
	log_message(LG_INFO, "Child exited with PID=%d", child_pid);
	if(release_dead_connection_entry(child_pid, used_ports) != 0)
        log_message(LG_INFO, "Tried to release connection in module %s with child_pid %d but was not found on the table", ext_module_name, child_pid);

	return;
}

int release_dead_connection_entry(int child_pid, struct used_port *ports)
{
 	int i;

    if(child_pid <= 0)
    {
         log_message(LG_INFO, "child_pid cannot be less or equal zero in module %s. Please investigate this issue.", ext_module_name);
         return -1;
    }

    if(ports == NULL)
    {
         log_message(LG_INFO, "Used_port struct null in module %s. Please investigate this issue.", ext_module_name);
         return -1;
    }

    for(i=0; i<ext_max_clients; i++)
	{
		if(ports[i].childpid == child_pid)
		{
			ports[i].childpid = 0;
			ports[i].ports = 0;
			total_clients--;
			log_message(LG_INFO, "Released %s connection number %d", ext_module_name, i);
			return 0;
		}
	}
	return -1;
}




/*TODO: functions are not in the test suite yet. They seem simple enough, all return values are checked */



void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    if (SSL_CTX_load_verify_locations(ctx, CertFile, KeyFile) != 1)
        ERR_print_errors_fp(stderr);

    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
        ERR_print_errors_fp(stderr);

    if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    if (!SSL_CTX_check_private_key(ctx))
    {
        log_message(LG_ERR, "Private key does not match the public certificate");
        abort();
    }

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
}


int validate_client_server_cert(SSL_CTX* ctx, X509 *client_cert, const char *keyfile)
{
	if ( client_cert != NULL )
	{
		if (SSL_CTX_use_certificate(ctx, client_cert) <= 0)
		{
			ERR_print_errors_fp(stderr);
		        abort();
		}
		else
			log_message(LG_INFO , "SSL_CTX_use_certificate");


		if (SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM) <= 0)
    		{
        		ERR_print_errors_fp(stderr);
        		abort();
    		}
		else
			log_message(LG_INFO ,"SSL_CTX_use_PrivateKey_file OK");

		/* verify private key between client certificate and stored private key to see if they match*/
		if (!SSL_CTX_check_private_key(ctx))
		{
        		log_message(LG_ERR , "Private key of server does not match the public client certificate");
		        abort();
	   	}
		return 0;
	}
	return 1;
}

SSL_CTX* InitServerCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    char server_key[] = "server.key";
    char server_crt[] = "server.crt";

    SSL_library_init(); /* this functions always returns 1. */

    OpenSSL_add_all_algorithms();  /* void return values */
    SSL_load_error_strings();

    if((method = SSLv3_server_method()) == NULL)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    if((ctx = SSL_CTX_new(method)) == NULL)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    LoadCertificates(ctx, server_crt, server_key); /* load certs */
    return ctx;
}

void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if ( cert != NULL )
    {
        log_message(LG_INFO ,"Client certificates:");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        log_message(LG_INFO, "Subject: %s",line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        log_message(LG_INFO, "Issuer: %s",line);
        free(line);
        X509_free(cert);
    }
    else
        log_message(LG_ERR ,"No certificates.");
}








/** \brief create_socket_and_listen(int server_port, struct sockaddr_in * clientAddress, int *mysocket )
           function is used to create and wait for connections of stations wishing to communicate
           with this controller.
 *
 * \param server_port int used to indicate to wich port it should listen for connections.
 * \param clientAddress struct sockaddr_in* structure needed to create the listen socket.
 * \param mysocket int* variable to return the listening socket because it's used in other functions.
 * \return int returns a connectSocket when a stations connects to it.
 *
 */
int create_socket_and_listen(int server_port, int setupdone )
{
    int connectSocket, err;
    static int mysocket;
    static struct sockaddr_in serverAddress;
    static socklen_t clientAddressLength;
    struct sockaddr_in clientAddress;
    int yes;

    if(setupdone == 0)  /* do it only the first time. */
    {
        mysocket = socket(AF_INET, SOCK_STREAM, 0);
        if (mysocket < 0)
            return -1;

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port = htons(server_port);

        yes=1;

        if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            return -1;
        }

        if ((err = bind(mysocket,(struct sockaddr *) &serverAddress,sizeof(serverAddress))) < 0)
        {
            log_message(LG_ERR, "cannot bind socket %s", strerror(-err));
            return -1;
        }

        if ( listen(mysocket, 10) != 0 )
        {
            perror("Can't configure listening port");
            return -1;
        }

        log_message(LG_INFO, "Socket created! Waiting for a TCP connection");

        clientAddressLength = sizeof(clientAddress);
    }


    connectSocket = accept(mysocket, (struct sockaddr *) &clientAddress, &clientAddressLength);  /* accept connection as usual */
    log_message("Connection: %s:%d",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

	if (connectSocket < 0)
	{
	    log_message(LG_ERR, "cannot accept connection ");
	    return -1;
	}

	log_message(LG_INFO, "Connection accepted");

    return connectSocket;

}

SSL* do_ssl_handshake(SSL_CTX *ctx, int connectSocket)
{
    int error;

    SSL* ssl;
    if((ssl = SSL_new(ctx)) == NULL)    /* get new SSL state with context */
        return NULL;

    SSL_set_fd(ssl, connectSocket);     /* set connection socket to SSL state */

    if ( SSL_accept(ssl) == -1 )        /* do SSL-protocol accept */
    {
        ERR_print_errors_fp(stderr);
        return NULL;
    }else{
        error = validate_client_server_cert(ctx, SSL_get_peer_certificate(ssl), "server.key");
        if(error == 0)
            {log_message(LG_INFO , "client certificate and server key matches !");
        }else{
             log_message(LG_ERR ,"client certificate and server key doesnt match !");
             return NULL;
        }
    }

    return ssl;
}

int verify_max_clients(int max_clients, int connectSocket, char* module_name)
{
    if(total_clients >= max_clients)
    {

        log_message(LG_WARN, "The number of %s connected exceeds the limit.", module_name);
        /*send "no more connections" message
        msg = NO_FREE_CONN;
        send_ushortdata(connectSocket, &msg ); TODO: perhaps this could be sent only with plain sockets, because no SSL yet.*/
        shutdown(connectSocket,2);
        close(connectSocket);
        return NOTOK;
    }
    else
        return OK;
}


int closeconnection(int connectSocket, SSL* ssl, SSL_CTX* ctx, char *module_name)
{
    if (module_name == NULL)
    {
        log_message(LG_ERR, "Argunments to closeconnection are null");
    }
    else
        log_message(LG_INFO, "Quiting %s ethernet command parser", module_name);

    shutdown(connectSocket,SHUT_RDWR);
    close(connectSocket);
    SSL_free(ssl);         /* release SSL state */
    SSL_CTX_free(ctx);     /* release SSL context */
    return 0;

}


SSL_CTX* initialize_SSL(uint16_t server_port, uint16_t max_clients, char* module_name)
{
 	SSL_CTX *ctx;

 	log_message(LG_INFO, "%s initing..", module_name);
	ext_server_port = server_port;
	ext_max_clients = max_clients;
	strcpy(ext_module_name, module_name);
	memset(used_ports,0,max_clients*sizeof(struct used_port));

	if((ctx = InitServerCTX()) == NULL)         /* initialize SSL */
    {
         log_message(LG_ERR,"Error while creating ctx");
         abort();
    }

    return ctx;
}


int create_socket_connect_verify(uint16_t server_port, uint16_t max_clients, char* module_name, int setupdone, int *connection_id)
{
    int connectSocket;

    log_message(LG_INFO, "Waiting connections on %d..", server_port);

    if((connectSocket = create_socket_and_listen(server_port, setupdone)) < 0)
    {
         log_message(LG_ERR, "Cannot create socket to listen.");
         return ERROR;
    }

    if(verify_max_clients(max_clients, connectSocket, module_name) != OK)
    {
         log_message(LG_ERR, "verify_max_clients returned not OK.");
         return ERROR;
    }

    if((*connection_id = get_new_port(connectSocket, used_ports)) == NO_FREE_CONN)
    {
         log_message(LG_ERR, "get_new_port returned NO_FREE_CONN");
         return ERROR;
    }

    return connectSocket;
}

/** \brief
 *
 * \param server_port uint16_t
 * \param max_clients uint16_t
 * \param module_name char*
 * \param (*ptr_cmd_handler_func int
 * \return int
 *

  STILL TO DEFINE:
       - key and crt files what will be the names ? If there are only one per system it should be named the same for all ?
       - Check return values and fail accordingly.
*/

int statcom_main(uint16_t server_port, uint16_t max_clients, char* module_name, int (*call_command_handler_function) (SSL *, int))
{
	int 	    setupdone=0, connection_id, connectSocket;
	pid_t 	    pid;

    /*SSL specific.*/
    SSL *ssl;
	SSL_CTX *ctx;

	total_clients = 0;
	ctx = initialize_SSL(server_port, max_clients, module_name);

	while(1) /*while loop to serve many connections. When one connection arrives, a new process is forked to handle it.*/
	{         /*and the parent process comes here again to continue listening.*/
        if((connectSocket = create_socket_connect_verify(server_port, max_clients, module_name, setupdone, &connection_id)) == ERROR)
            goto error;

        switch(pid = fork()) /* here a new child process is created and the parent continues.*/
        {
            case -1:/*something went wrong..*/
                log_message(LG_ERR, "Error in forking a new %s connection. Aborting.....", module_name);
                break;

            case  0:/*child process*/
                signal(SIGCHLD,SIG_IGN); /* to keep track of when a child is terminated.*/
                signal(SIGCLD,SIG_IGN); /* to keep track of when a child is terminated.*/

                if((ssl = do_ssl_handshake(ctx, connectSocket)) != NULL) /* if OK call function to handle engstation comms.*/
                    call_command_handler_function(ssl, connection_id); /* this function don't return until the station ends the connection.*/

                closeconnection(connectSocket, ssl, ctx, module_name); /* we're done handling this connection, the client will exit now.*/
                exit(0);	/*when finished handling comms, we kill the child.*/
                break;

            default: /*parent process go on. The parent goes back to the begining of the while loop to continue listening.*/
                      /*the child handles the new connection.*/
                setupdone = 1;
                log_message(LG_INFO, "Parent saving PID=%d of child in slot=%d", pid,connection_id );
                used_ports[connection_id].childpid = pid; /*save the PID of the child in the used_port table.*/
                break;
        }
error: ; /*an empty statmenent is needed by the compiler.*/
    }

	return 0; 	/*we should never reach here !*/
}

/*There is an array that holds a map of the ports used to connect to other clients via ethernet.
This function searches in the array looking for a free socket and return an index that added to
the base give us the port number.*/
uint16_t get_new_port(int connectSocket, struct used_port *ports)
{
	uint16_t port=0;
	while(ports[port].ports != 0)
	{
		if(port < ext_max_clients)
			port++;
		else
		{
			log_message(LG_ERR, "No more free ports for %s.", ext_module_name);
			shutdown(connectSocket,SHUT_RDWR);
			close(connectSocket);
			return NO_FREE_CONN;
		}
	}
	ports[port].ports = 1; /* mark it as used.*/
	total_clients++; /*one more client being served.*/

    return port;
}

void makerealtime(char *task_name)
{
	int err=0;
	mlockall(MCL_CURRENT|MCL_FUTURE); /* Used by Xenomai to avoid paging used RAM.*/

	err = rt_task_shadow(&rt_task, task_name, ENGST_PRIORITY, ENGST_CREATE_MODE);
	rt_log_message(LG_INFO, "In makerealtime after shadow err = %d", err);

    /*5 secs ? if fdsched is not running, this blocks forever, why ?*/
	err = rt_task_bind(&sched_task,"SCHED_PROXY",5000000000ULL);
	if (err < 0 ){
		rt_log_message(LG_ERR, "Bind error %d in %s: %s",-err,ext_module_name,strerror(-err));
		exit(1);
	}
	else
		rt_log_message(LG_INFO, "Binding engstation to sched task succesfully.");

	return;
}


