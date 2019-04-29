#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 80 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

int main (int argc, char **argv)
{
    int listenfd, connfd, n;
    int x = 0;
    pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;
    char end[MAXLINE];
    char line[MAXLINE];
    strcpy(end, "end");
    //Create a socket for the socket
    //If sockfd<0 there was an error in the creation of the socket
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }
    
    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((int) strtol(argv[1], (char **)NULL, 10));
    
    //bind the socket
    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
    //listen to the socket by creating a connection queue, then wait for clients
    listen (listenfd, LISTENQ);
    
    printf("%s\n","Server running...waiting for connections.");
    
    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        //accept a connection
        
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s\n","Received request...");
        if(x == 2)
	return 0;
        if ( (childpid = fork ()) == 0 ) {    //if it’s 0, it’s child process
            printf ("%s\n","Child created for dealing with client requests");
            //close listening socket
            close (listenfd);
            
            while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
                printf("\nServer : %d\n", getpid());
    time_t T= time(NULL);
    struct  tm tm = *localtime(&T);
    printf("Date: %02d/%02d/%04d\n",tm.tm_mon+1, tm.tm_mday, tm.tm_year+1900);
    printf("Time: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
		  
                if(strncmp(buf, "end", strlen("end")) == 0){
                    printf("Server is terminated");
		   x = 2;
		   exit(0);
		    
                }
                printf("Transaction received from and result sent to the client: %s", buf);
                
	          FILE *result = popen(buf, "r");
		 while(fgets(line, MAXLINE, result) != NULL){
                    send(connfd, line, strlen(line), 0);
		 
		     printf("%s", line);
		      }
	      memset(line, 0, MAXLINE);
	    
	    memset(buf, 0, MAXLINE);
	    pclose(result);
	    }
            
            if (n < 0){
                printf("%s\n", "Read error");
            exit(0);
        }
        //close socket of the server
        close(connfd);
    
    }
}
return 0;
}
