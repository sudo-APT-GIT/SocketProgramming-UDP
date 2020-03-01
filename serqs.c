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
	int r, s, recb, sntb, x;
	int ca;
	char port[5];
	int c, d, swap;

	char b[MAXSIZE];
	int a=0;

	printf("Enter the port number: ");
	gets(port);
	sscanf(port, "%d", &x); //converting string "port" to integer "x"
	
	socklen_t len;
	struct sockaddr_in server, client;
	int buff[MAXSIZE], n, z;

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

	recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, &ca);
	if( recb == -1 )
	{
		printf("\nMessage recieving failed!:((\n");
		close(s);
		exit(0);
	}

	printf("Array Recieved!\n");
	printf("Sorting the array...\n");

	quick_sort(buff,1,buff[0]);
	
	sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
	if( sntb == -1 )
	{
		printf("\nSending failed!:(\n");
		close(s);
		exit(0);
	}

	printf("Result sent!\n");
	

	close(s);

}//end of main
void quick_sort(int a[],int l,int u)
{
	/*int index;
	for(index=1; index<=u+1; index++)
	{
		printf("%d\t", a[index]);
		a[index]=1;
	}
	printf("done!\n");*/
	int j;
	if(l<u)
	{
		j=partition(a,l,u);
		quick_sort(a,l,j-1);
		quick_sort(a,j+1,u);
	}
}

int partition(int a[],int l,int u)
{
	int v,i,j,temp;
	v=a[l];
	i=l;
	j=u+1;
	
	do
	{
		do
			i++;
			
		while(a[i]<v&&i<=u);
		
		do
			j--;
		while(v<a[j]);
		
		if(i<j)
		{
			temp=a[i];
			a[i]=a[j];
			a[j]=temp;
		}
	}while(i<j);
	
	a[l]=a[j];
	a[j]=v;
	
	return(j);
}