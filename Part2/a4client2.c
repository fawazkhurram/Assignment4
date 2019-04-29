
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int
main(int argc, char **argv)
{
    int n;
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];
    char fileName[MAXLINE];
    FILE *fp;
    FILE *out;
    alarm(300);
    //basic check of the arguments
    //additional checks can be inserted
    
    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }
    
    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(argv[1]);
    servaddr.sin_port =  htons((int) strtol(argv[2], (char **)NULL, 10)); //convert to big-endian order
    
    
    fp = fopen(argv[3], "r");
    out = fopen(argv[4], "w");
    //Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Problem in connecting to the server");
        exit(3);
    }
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        fprintf(out, "Sending command: %s\n", sendline);
       if(send(sockfd, sendline, strlen(sendline), 0)==-1){
           printf("Not sent");}
   
        
        if (recv(sockfd, recvline, MAXLINE,0) == 0){
	  //error: server terminated prematurely
	              perror("The server terminated prematurely");
	                        exit(4);}
       
        //printf("Command result received from the server: %s\n", recvline);
       fprintf(out, "Client: %d\n", getpid());
	 time_t T= time(NULL);
	 struct  tm tm = *localtime(&T);
fprintf(out, "Date: %02d/%02d/%04d\n",tm.tm_mon+1, tm.tm_mday, tm.tm_year+1900);
 fprintf(out, "Time: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
 fprintf(out, "Result of command receive by client\n");
	//fputs(recvline, stdout);
	fprintf(out, "%s\n", recvline);
	memset(recvline, 0, MAXLINE);
    }
    fclose(out);
    fclose(fp);
    exit(0);
}
