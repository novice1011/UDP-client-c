// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
    int sockfd;
    struct sockaddr_in	 servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    //set reading timeout
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("timeout setting error");
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr =INADDR_ANY;// inet_addr("10.42.0.100");//("192.168.4.1") or INADDR_ANY

    int msgNum=0;
    while (1){
        unsigned int n, len;
        char buffer[MAXLINE];

        char sendlalala[100];
        sprintf(sendlalala,"This is number %d\n",msgNum);

        sendto(sockfd, (const char *)sendlalala, strlen(sendlalala),
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        printf("%s", sendlalala);

        msgNum++;
        if (msgNum>100)msgNum=0;

        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);

        sleep(1);//beware, this will stop all thread
    }

    close(sockfd);
    return 0;
}

//Code: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
//timeout: https://stackoverflow.com/questions/13547721/udp-socket-set-timeout
