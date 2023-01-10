#ifndef WELCOME_screen1
#define WELCOME_screen1
#include "admincl.h"
#include "customercl.h"

#include "graphics.h"

bool WELCOME_request_handler(int USER_TYPE,int client_fd);
void WELCOME_screen(int sockfd_client)
{ 
	while(1)
	{
		system("clear");
		//printf("%c",bar);
		//printf("%*s", "=");
		char *heading="| ONLINE BANKING MANAGEMENT SYSTEM |";
		
		printVL(0,30,"|",0);
		printVL(80,30,"|",0);
		gotoxy(0,0);
		printHL(22,0,"=",0);
		gotoxy(23,0);
			printf("%s",heading);
		printHL(22,0,"=",0);
		//printL(0,20,'|');
		//gotoxy(16,0);
		//gotoxy(0,20);
		//for(int x=0;x<50;x++)
		//	printf("%d",x);
		
		//gotoxy(27,5);
		printHL(30,3,"-",25);
		gotoxy(27,4);
		printf("<<< IIITB BANK OF INDIA >>>\n");
		printHL(30,5,"-",25);
		gotoxy(25,9);
		
		printf("Please Select the Account Type---> \n");
		
		gotoxy(25,11);
		printf("---> ADMINISTRATOR		[1]\n");
		gotoxy(25,12);
		printf("---> CUSTOMER			[2]\n");
		//gotoxy(25,13);
		
		
		gotoxy(7,23);
		printf("INFO: Enter Your Account type(1/2) or Press any other value to Exit. \n");
		printHL(80,25,"=",0);
		
		gotoxy(60,9);
		int USER_TYPE;
		scanf("%d",&USER_TYPE);
		write(sockfd_client, &USER_TYPE, sizeof(USER_TYPE));         //write2 S1
		bool exitflag=false;
		exitflag= WELCOME_request_handler(USER_TYPE,sockfd_client);
		if(exitflag)
			return;
		
	}	
	
////////printf("===================== | ONLINE BANKING MANAGEMENT SYSTEM | =====================\n");
 	//printf("||                                                                            ||\n");
}

bool WELCOME_request_handler(int USER_TYPE,int client_fd)
{
		
           switch (USER_TYPE)
            {
		case 1:	// Admin
			admin_handler(USER_TYPE, client_fd);
			return false;			
			break;
            	case 2:
			// Normal Customer
			customer_handler(USER_TYPE, client_fd);
			return false;
			break;
           	
           	default:	 // Exit
			gotoxy(5,23);
			printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->\n");
			return true;
            }
				
}

#endif
