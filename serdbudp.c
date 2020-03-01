#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXSIZE 50
/* define a structure to store regno, name, course, DOB */

struct Student
{
	int regno;
	char name[MAXSIZE];
	char course[MAXSIZE];
	char DOB[MAXSIZE];
};

void fill_five(struct Student *records);

int main()
{
	int r, s, recb, sntb, x,index=0, a=0, size=4, count, regnum, rec_found=0;
	int ca;
	struct Student records[100];
	int num=0,b;
	char str[MAXSIZE], e, sstr[MAXSIZE], filename[MAXSIZE], port[5];
	char reorderstr[MAXSIZE][MAXSIZE], temp[MAXSIZE], buff[MAXSIZE];
	char *pch;
	socklen_t len;
	struct sockaddr_in server, client;

	printf("Enter the port number: ");
	gets(port);
	sscanf(port, "%d", &x); //converting string "port" to integer "x"

	fill_five(&records);
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
		/* wait for clients reply */

		recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, &ca);
		if( recb == -1 )
		{
			printf("\nMessage recieving failed!:((\n");
			close(s);
			exit(0);
		}

		switch(buff[0])
		{
			case '1':
					/*insert*/

					printf("\n\nTrying to insert values ....\n");

					/*Search the 'records' array to check if the 
					registration number recieved is already present*/
					
					rec_found=0;
					e=buff[index+2];
					while(e!='\0')
					{
						e=buff[index+2];
						sstr[index]=e;
						index++;
					}
					sstr[index]='\0';
					index=0;
					regnum=atoi(sstr);			

					for(count = 0; count <=size; count++)
					{
						if(regnum == records[count].regno)
						{
							rec_found=1;
							break;
						}

					}
					/*if found then send '0' to the client
					to indicate that file is already present*/
					if(rec_found == 1)
					{

						buff[0] = '0';
						sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}

					} 
					/*
					if not found then insert the values recieved 
					and send '1' to the client indicating that value was inserted
					*/
					else
					{
						num=0;
						pch=strtok(buff," ");
						size++;
						while(pch!=NULL)
						{
							if(num==0)
							{
								num++;
								pch=strtok(NULL, " ");
								continue;
							}
							else if(num==1)
							{
								regnum=atoi(pch);
								records[size].regno = regnum;
								num++;
							}
							else if(num==2)
							{
								strcpy(records[size].name,pch);
								num++;
							}
							else if(num==3)
							{
								strcpy(records[size].course,pch);
								num++;
							}
							else if(num==4)
							{
								strcpy(records[size].DOB,pch);
								num++;
							}
							
							pch=strtok(NULL, " ");

						} //end of while(pch!=NULL)

						if(num == 5)
						{	
							strcpy(buff,"1");
						}
						else
						{
							strcpy(buff, "0");
						}
					
						sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}

						
					}//end of if-else
					printf("\nResults sent!\n");
			
			break;

			case '2':
					/*search*/

					printf("\nSearching the record ....\n\n");

					rec_found=0;
					e=buff[index+2];
					while(e!='\0')
					{
						e=buff[index+2];
						sstr[index]=e;
						index++;
					}
					sstr[index]='\0';
					index=0;
					regnum=atoi(sstr);			

					for(count = 0; count <=size; count++)
					{
						if(regnum == records[count].regno)
						{
							rec_found=1;
							break;
						}

					}

					/*if found then send '1' to the client
					to indicate that file present else send '0'*/

					if(rec_found == 0)
					{
						buff[0] = '0';
					}
					else
					{
						buff[0] = '1';
					}

					sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
					if( sntb == -1 )
					{
						printf("\nSending failed!:(\n");
						close(s);
						exit(0);
					}

					if(rec_found != 0)
					{

						int rn[1];
						/* send registration number */

						rn[0] = records[count].regno;
						sntb = sendto(s, rn, sizeof(rn), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}

						char send_str[25];
						
						/*send name*/

						strcpy(send_str,records[count].name);
						printf("%s\n",send_str);
						
						sntb = sendto(s, send_str, sizeof(send_str), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}
						
						/*send course*/

						strcpy(send_str,records[count].course);
						printf("%s\n",send_str);
						
						sntb = sendto(s, send_str, sizeof(send_str), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}
						
						/*send dob*/

						strcpy(send_str,records[count].DOB);
						printf("%s\n",send_str);
						
						sntb = sendto(s, send_str, sizeof(send_str), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}
					}

					printf("\nResults sent!\n");

					break;

			case '3':
					/*delete*/

					printf("\n\nTrying to delete....\n\n");

					/*Search the 'records' array to check if the 
					registration number recieved is already present*/

					rec_found=0;
					e=buff[index+2];
					while(e!='\0')
					{
						e=buff[index+2];
						sstr[index]=e;
						index++;
					}
					sstr[index]='\0';
					index=0;
					regnum=atoi(sstr);			

					for(count = 0; count <=size; count++)
					{
						if(regnum == records[count].regno)
						{
							rec_found=1;
							break;
						}
					}

					/* if record is not found send '0'
					indicating that record wasn't found 
					else delete the record and send '1'*/

					if(rec_found == 0)
					{
						buff[0] = '0';

						sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}

					}
					else
					{
						buff[0] = '1';
						for (b = count; b < size; b++)
						{
							records[b].regno = records[b+1].regno;
							strcpy(records[b].name, records[b+1].name);
							strcpy(records[b].course, records[b+1].course);
							strcpy(records[b].DOB, records[b+1].DOB);
						}
						sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
						if( sntb == -1 )
						{
							printf("\nSending failed!:(\n");
							close(s);
							exit(0);
						}

						size--;
					}
					printf("\nResults sent!\n");
					
					break;

			case '4':
					/*display*/

					printf("\nRecords sent to display!\n\n");

					/*send 'records' array*/
					sntb = sendto(s, records, sizeof(records), 0, (struct sockaddr*)&client, len);
					if( sntb == -1 )
					{
						printf("\nSending failed!:(\n");
						close(s);
						exit(0);
					}


					break;

			case '5':
					/*exit*/
					printf("\nServer is trying to terminate ...\n....\n");
					strcpy(buff, "exit");

					sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
					if( sntb == -1 )
					{
						printf("\nSending failed!:(\n");
						close(s);
						exit(0);
					}

					break;
			default:
					printf("\nPlease choose valid options!ಠ_ಠ\n");
					break;
		} //end of switch

		if(strcmp(buff,"exit")==0)
		{
			break;
		}

	}//end of while(1)

	close(s);
	printf("Done!\n");
}//end of main

/*fill_five will store 5 records as soon as the code is run fresh*/

void fill_five(struct Student *records)
{
	records[0].regno = 1;
	records[1].regno = 2;
	records[2].regno = 3;
	records[3].regno = 4;
	records[4].regno = 5;

	strcpy(records[0].name, "keren swanson");
	strcpy(records[1].name, "khoa ngyuen");
	strcpy(records[2].name, "sutton rain");
	strcpy(records[3].name, "jackson khai");
	strcpy(records[4].name, "landon kal");

	strcpy(records[0].course, "ICT1001");
	strcpy(records[1].course, "CSE1001");
	strcpy(records[2].course, "MECH001");
	strcpy(records[3].course, "PMT1201");
	strcpy(records[4].course, "ECE2001");

	strcpy(records[0].DOB, "12/11/2000");
	strcpy(records[1].DOB, "02/01/1999");
	strcpy(records[2].DOB, "14/12/1998");
	strcpy(records[3].DOB, "13/09/1990");
	strcpy(records[4].DOB, "20/04/2002");	
}