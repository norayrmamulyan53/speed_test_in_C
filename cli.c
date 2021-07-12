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

int main() {
	int sockfd;
	//Size of buffer is 8kb (as pluto sends thus much)
	char *buffer= (char *)calloc(1024, sizeof(char));
	char *buffEnd="end";
	struct sockaddr_in	 servaddr;

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("192.168.1.172");//INADDR_ANY;
	
	// Sending 8kb packages with 0.01s interval
	for(int i= 0;i<10000;i++){
	  sleep(.01);
	  sendto(sockfd, (const char *)buffer, strlen(buffer),
	  MSG_CONFIRM, (const struct sockaddr *) &servaddr,
	  sizeof(servaddr));
	}
	// Send end Message
	sendto(sockfd, (const char *)buffEnd, strlen(buffEnd),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));

	printf("Message sent.\n");
	close(sockfd);
	return 0;
}

