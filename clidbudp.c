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

struct Student
{
	int regno;
	char name[MAXSIZE];
	char course[MAXSIZE];
	char DOB[MAXSIZE];
};

int main()
{
	int s, r, recb, sntb, x, count,size=4;
	int sa; 
	socklen_t len;
	char port[5], opt, buff[MAXSIZE];
	struct Student records[100];
	struct sockaddr_in server, client;

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
		printf("\nPlease enter your choice:\n");
		printf("1) Insert ( Format: \"1 regno name course dob\" )\n2) Search ( Format: \"2 regno\" )\n3) Delete ( Format: \"3 regno\") \n4) Display\n5) Exit\n\n");
		gets(buff);

		opt=buff[0]; //save the option number in "opt" variable

		sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&server, len);
		if( sntb == -1 )
		{
			printf("\nMessage sending failed!:(\n");
			close(s);
			exit(0);
		}

		printf("\nChoice sent!\n\n"); 

	
		switch(opt)
		{
			case '1':

					/*Insert*/
					recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&server,&sa);
					if( recb == -1 )
					{
						printf("\nMessage recieving failed!:(\n");
						close(s);
						exit(0);
					}

					if(buff[0]=='1')
					{
						size++;
						printf("\nValues inserted successfully! :D\n\n");
					}
					else
					{
						printf("\nValues already present:(\n\n");
					}
					break;
					
			case '2':
					/*search*/
					recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&server,&sa);
					if( recb == -1 )
					{
						printf("\nMessage recieving failed!:(\n");
						close(s);
						exit(0);
					}


					if(buff[0]=='1')
					{
						printf("\nRECORD FOUND! :D\n\n");
					}
					else
					{
						printf("\nRECORD NOT FOUND! :(\n\n");
					}	

					if(buff[0] == '1')	
					{
						/*recieve regno*/
						int rn[1];
						recb = recvfrom(s, rn, sizeof(rn), 0, (struct sockaddr*)&server,&sa);
						if( recb == -1 )
						{
							printf("\nMessage recieving failed!:(\n");
							close(s);
							exit(0);
						}
						char rcv_str[100];
						printf("\n--------------------------------------------------------------------------\n");
						printf("\nRegistration number: %d\n",rn[0]);

						/*recieve name*/
						
						recb = recvfrom(s, rcv_str, sizeof(rcv_str), 0, (struct sockaddr*)&server,&sa);
						if( recb == -1 )
						{
							printf("\nMessage recieving failed!:(\n");
							close(s);
							exit(0);
						}
						printf("\nName: ");
						puts(rcv_str);

						/*recieve course */
						
						recb = recvfrom(s, rcv_str, sizeof(rcv_str), 0, (struct sockaddr*)&server,&sa);
						if( recb == -1 )
						{
							printf("\nMessage recieving failed!:(\n");
							close(s);
							exit(0);
						}
						printf("\nCourse: ");
						puts(rcv_str);
						/*recieve DOB*/
						
						recb = recvfrom(s, rcv_str, sizeof(rcv_str), 0, (struct sockaddr*)&server,&sa);
						if( recb == -1 )
						{
							printf("\nMessage recieving failed!:(\n");
							close(s);
							exit(0);
						}
						printf("\nDate of birth: ");
						puts(rcv_str);
					}
					break;

			case '3':
					/*delete*/
					recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&server,&sa);
					if( recb == -1 )
					{
						printf("\nMessage recieving failed!:(\n");
						close(s);
						exit(0);
					}

					if(buff[0]=='0')
					{
						printf("Record Not Found!:(\n");
					}
					else
					{
						size--;
						printf("Record deleted! :D\n");
					}
					
					break;
					
			case '4':
					/*display*/
					recb = recvfrom(s, records, sizeof(records), 0, (struct sockaddr*)&server,&sa);
					if( recb == -1 )
					{
						printf("\nMessage recieving failed!:(\n");
						close(s);
						exit(0);
					}

					
					for (count = 0; count <=size; count++)
					{
						printf("\n--------------------------------------------------------------------------\n");
						printf("\nRECORD %d \n", count+1);
						printf("\nRegistration number: %d\n",records[count].regno);
						printf("\nName: ");
						puts(records[count].name);
						printf("\nCourse: ");
						puts(records[count].course);
						printf("\nDate of birth: ");
						puts(records[count].DOB);
					}
					printf("\nDone!\n");

					break;

			case '5':
					/*exit*/
					recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&server,&sa);
					if( recb == -1 )
					{
						printf("\nMessage recieving failed!:(\n");
						close(s);
						exit(0);
					}

					break;		

			default:
					printf("\nPlease choose valid options!ಠ_ಠ\n");
					break;

		}//end of switch case

		if(strcmp(buff, "exit")==0)
		{
			break;
		}

	}//end of while(1)

	close(s);
	printf("\nEnding..\n");
}//end of main
