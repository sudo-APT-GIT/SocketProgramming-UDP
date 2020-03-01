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
	char port[5],opt;
	struct sockaddr_in server, client;
	char buff[MAXSIZE];

	printf("Enter the port number: ");
	gets(port);
	sscanf(port, "%d", &x); //converting string "port" to integer "x"

	

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
		printf("\nPlease enter the filename: \n");
		gets(buff);

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

		if(buff[0]=='1')
		{
			printf("\nFile found!(^o^)\n\n");
		}
		else
		{
			printf("\nError 404: File not found! ¯\\_(ツ)_/¯\n\nPlease enter valid file names.\n");
			break;
		}
		
		printf("Please enter your choice:\n");
		printf("1) Search ( Format: \"1 string_you_want_to_search\" )\n2) Replace ( Format: \"2 old_word new_word\" \n3) Reorder\n4) Exit\n\n");
		gets(buff);
		opt=buff[0];

		sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&server, len);
		if( sntb == -1 )
		{
			printf("\nMessage sending failed!:(\n");
			close(s);
			exit(0);
		}

		printf("\nChoice sent!\n\n");

		recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&server,&sa);
		if( recb == -1 )
		{
			printf("\nMessage recieving failed!:(\n");
			close(s);
			exit(0);
		}

		/* if option 4 is chosen (exit) */

		if(strcmp(buff, "exit")==0){
			break;
		}
		
		/* the code below can be implemented using switch cases too */

		/* if option 1 is chosen (search) */

		if(opt=='1')
		{
			if(buff[0]=='1')
			{
				printf("\nWORD FOUND! (^o^) \n\n");
			}
			else
			{
				printf("\nWORD NOT FOUND! （>﹏<）\n\n");
			}
		}

		/* if option 2 is chosen (replace) */

		if(opt == '2')
		{
			if(buff[0]=='1')
			{
				printf("\nWORD REPLACED! (^o^) \n\n");
			}
			else
			{
				printf("\nWORD NOT REPLACED! （>﹏<）\n\n");
			}		
		}

		/* if option 3 is chosen(reorder) */

		if(opt=='3')
		{
			if(buff[0]=='1')
			{
				printf("\nResults successfully saved!\n\n");
			}
			else
			{
				printf("\nThere was a problem while saving the results.（>﹏<）\nPlease try again!\n");
			}
		}


	}//end of while(1)

	close(s);

}//end of main
