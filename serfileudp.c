#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXSIZE 50

int main()
{
	int r, s, recb, sntb, x, index=0, a=0;
	int ca;
	socklen_t len;
	struct sockaddr_in server, client;
	char str[MAXSIZE], e, sstr[MAXSIZE], filename[MAXSIZE], port[5],rstr[MAXSIZE];
	char reorderstr[MAXSIZE][MAXSIZE], temp[MAXSIZE], buff[MAXSIZE];
	char bye[]="bye";
	char *pch;
	int num=1;


	printf("Enter the port number: ");
	gets(port);
	sscanf(port, "%d", &x); //converting string "port" to integer "x"
	
	

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

		printf("\nThe file name is: ");	
		puts(buff);
		printf("\n");

		/* check if the file exists */

		strcpy(filename, buff);
		FILE *fp = fopen(buff, "r");
		if(fp==NULL)
		{
			buff[0]='9';
			printf("\nError 404: File not found!¯\\_(ツ)_/¯\nPlease enter valid file names\n\n");
			sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
			if( sntb == -1 )
			{
				printf("\nSending failed!:(\n");
				close(s);
				exit(0);
			}
			printf("\nResult sent!\n");
			break;
		}
		else
		{
			printf("\nFile found!(^o^)\n");
			buff[0]='1';
		}

		fclose(fp);

		sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
		if( sntb == -1 )
		{
			printf("\nSending failed!:(\n");
			close(s);
			exit(0);
		}
 		/* wait for the client to send the choice and string */

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

					/*get the search string and store it in sstr[]*/

						e=buff[index+2];
						while(e!='\0')
						{
							e=buff[index+2];
							sstr[index]=e;
							index++;
						}
						sstr[index]='\0';
						index=0;

						printf("\nServer is searching for \"%s\" in the file %s....\n",sstr, filename);

								/*open the file in read mode and check*/

						FILE *f = fopen(filename, "r");

						int c = getc(f),z=0, flag=0;
						while (c != EOF) {

						if(c!=10) //10 is the ascii value of endline
						{
							str[z]=(char)c;
							z++;
							c = getc(f);
							continue;
						}
						str[z]='\0';
						z=0;

						if(!strcmp(sstr,str))
						{
							flag=1;
							break;
						}

						c = getc(f);
					}

					if(flag==1)
						buff[0]='1';
					else
						buff[0]='-';
					
					fclose(f);

					printf("\nResults sent!\n");

					break;

				case '2':

					/*get the search string and store it in sstr[] and replace string in rstr[]*/
					pch=strtok(buff," ");
					num=1;
					while(pch!=NULL)
					{
						if(num==1)
						{
							num++;
							pch=strtok(NULL, " ");
							continue;
						}
						else if(num==2)
						{
							num++;
							strcpy(sstr,pch);
						}
						else if(num==3)
						{
							num++;
							strcpy(rstr, pch);
						}
						pch=strtok(NULL, " ");
					}
					
					printf("\nServer is replacing ...\n");


					FILE *fr = fopen(filename, "r");
					FILE *fwone = fopen("aftereplace.txt", "w");

					int c1 = getc(fr),z1=0, flag1=0;
					while (c1 != EOF) {

						if(c1!=10) //10 is the ascii value of endline
						{
							str[z1]=(char)c1;
							z1++;
							c1 = getc(fr);
							continue;
						}
						str[z1]='\0';
						z1=0;

						if(!strcmp(sstr,str))
						{
							fputs(rstr,fwone);
							flag1=1;
						}
						else
						{
							fputs(str,fwone);
						}
						fputs("\n",fwone);
						c1 = getc(fr);
					}

					if(flag1==1)
						buff[0]='1';
					else
						buff[0]='-';
					
					fclose(fr);
					fclose(fwone);

					/* store new files contents after replacement (aftereplace.txt)
					into thee original file */
					
					FILE *fwtwo = fopen(filename,"w");
					FILE *frtwo = fopen("aftereplace.txt", "r");
					c1=getc(frtwo);
					z1=0;
					while(c1!=EOF)
					{
						if(c1!=10) //10 is the ascii value of endline
						{
							str[z1]=(char)c1;
							z1++;
							c1 = getc(frtwo);
							continue;
						}
						str[z1]='\0';
						z1=0;
						fputs(str, fwtwo);
						fputs("\n", fwtwo);
						c1 = getc(frtwo);
					}
					fclose(fwtwo);
					fclose(frtwo);
					printf("\nResults sent!\n");
					break;

				case '3':

					printf("\nServer is trying to reorder the contents of %s ...", filename);
					FILE *fro = fopen(filename, "r");
					int cro = getc(fro),i=0, j=0, count=0;

					while (cro != EOF) 
					{
						if(cro!='\n')
						{
							reorderstr[i][j]=(char)cro;
							j++;
							cro = getc(fro);
							continue;
						}
						reorderstr[i][j]='\0';
						i++;
						j=0;
						count++;
						cro = getc(fro);
					}

					for(i = 0; i < count; i++)
					{
						for(j = i+1; j <= count; j++)
						{
							if(strcmp(reorderstr[i],reorderstr[j])>0)
							{
								strcpy(temp,reorderstr[i]);
								strcpy(reorderstr[i],reorderstr[j]);
								strcpy(reorderstr[j],temp);
							}
						}
					}

					fclose(fro);

					FILE *fw=fopen(filename, "w");
					for(i=0; i<=count; i++)
					{
						fputs(reorderstr[i], fw);
						fputs("\n",fw);
					}
					fclose(fw);
					buff[0]='1';
					printf("\nResults saved!\n");
					break;

				case '4':

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

				}
				if(strcmp(buff,"exit")==0)
					break;
				sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
				if( sntb == -1 )
				{
					printf("\nSending failed!:(\n");
					close(s);
					exit(0);
				}

	}//end of while(1)

	close(s);

}//end of main
