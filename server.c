/* NAME : Karishma Chauhan ; 
   ROLL NO. : MT2022056 
  
################################ ---CONCURRENT SERVER (using fork)--- ####################################
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "./serverACTIONS/adminServer_actions.h"
#include "./serverACTIONS/customerServer_actions.h"
#include "dbStructures.h"

void main()
{
//********************************** SOCKET CREATION **********************************//
	int sockfd_server = socket(AF_INET,SOCK_STREAM,0);  //create socket
	if(sockfd_server==-1)
	{
		perror("ERROR OCCURED IN SOCKET CREATION ");
		printf("Exiting.......\n");
		_exit(0);
	}

//**************************** SOCKET ADDRESS INITIALIZATION ****************************//
	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	printf("Enter the port no. to establish the connection: ");
	int port;
	scanf("%d",&port);
	server_addr.sin_port = htons(port); 		 // Server will listen to this port
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	

//******************** BIND() - ASSOCIATING LOCAL ADDRESS TO SOCKET ********************//
	int bindstatus = bind(sockfd_server,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if (bindstatus == -1)
        {
       		 perror("ERROR OCCURED IN SOCKET BINDING ");
       		 printf("Exiting.......\n");
		_exit(0);
    	}

//*************************** LISTEN() - opening passive socket ***************************//
	int listenstatus = listen(sockfd_server,12);  //converts an unconnected socket into a passive socket
	 if (listenstatus == -1)
    	{
        	perror("ERROR OCCURED WHILE OPENING PASSIVE SOCKET");
        	close(sockfd_server);
        	printf("Exiting.......\n");
        	_exit(0);
    	}
    	
//*************************** ACCEPTING CLIENT CONNECTION AND HANDLING REQUESTS ***************************//
	int connected_client=0;  		// Stores the number of connected clients
	while(1)
	{
		printf("**** SERVER WAITING FOR CLIENT %d ****\n",connected_client+1);

	//***************** ACCEPT() call - waiting/accepting client connection requests*****************//		
		struct sockaddr_in client_addr;// contains clients address info
		int C_addrlen = sizeof(client_addr);
		
		int client_fd = accept(sockfd_server,(struct sockaddr *)&client_addr,(socklen_t *)&C_addrlen);
		if (client_fd == -1)
        	{
            		perror("ERROR OCCURED WHILE CONNECTING TO CLIENT");
            		close(sockfd_server);
            		printf("Exiting.......\n");
        	}
        	
        //***************** CONNECTION ESTABLISHED WITH CLIENT *****************//	
        	else
		{
			connected_client++;
			printf("**** CONNECTION ESTABLISHED WITH CLIENT %d****\n",connected_client);
			int userType;
			int pid = fork();
			if(pid==0)						//CHILD WILL HANDLE REQUEST OF THIS CLIENT
			{
				while(1)
			
				{	read(client_fd, &userType, sizeof(userType));           	//read S1
				
				
					if(userType==1)
						adminlogin(userType,client_fd,connected_client);		//calls User defined function <login_handler> defined in "Server_actions.h" to take appropriate action as per client request
					else if(userType==2 )
						customerlogin(userType,client_fd,connected_client);	
					else
					{	printf("Client %d Incorrect input...\n",connected_client);
						break;
					}
						
				}	
				
				getchar();
				printf("Exiting Client %d\n",connected_client);
				close(client_fd);
				
				_exit(0);

			}
			else if(pid==-1)                           
			{
				perror("FORK ERROR");
				printf("Exiting .......\n");
				_exit(0);
			}
		
			else
				//Parent will handle other clients requests
				close(client_fd);
		}	

	}
	
}
