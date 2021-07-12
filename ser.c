// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define PORT	 8080
#define MAXLINE 8192	

int speedCalc(time_t start, time_t end)
{
      return end-start;
}
void printStates(size_t bit, size_t seconds)
{
	double byte=bit/8;
	char size[MAXLINE], speed[MAXLINE];
	if(byte>1073741824) {
		snprintf(size, MAXLINE, "%.3fgb",byte/1073741824);
		snprintf(speed, MAXLINE, "%.3fgbps",byte/1073741824/seconds);
	}
	else if(byte>1048576) {
		snprintf(size, MAXLINE, "%.3fmb",byte/1048576); 
                snprintf(speed, MAXLINE, "%.3fmbps", byte/1048576/seconds);
        }

	else if(byte>1024) {
		snprintf(size, MAXLINE, "%.3fkb",byte/1024);
		snprintf(speed, MAXLINE, "%.3fkbps", byte/1024/seconds);
	}
	else {
		snprintf(size, MAXLINE, "%.3ldbit", bit);
		snprintf(speed, MAXLINE, "%.3ldbps", bit/seconds);
	}

	printf("Received %s in %ldseconds with %s\n", size, seconds, speed);
}

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	int len, n;
	// Speed calculating start after receiving a buffer
	size_t bufferSize=0;
	time_t start, end, duration;
	while(1) {	
		len = sizeof(cliaddr); //len is value/resuslt
		memset(buffer, 0, sizeof(buffer));	
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);
		
		if(bufferSize == 0) time(&start);
		if(strcmp(buffer, "end") == 0) {
			time(&end);
			duration=speedCalc(start,end);
			printStates(bufferSize, duration);	
			break;
		}
		buffer[n] = '\0';
		bufferSize += sizeof(buffer);
	}
	return 0;
}

