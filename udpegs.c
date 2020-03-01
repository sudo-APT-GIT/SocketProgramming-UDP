#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int r, s, recb, sntb, x;
	int ca;
	char port[5];

	printf("Enter the port number: ");
	gets(port);
	sscanf(port, "%d", &x); //converting string "port" to integer "x"
	
	socklen_t len;
	struct sockaddr_in server, client;
	char buff[50];

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if( s == -1 )
	{
		printf("\nSocket connection error!\n");
		exit(0);
	}

	printf("\nSocket created!:)\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(x);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	len = sizeof(client);
	ca = sizeof(client);

	r = bind(s, (struct sockaddr*)&server, sizeof(server));
	if( r == -1 )
	{
		printf("\nBinding error!:(\n");
		exit(0);
	}

	printf("\nSocket Binded!:)\n");

	while(1)
	{
		recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, &ca);
		if( recb == -1 )
		{
			printf("\nMessage recieving failed!:((\n");
			close(s);
			exit(0);
		}

		printf("\nMessage recieved!:)\n");
		puts(buff);

		if( !strcmp(buff, "stop") )
		{
			break;
		}

		printf("\nType your message: ");
		gets(buff);

		sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
		if( sntb == -1 )
		{
			printf("\nSending failed!:(\n");
			close(s);
			exit(0);
		}

		if( !strcmp(buff, "stop") )
		{
			break;
		}


	}//end of while(1)

	close(s);

}//end of main
