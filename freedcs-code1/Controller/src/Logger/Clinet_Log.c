#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define LoggingPort 55556

int main(int argc, char *argv[])
{
	printf("Waiting for Log message1");
    char *Path_to_log_file = "/log/Controller.log";
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(LoggingPort);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
	char recvBuff[1024];
	memset(recvBuff, '\0',sizeof(recvBuff));
	printf("Waiting for Log message1");
	FILE *f = fopen(Path_to_log_file, "a");
	if (f == NULL) {
		perror("fopen");
		printf("%s\n",strerror(fopen));
        fprintf(stderr, "Cannot open file `%s`!\n",Path_to_log_file);
        return 1;
    }
    while(1)
    {
		printf("Waiting for Log message");
		int n = 0;
		memset(recvBuff, '\0',sizeof(recvBuff));
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
	while((n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0){
		printf("Size of Buffer Received :: %d",n);	
        ticks = time(NULL);
        recvBuff[n] = 0;
        printf("%s",recvBuff);
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
	   //fwrite(recvBuff, strlen(recvBuff), 1, f);
        //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
	}
        //write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }
}
