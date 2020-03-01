#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#define MAXSIZE 50

int main()
{
	int s, r, recb, sntb, x;
	int sa; 
	socklen_t len;
	char port[5];

	printf("Enter the port number: ");
	gets(port);
	sscanf(port, "%d", &x); //converting string "port" to integer "x"

	struct sockaddr_in server, client;
	int buff[MAXSIZE], n, z;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if( s == -1 )
	{
		printf("\nSocket creation error:(\n");
		exit(0);
	}

	printf("\nSocket created!:)\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(x);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa = sizeof(server);
	len = sizeof(server);

	
	printf("Enter the number of elements in the array: \n");
	scanf("%d",&buff[0]);

	printf("Enter the array: \n");
	for(z=1; z<=buff[0]; z++)
	{
		scanf("%d",&buff[z]);
	}

	printf("Waiting for the server to compute...\n");

	sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&server, len);
	if( sntb == -1 )
	{
		printf("\nMessage sending failed!:(\n");
		close(s);
		exit(0);
	}


	recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&server,&sa);
	if( recb == -1 )
	{
		printf("\nMessage recieving failed!:(\n");
		close(s);
		exit(0);
	}

	printf("\nServer says: The sorted array is given below\n");
	
	for(z=1; z<=buff[0]; z++)
	{
		printf("%d    ",buff[z]);
	}

	close(s);

}//end of main
