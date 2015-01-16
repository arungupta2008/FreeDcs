//#include "../../include/Controller/log_linux.h"
//For Setting up the rt_file 
//#include "../../include/Controller/log_rt.h"
#include "../../include/Controller/statcom.h"
#include <sys/file.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include <sys/socket.h>
#include <stdlib.h>
//#define EXIT_FAILURE_  log_message(LG_MSG, "Incorrect arguments Log_to_station"); exit(1);
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

char *Path_to_log_cleint = "/log/client_log";
static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0){
		EXIT_FAILURE;
        exit(EXIT_FAILURE);
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
    chdir("/");

    /* Close all open file descriptors */
    //int x;
    //for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    //{
    //    close (x);
    //}
   
}
// Input is client hostname and client port number 
int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL )
    {
		perror(hostname);
        //printf("%s\n",strerror(hostname));
    }
    sd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        //printf("%s\n",strerror(hostname));
    }
    return sd;
}
//Port is already defined Here
#define LoggingPort 55556
#define SUCCESS 1
#define FAILURE 0
void log_to_current_client(const char *IP_address){
	//printf("Here00 %s",IP_address);
		char client_file[200];
		snprintf (client_file,200,"%s/%s",Path_to_log_cleint, IP_address);
		//printf("%s\n",client_file);
	FILE *f = fopen(client_file, "rb");
	if (f == NULL) {
		perror("fopen");
		//printf("%s\n",strerror(fopen));
        fprintf(stderr, "Cannot open file `%s`!\n",client_file);
        return 1;
    }
	fseek(f, 0, SEEK_END);
	//Here i have taken 7 due to expectation that character size is least 8 bytes:
	if(ftell(f) > 7){
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		//printf("Here1");
		char *Logstring = malloc(fsize + 1);
		fread(Logstring, fsize, 1, f);
		Logstring[fsize] = '\0';
		printf("\n**************************String To write***************************\n");
		printf("\n%s\n",Logstring);
		printf("\n*****************************************************\n");
		int Client_fd = OpenConnection(IP_address,LoggingPort);
		if(write(Client_fd, Logstring, strlen(Logstring)) != -1 ){
				fseek(f, 0, SEEK_END);
				long fsize_new = ftell(f);
				printf("\n**************************String written***************************\n");
				printf("\n%s\n",Logstring);
				printf("\n**************************String written with old fsize_new : %d  :::: fsize : %d****Logstring  ::::: %d ***********************\n",fsize_new,fsize,strlen(Logstring) );
				
				// If new logs have written been. then delete the log we have sent to client machine.
				if(fsize < fsize_new ){
						printf("\n**************************String written1***************************\n");
						fseek(f, fsize, SEEK_END);
						fsize_new = ftell(f);
						char *Logstring_to_write = malloc(fsize_new + 1);
						fread(Logstring_to_write, fsize_new, 1, f);
						FILE *f_ = fopen(client_file, "wb");
						fwrite(Logstring_to_write, strlen(Logstring_to_write) + 1, 1, f_);
						fclose(f_);
					}
				//fclose(Client_fd);
			}else{
				printf("Can not log to IP addreess : %s\n",IP_address);
				}
	close(Client_fd);		
	}else{
		printf("File is empty %s\n",IP_address);
		}
	fclose(f);
	//return SUCCESS;
	}

int main( int argc, char *argv[] ){
	skeleton_daemon();
	pid_t get_pid = getpid();
	char str[6]; // Max PID is 5 only
	snprintf(str, 6,"%d", get_pid);
	int pid_file = open("/var/run/logcd.pid", O_CREAT | O_RDWR, 0666);
	write(pid_file,str, strlen(str));
	int rc = flock(pid_file, LOCK_EX | LOCK_NB);
	if(rc) {
		if(EWOULDBLOCK == errno)
        printf("logcd already running\n"); // another instance is running
	}
	else {
		// this is the first instance
		log_message(LG_MSG, "logcd started");
		for(;;){
			sleep(SLEEP_TIME);
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir (Path_to_log_cleint)) != NULL) {
			/* print all the files and directories within directory */
			while ((ent = readdir (dir)) != NULL) {
				// Read the File and log it to particular node
				if(!( strcmp (ent->d_name,".") == 0 || strcmp (ent->d_name, "..") == 0 )){
					printf ("%s\n", ent->d_name);
				log_to_current_client(ent->d_name);
				printf("\n**************************String written ---- ***************************\n");
				}
			}
			closedir (dir);
			} else {
			/* could not open directory */
				return EXIT_FAILURE;
			}
		}
	}
	return EXIT_SUCCESS;
	}
