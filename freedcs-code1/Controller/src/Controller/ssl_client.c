//SSL-Client.c
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL    -1
#define LoggingPort 55556
// Simple structure to keep track of the handle, and
// of what needs to be freed later.
typedef struct {
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
} connection;

// current machine log code 
void Log_to_current_machine();
// This is modified one here parent will stay(in the shell waiting output)and child will call the logging function.
static void skeleton_daemon(){
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0){
        exit(EXIT_FAILURE);
	}

    /* Success: Let the parent terminate */
	//parent will stay in track 
    if (pid > 0){
      return;
	}
	if(pid == 0){
			printf("Child created");
	// Child's work starts here 
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
    chdir("/");

    /* Close all open file descriptors */
    //int x;
    //for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    //{
    //    close (x);
    //}
	// Starts logging for machine
	Log_to_current_machine();
	}
}

// Log Code 
void Log_to_current_machine(){	
	printf("Log_to_current_machine started\n");
    char *Path_to_log_file = "/log/Controller.log";
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    //char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //memset(sendBuff, '0', sizeof(sendBuff)); 
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(LoggingPort);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
	char recvBuff[1024];
	memset(recvBuff, '\0',sizeof(recvBuff));
	printf("Waiting for Log message");
	FILE *f = fopen(Path_to_log_file, "a+");
	if (f == NULL) {
		perror("fopen");
		//printf("%s\n",strerror(fopen));
        fprintf(stderr, "Cannot open file `%s`!\n",Path_to_log_file);
    }
printf("\n");
    while(1)
    {
		//printf("Waiting for Log message");
		int n = 0;
		memset(recvBuff, '\0',sizeof(recvBuff));
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
	while((n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0){
        	//printf("***Buffer Size %d",n);
		ticks = time(NULL);
        recvBuff[n] = '\0';
        printf("%s",recvBuff);
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
	   fwrite(recvBuff, strlen(recvBuff), 1, f);
        //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
	}
        //write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }	
}

//Added the LoadCertificates how in the server-side makes.    
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
 /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}

SSL_CTX* InitCTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = SSLv3_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("No certificates.\n");
}


// Write text to the connection
void sslWrite (connection *c, char *text)
{
	printf("Writing Command %s\n",text);
  if (c)
    SSL_write (c->sslHandle, text, strlen (text));
}

// Write text to the connection
void send_command (connection *c, uint32_t *command)
{
	printf("Writing Command %lu\n",(unsigned long)command);
  if (c)
    SSL_write (c->sslHandle, command, strlen (command));
}

// Disconnect & free connection struct
void sslDisconnect (connection *c)
{
  if (c->socket)
    close (c->socket);
  if (c->sslHandle)
    {
      SSL_shutdown (c->sslHandle);
      //SSL_free (c->sslHandle);
      c->sslHandle = NULL;
    }
  if (c->sslContext)
    //SSL_CTX_free (c->sslContext);
    c->sslContext = NULL;
  free (c);
}

// Read all available text from the connection
char *sslRead (connection *c)
{
	char buffer[1024];
  const int readSize = 1024;
  char *rc = NULL;
  int received, count = 0;
  

  if (c)
    {
      while (1)
        {
          if (!rc)
            rc = (char *) calloc("", sizeof(char));
          else
            rc = (char *) realloc (rc, (count + 1) *
                          readSize * sizeof (char) + 1);

          received = SSL_read (c->sslHandle, buffer, readSize);
          buffer[received] = '\0';

          if (received > 0)
            strcat (rc, buffer);

          if (received < readSize)
            break;
          count++;
        }
    }

  return rc;
}

// Establish a connection using an SSL layer
connection *sslConnect (char *host_supplied , char *port_supplied, char *CertFile , char *KeyFile )
{
	printf("host_supplied : %s \n",host_supplied);
	printf("port_supplied : %s \n",port_supplied);
	printf("CertFile : %s \n",CertFile);
	printf("KeyFile : %s \n",KeyFile);
  connection *c;
  c = malloc (sizeof (connection));
  c->sslHandle = NULL;
  c->sslContext = NULL;

  c->socket = OpenConnection(host_supplied, atoi(port_supplied));
  if (c->socket)
    {
      // Register the error strings for libcrypto & libssl
      SSL_load_error_strings ();
      // Register the available ciphers and digests
      SSL_library_init ();

      // New context saying we are a client, and using SSL 2 or 3
      c->sslContext = InitCTX();
      if (c->sslContext == NULL)
        ERR_print_errors_fp (stderr);
      // Load all certificates   
	  LoadCertificates(c->sslContext, CertFile, KeyFile);
      // Create an SSL struct for the connection
      c->sslHandle = SSL_new(c->sslContext);
      if (c->sslHandle == NULL)
        ERR_print_errors_fp (stderr);

      // Connect the SSL struct to our connection
      if (!SSL_set_fd (c->sslHandle, c->socket))
        ERR_print_errors_fp (stderr);

      // Initiate SSL handshake
      if (SSL_connect (c->sslHandle) != 1)
        ERR_print_errors_fp (stderr);
    }
  else
    {
      perror ("Connect failed");
    }

  return c;
}


int main(int count, char *strings[])
{
	connection *c;
    char *host_supplied, *port_supplied;
    char CertFile[] = "server.crt";
    char KeyFile[] = "server.key";
    if(count<3){
		printf("<usage> : <IP> <Port> <Command>\n");
		exit(1);
	}
	skeleton_daemon();
    uint32_t command = atoi(strings[3]);
	
    SSL_library_init();
	host_supplied=strings[1];
	port_supplied=strings[2];
	c = sslConnect (host_supplied,port_supplied,CertFile,KeyFile);
    printf("Connected with %s encryption\n", SSL_get_cipher(c->sslHandle));
    ShowCerts(c->sslHandle);        /* get any certs */
    send_command(c,&command); /* encrypt & send message */
    printf("Received: \"%s\"\n",sslRead(c));
	sslDisconnect (c);
    return 0;
}
