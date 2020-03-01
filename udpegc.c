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
	char buff[50];

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

	while(1)
	{
		printf("\nPlease enter your message: \n");
		gets(buff);

		sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&server, len);
		if( sntb == -1 )
		{
			printf("\nMessage sending failed!:(\n");
			close(s);
			exit(0);
		}

		if( !strcmp(buff, "stop") )
			break;

		recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&server,&sa);
		if( recb == -1 )
		{
			printf("\nMessage recieving failed!:(\n");
			close(s);
			exit(0);
		}

		//printf("\nMessage Recieved!:D\n");
		puts(buff);

		if( !strcmp(buff, "stop") )
			break;

	}//end of while(1)

	close(s);

}//end of main
