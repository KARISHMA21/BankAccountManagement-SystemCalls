/* NAME : Karishma Chauhan ; 
   ROLL NO. : MT2022056 
   
######################### ---CLIENT--- ########################
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>

#include "./clientSCREENS/WELCOME.h"

void main()
{
//********************************** SOCKET CREATION **********************************//
	int sockfd_client; //file descriptor that refers to endpoint

	sockfd_client = socket(AF_INET,SOCK_STREAM,0);		//create an endpoint for communication. SOCK_STREAM for TCP. Protocol value for IP =0
						      		// AF_INET for IPv4
	if(sockfd_client==-1)
	{
		perror("ERROR OCCURED IN SOCKET CREATION ");		
		printf("Exiting.......\n");
		_exit(0);
	}									   

//**************************** SOCKET ADDRESS INITIALIZATION ****************************//
	//initiate the connection on a socket using connect()
	struct sockaddr_in server_addr; 			/*Each socket domain requires its own address format.
								In the AF_INET domain, the address is specified using a structure called sockaddr_in(man 7 ip)*/

	server_addr.sin_family = AF_INET;
	printf("Enter the port no. to establish the connection: ");
	int port;
	scanf("%d",&port);
	server_addr.sin_port = htons(port);			//convert values b/w host and network byte order
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	//converts Internet host address into netwrk byte order


//*************************** CONNECT() - sending connection request to server ***************************//
	printf("Establishing connection with server.........\n");
	int ret = connect(sockfd_client,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(ret==-1)
	{
		perror("CONNECTION FAILED");
		_exit(0);
	}
	else
	//***************** CONNECTION ESTABLISHED WITH CLIENT *****************//
		printf("****CONNECTION ESTABLISHED****\n");
	//	printf("HIT ENTER \n");
	//getchar();
	getchar();
	//int runapp=1;
	//while(runapp)
	 WELCOME_screen(sockfd_client);
	
		
	
	
	
	close(sockfd_client);
	printf("\nRELEASING CONNECTION TO SERVER......BYE!!\n");
	_exit(0);
}
