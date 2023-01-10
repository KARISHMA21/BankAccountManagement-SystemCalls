#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS
#include <stdbool.h>
//#include "./singlecl.h"
//#include "./defaultcl.h"
#include "graphics.h"
#include "dbStructures.h"
static int admin_action=-1;
void adminlogin(int USER_TYPE,int client_fd);
void add_account(int connFD);
void search_customer(int connFD);
void modify_customer(int connFD);
void search_account(int connFD);
void delete_account(int connFD);
void menu(int connFD);

void admin_handler(int USER_TYPE,int client_fd)
{	
	adminlogin(USER_TYPE,client_fd);     //1: Defined below
	
 	

}
void adminlogin(int USER_TYPE,int client_fd){
	int login_status;

	
		
	
	 
	
	boxify();		//defined in graphics.h
	
 	
 	//gotoxy(27,5);
 	printHL(30,3,"-",25);
 	gotoxy(30,4);
 	printf("<<< Welcome ADMIN >>>\n");
 	printHL(30,5,"-",25);
 	gotoxy(15,9);
 	
 	char AID[20],PWD[20];
 	printf("Please you Admin Id and Password and hit Enter key---> \n");
 	gotoxy(20,11);
 	printf("---> ADMIN ID	:\t");
 	scanf("%s",AID);
 	gotoxy(20,13);
 	printf("---> PASSWORD	:\t");
 	scanf("%s",PWD);
 	
 	
 	//gotoxy(45,11);
 	//printf("\n");
 
 	//gotoxy(45,13);
 	
 	getchar();
 	write(client_fd, AID, sizeof(AID));                		 //W2 S2
	write(client_fd, PWD, sizeof(PWD));				 //W2 S3
	read(client_fd, &login_status, sizeof(login_status));		 //R  C1
	gotoxy(15,15);
	if(login_status==1)    			//login successfull
	{	printf("LOGGING IN ...... HIT ENTER TO LOGIN\n"); 
		getchar();
		menu(client_fd);
		return ;
		
		
	}
	else if (login_status==110)		//credentials used are of different user
		printf("Credentials used are of different user!! EXITING......\n");
	else if (login_status==10)		//UID found but password not match
		printf("UID found but password not match!! EXITING......\n");
	else if (login_status==0)               //UID not found
		printf("Please check UID not found!! EXITING....\n");
	gotoxy(5,23);
	printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->\n");
	getchar();
	getchar();
	
	return;
}

void menu(int connFD)
{
	while(1)
	{	boxify();
		
		printHL(30,3,"-",25);
 		gotoxy(30,4);
 		printf("<<< Welcome ADMIN >>>\n");
 		printHL(30,5,"-",25);
 		gotoxy(15,8);
 	
 		printf("ADMIN MENU ---> \t");
 	
 		gotoxy(25,11);
 		printf("---> OPEN A NEW ACCOUNT		[1]\n");
 		gotoxy(25,12);
 		printf("---> SEARCH CUSTOMER DETAILS	[2]\n");
 		gotoxy(25,13);
 		printf("---> MODIFY CUSTOMER DETAILS	[3]\n");
 		gotoxy(25,14);
 		printf("---> SEARCH ACCOUNT DETAILS	[4]\n");
 		gotoxy(25,15);
 		printf("---> DELETE AN ACCOUNT		[5]\n");
 		gotoxy(25,16);
 		printf("---> LOGOUT			[6]\n");
 		
 		gotoxy(10,23);
 		printf("INFO: Select the required action (1-5) or Press 6 to Logout. \n\n");
 		gotoxy(35,8);
 		
 		//int admin_action;
 		scanf("%d",&admin_action);
 		
 		gotoxy(10,20);
 		switch (admin_action)
            	{
            		case 1:	// CREATE NEW ACCOUNT
            			printf("You have selected option 1 : OPEN A NEW ACCOUNT\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &admin_action, sizeof(admin_action));						//W2 S4
                		add_account(connFD);
                		break;
           		case 2:
                		// SEARCH CUSTOMER DETAILS
                		printf("You have selected option 2 : SEARCH CUSTOMER DETAILS\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &admin_action, sizeof(admin_action));						//W2 S4
                		search_customer(connFD);
                		break;
         
                	case 3:
                		// MODIFY CUSTOMER DETAILS
                		printf("You have selected option 3 : MODIFY CUSTOMER DETAILS\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &admin_action, sizeof(admin_action));						//W2 S4
                		modify_customer(connFD);
                		break;
                	case 4:
                		// SEARCH ACCOUNT DETAILS	
                		printf("You have selected option 4 : SEARCH ACCOUNT DETAILS\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &admin_action, sizeof(admin_action));						//W2 S4
                		search_account(connFD);
                		break;
                	case 5:
                		// DELETE AN ACCOUNT	
                		printf("You have selected option 5 : DELETE AN ACCOUNT\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &admin_action, sizeof(admin_action));						//W2 S4
                		delete_account(connFD);
                		break;
                	
                	case 6:
                		// LOGOUT	
                		printf("You have selected option 6 : LOGOUT\n|	LOGGING OUT press 1 to stay login or any other value to LOGOUT...... ");
                		int ret;
				scanf("%d",&ret);
				if(ret==1)
				{	continue;
							
				}
            			else
                		{
                			write(connFD, &admin_action, sizeof(admin_action));					//W2 S4
                			gotoxy(5,22);
					printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->\n");
					getchar();
					getchar();
					return;
					//write(connFD, &admin_action, sizeof(admin_action));
					
				}
				
                		
                		
                	
            		default:
            			  printf("INCORRECT INPUT \n|	LOGGING OUT...... \n");
                			gotoxy(5,22);
                			admin_action=6;    //forced logout
                			write(connFD, &admin_action, sizeof(admin_action));					//W2 S4
					printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->\n");
					getchar();
					getchar();
					return;
                			
            }	
	}	
}

void add_account(int connFD)
{	
	boxify();
	//admin_action=-1;
	printHL(30,3,"-",25);
 	gotoxy(15,4);
 	printf("<<< ENTER BELOW DETAILS TO ADD A NEW ACCOUNT >>>\n");
 	printHL(30,5,"-",25);
 	gotoxy(15,9);
 	
 	printf("SELECT THE ACCOUNT TYPE(1/2) ---> \n");
 	gotoxy(25,11);
 	printf("---> SINGLE USER ACCOUNT	[1]\n");
 	gotoxy(25,12);
 	printf("---> JOINT USER ACCOUNT		[2]\n");
 	gotoxy(8,23);
 	printf("INFO: Select the account type (1/2) or Press any other key to go back \n");
 	gotoxy(50,9);
 		
 	int account_type;
 	scanf("%d",&account_type);
 	
 	if(account_type==1)
 	{	int confirm=0;
 		int allusercreated=0;
 		char PWD1[20],PWD2[20];
    		char Uname[25];
    		char gender[10]; // M -> Male, F -> Female, O -> Other
    		int age;
    		
 		printf("You have selected to create a single user account, Hit enter to continue...\n");
 		getchar();
 		write(connFD, &account_type, sizeof(account_type));					//W2 s5
 		boxify();
 		struct CustomerDetails Normalacc;
 		printHL(30,3,"-",25);
 		gotoxy(15,4);
 		printf("<<< ENTER BELOW DETAILS TO ADD A NEW ACCOUNT >>>\n");
 		printHL(30,5,"-",25);
 		//gotoxy(15,9);
 		gotoxy(15,11);
 		printf("---> NAME(<= 25 char)		:\t");
 		scanf("%s",Uname);
 		gotoxy(15,13);
 		printf("---> GENDER			:\t");
 		scanf("%s",gender);
 		//getchar();
 		gotoxy(15,15);
 		printf("---> AGE(>=18)			:\t");
 		scanf("%d",&age);
 		if(age<18)
 		{	
 			
 			printf("|	INVALID ENTRY : Age should be more that 18 for major accounts,\n|	Returning back to menu...\n");
 			getchar();
 			confirm=0;
 			write(connFD, &confirm, sizeof(confirm));				//W2 s6
 			getchar();
 			//menu(connFD);
			return;
 		}
 		gotoxy(15,17);
 		printf("---> PASSWORD(20char length)	:\t");
 		//getchar();
 		/*int i;
 		for(i=0;i<8;i++)
		 {
 			char ch = getchar();
  			PWD1[i] = ch;
  			ch = '*' ;
  			printf("%c",ch);
  			fflush();
  			
 		}
 		PWD1[i] = ' ';*/
 		scanf("%s",PWD1);
 		gotoxy(15,19);
 		printf("---> RENTER PASSWORD		:\t");
 		
 		scanf("%s",PWD2);
 		if(strcmp(PWD1,PWD2)==0)
 		{	printf("\n| Thank you for sharing the details, creating new account.....\n| Please check the entered detailes below and HIT 1 TO CONTINUE else return.....\n");
 			printf("| The USERNAME is %s\n",Uname);  
			printf("| The PASSWORD is %s\n",PWD1); 
			printf("| The GENDER is %s\n",gender); 
			printf("| The AGE is %d\n",age); 
 			printf("| Please confirm ---> \t");
 			scanf("%d",&confirm);
 			//getchar();
 			//confirm=getchar();
 			write(connFD, &confirm, sizeof(confirm));				//W2 s6
 			if(confirm==1)
 			{
 				write(connFD, Uname, sizeof(Uname));				//W2 s7
 				write(connFD, gender, sizeof(gender));				//W2 s8
 				write(connFD, &age, sizeof(age));				//W2 s9
 				write(connFD, PWD1, sizeof(PWD1));				//W2 s10
 				
 				read(connFD, &Normalacc, sizeof(Normalacc));			//R  C2
 				
 			//int admin_action=6;
 				boxify();
 				printHL(30,3,"-",25);
 				gotoxy(15,4);
 				printf("CONGRATULATION !!! ACCOUNT HAS BEEN CREATED SUCCESSFULLY\n");
 				printHL(30,5,"-",25);
 				gotoxy(5,9);
 	
 				printf("Please note below USER ID, Account no. etc. for future reference ---> \n");
 				printf("|	USERNAME					: %s\n",Normalacc.cred.Uname); 
				//printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",Normalacc.cred.USERTYPE); 
				//printf("|	ACCOUNT NO.					: %d\n",Normalacc.account); 
				printf("|	USER ID FOR LOG IN				: %s\n",Normalacc.cred.UID); 
				printf("|	PASSWORD					: %s\n",Normalacc.cred.PWD); 
				//printf("|	The status is %c\n",Nacc.cred.status);
				printf("|	GENDER						: %s\n",Normalacc.gender); 
				printf("|	AGE						: %d\n",Normalacc.age); 
				
				allusercreated=1;
 	  			write(connFD, &allusercreated, sizeof(allusercreated));
				struct AccountDetails newAccount;
 				read(connFD, &newAccount, sizeof(newAccount));			//R  C3
 			//	admin_action=-1;
 			
 			//Below are the ACCOUNT Details ---> 
 				printf("|	ACCOUNT NO.					: %d\n",newAccount.AccountNo); 
 				printf("|	USER NO. of Account holder 			: %d\n",newAccount.UserNo[0]); 
 				//printf("|	USER NO. of Account holder 2			: %d\n",newAccount.UserNo[1]); 
				printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",newAccount.account_type); 
		
				printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)		: %c\n",newAccount.status); 
				printf("|	CURRENT BALANCE					: %ld\n",newAccount.balance); 
				
				printf("\n\n|	Press enter to return to admin menu\t");
 				getchar();
 				getchar();
 				return;
				
			}
			else
			{	printf("|	Aborting the account add process, returning to the MENU\n");
 				getchar();
				return;
			}
		
 		}
 		else
 		{	printf("| CAUTION: Entered passwords don't match, returning back to menu\n");
 			getchar();
 			confirm=0;
 			write(connFD, &confirm, sizeof(confirm));				//W2 s6
 			getchar();
 			//menu(connFD);
			return;
 		}
		
 		
 		
 	}	
 	else if(account_type==2)
 	{	int confirm=0;
 		int allusercreated=0;
 	    printf("You have selected to create a Joint user account, Hit enter to continue...\n");
 	    getchar();
 	    write(connFD, &account_type, sizeof(account_type));					//W2 s5
 	  	int usercreated=0;
 	    for(int user=1;user<3;user++)
 	    {	
 	    	char PWD1[20],PWD2[20];
    		char Uname[25];
    		char gender[10]; // M -> Male, F -> Female, O -> Other
    		int age;
    		
 		
 		boxify();
 		struct CustomerDetails Normalacc;
 		printHL(30,3,"-",25);
 		gotoxy(15,4);
 		printf("<<< ENTER BELOW DETAILS OF CUSTOMER [%d] TO CREATE ACCOUNT >>>\n",user);
 		printHL(30,5,"-",25);
 		//gotoxy(15,9);
 		gotoxy(15,11);
 		printf("---> NAME(<= 25 char)		:\t");
 		scanf("%s",Uname);
 		gotoxy(15,13);
 		printf("---> GENDER			:\t");
 		scanf("%s",gender);
 		//getchar();
 		gotoxy(15,15);
 		printf("---> AGE(>=18)			:\t");
 		scanf("%d",&age);
 		if(age<18)
 		{	
 			
 			printf("|	INVALID ENTRY : Age should be more that 18 for major accounts,\n|	Returning back to menu...\n");
 			getchar();
 			confirm=0;
 			write(connFD, &confirm, sizeof(confirm));				//W2 s6
 			getchar();
 			//menu(connFD);
			return;
 		}
 		gotoxy(15,17);
 		printf("---> PASSWORD(20char length)	:\t");
 		scanf("%s",PWD1);
 		gotoxy(15,19);
 		printf("---> RENTER PASSWORD		:\t");
 		scanf("%s",PWD2);
 		if(strcmp(PWD1,PWD2)==0)
 		{	printf("| Thank you for sharing the details,.....\n| Please confirm the entered details below and HIT 1 TO CONTINUE.....\n");
 			printf("| The USERNAME is %s\n",Uname);  
			printf("| The PASSWORD is %s\n",PWD1); 
			printf("| The GENDER is %s\n",gender); 
			printf("| The AGE is %d\n",age);  
			printf("| Please confirm ---> \t");
 			//getchar();
 			//getchar();
 			//int flag=1;
    			//printf("296-%d\n",flag++);
    			//getchar();
    			//confirm=getchar();
 			scanf("%d",&confirm);
 			write(connFD, &confirm, sizeof(confirm));			//W2 s6
 			if(confirm==1)
 			{	usercreated++;
 				write(connFD, Uname, sizeof(Uname));
 				write(connFD, gender, sizeof(gender));
 				write(connFD, &age, sizeof(age));
 				write(connFD, PWD1, sizeof(PWD1));
 				//printf("301-%d\n",flag++);
 			//getchar();
 				int status =read(connFD, &Normalacc, sizeof(Normalacc));  //R  C3
 				perror("read status");
 				if(status != -1)
 				{
 					//printf("303-%d\n",flag++);
 					//getchar();
 					getchar();
 					boxify();
 					printHL(30,3,"-",25);
 					gotoxy(2,4);
 					printf("  CONGRATULATION !!! USER PROFILE FOR CUSTOMER %d CREATED SUCCESSFULLY\n",user);
 					printHL(30,5,"-",25);
 					gotoxy(5,9);
 		
 					printf("Please note below USER ID, Account no. etc. for future reference ---> \n");
 					printf("|	USERNAME					: %s\n",Normalacc.cred.Uname); 
					printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",Normalacc.cred.USERTYPE); 
					//printf("|	ACCOUNT NO.					: %d\n",Normalacc.account); 
					printf("|	USER ID FOR LOG IN				: %s\n",Normalacc.cred.UID); 
					printf("|	PASSWORD					: %s\n",Normalacc.cred.PWD); 
					//printf("|	The status is %c\n",Nacc.cred.status);
					printf("|	GENDER						: %s\n",Normalacc.gender); 
					printf("|	AGE						: %d\n",Normalacc.age); 
					sleep(1);
					if(user==1)
						printf("|  Hit ENTER to create user profile of second user of joint account ---> \n");
						getchar();
					confirm=0;
				}
				else
				{
					printf("|	Aborting the account add process, returning to the MENU\n");
					getchar();
					return;
				}
			}
			else 
			{	
				printf("|	Okay, aborting the account add process, returning to the MENU\n");
				getchar();	
				
				//menu(connFD);
				return;
			}
 		}
 		else
 		{	printf("| CAUTION: Entered passwords don't match, Hit ENTER to return back to MENU\n");
 			getchar();
 			confirm=0;
 			write(connFD, &confirm, sizeof(confirm));
 			getchar();
 			//menu(connFD);
			return;
 		
 		}
 	  }  //loop for 2 user details input
 	
 	  	if(usercreated==2)
 	  	{
 	  		allusercreated=1;
 	  		write(connFD, &allusercreated, sizeof(allusercreated));
 	  		boxify();
 			printHL(30,3,"-",25);
 			gotoxy(8,4);
 			printf("	GOOD JOB!! ACCOUNT HAS BEEN CREATED SUCCESSFULLY\n");
 			printHL(30,5,"-",25);
 			gotoxy(5,9);
 			struct AccountDetails newAccount;
 			read(connFD, &newAccount, sizeof(newAccount));
 			admin_action=-1;
 			printf("Below are the ACCOUNT Details ---> \n");
 			printf("|	ACCOUNT NO.					: %d\n",newAccount.AccountNo); 
 			printf("|	USER NO. of Account holder 1			: %d\n",newAccount.UserNo[0]); 
 			printf("|	USER NO. of Account holder 2			: %d\n",newAccount.UserNo[1]); 
			printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",newAccount.account_type); 
			//printf("|	ACCOUNT NO.					: %d\n",Normalacc.account); 
			printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)		: %c\n",newAccount.status); 
			printf("|	CURRENT BALANCE					: %ld\n",newAccount.balance); 
			//printf("|	The status is %c\n",Nacc.cred.status);
			
			printf("\n\n|	Press enter to return back to admin menu\t");
 		
 				getchar();
 				getchar();
 				return;
		}
		else
			
		
 		printf("\n\n|	Press enter to return back to admin menu\t");
 		
 		getchar();
 		getchar();
 		return;
 			
       }
 	else
 	{	write(connFD, &account_type, sizeof(account_type));					//W2 s5
 		gotoxy(5,20);
 		printf("INCORRECT INPUT  \n|	Returning to admin menu...... ");
 		
                return;
	}
 	//gotoxy(5,20);
	//return true;
	
}

void search_customer(int connFD)
{
	boxify();
	struct CustomerDetails Normalacc;
	bool Customerfound; 
	printHL(30,3,"-",25);
 	gotoxy(20,4);
 	printf("<<<  CUSTOMER DETAILS SEARCH PAGE >>>\n");
 	printHL(30,5,"-",25);
 	gotoxy(5,9);
 	
 	printf("ENTER CUSTOMER'S USERID TO SEARCH DETAILS ---> \n");
 	char UserID[20];
 	gotoxy(52,9);
 	scanf("%s",UserID);
 	write(connFD, UserID, sizeof(UserID));
 	
 	read(connFD, &Customerfound, sizeof(Customerfound));
 	if(Customerfound)
 	{
 		
 		read(connFD, &Normalacc, sizeof(Normalacc));
 		
 	
 		gotoxy(15,12);
		printf("BELOW ARE THE DETAILS OF ENTERED USERID ---> \n");
 		printf("|	USERNAME					: %s\n",Normalacc.cred.Uname); 
		printf("|	ACCOUNT TYPE(2-Single user/3-Joint Account	: %d\n",Normalacc.cred.USERTYPE); 
		printf("|	ACCOUNT NO.					: %d\n",Normalacc.AccountNo); 
		printf("|	USER ID FOR LOG IN				: %s\n",Normalacc.cred.UID);  
		//printf("|	ACCOUNT STATUS(A-Active/D-Deactivated		: %c\n",Normalacc.cred.status);
		printf("|	GENDER						: %s\n",Normalacc.gender); 
		printf("|	AGE						: %d\n",Normalacc.age); 
		
	}
	else
	{
		gotoxy(8,12);
		printf("OOPS !! ENTERED CUSTOMER DETAILS NOT FOUND, PLEASE CHECK ID AND RETRY. \n");
	}
	printf("\n\n|	Press enter to return back to admin menu\t");
 		
 				getchar();
 				getchar();
 				return;
}

void modify_customer(int connFD)
{
	boxify();
	struct CustomerDetails Normalacc;
	bool Customerfound; 
	printHL(30,3,"-",25);
 	gotoxy(20,4);
 	printf("<<<  CUSTOMER DETAILS MODIFICATION PAGE >>>\n");
 	printHL(30,5,"-",25);
 	gotoxy(5,9);
 	
 	printf("ENTER CUSTOMER'S USERID TO MODIFY PERSONAL DETAILS ---> \n");
 	char UserID[20];
 	gotoxy(62,9);
 	scanf("%s",UserID);
 	write(connFD, UserID, sizeof(UserID));
 	
 	read(connFD, &Customerfound, sizeof(Customerfound));
 	if(Customerfound)
 	{
 		read(connFD, &Normalacc, sizeof(Normalacc));
 		
 	
 		gotoxy(8,12);
		printf("Select one of the below details which you want to modify ---> \n");
 		printf("|	[1] USERNAME		: %s\n",Normalacc.cred.Uname); 
		printf("|	[2] GENDER		: %s\n",Normalacc.gender); 
		printf("|	[3] AGE			: %d\n",Normalacc.age); 
		gotoxy(70,12);
		int modify;
 		scanf("%d",&modify);
 		write(connFD, &modify, sizeof(modify));
 		gotoxy(0,18);
 		switch (modify)
            	{
            		case 1:	// MODIFY USERNAME
            			printf("| You have selected option 1 : To modify Customer name\n| HIT ENTER To Continue......\n");
            			char newname[20];
            			getchar();
            			getchar();
            			printf("|	Enter the New Name -->\t");
            			scanf("%s",newname);
            			write(connFD, newname, sizeof(newname));
            			read(connFD, &Normalacc, sizeof(Normalacc));
                		break;
                		
           		case 2: // MODIFY GENDER
                		printf("| You have selected option 2 : To modify Customer GENDER\n| HIT ENTER To Continue......\n");
            			char newgender[10];
            			getchar();
            			getchar();
            			printf("|	Enter the New Gender -->\t");
            			scanf("%s",newgender);
            			write(connFD, newgender, sizeof(newgender));
            			read(connFD, &Normalacc, sizeof(Normalacc));
            			break;
         
                	case 3: // MODIFY AGE
                		printf("| You have selected option 3 : To modify Customer AGE\n| HIT ENTER To Continue......\n");
            			int newage;
            			getchar();
            			getchar();
            			printf("|	Enter the New Age -->\t");
            			scanf("%d",&newage);
            			write(connFD, &newage, sizeof(newage));
            			read(connFD, &Normalacc, sizeof(Normalacc));
            			break;
                	
            		default:	// Exit
            			printf("OOPS you selected an incorrect option\n");
                		break;
          	  }	

 			gotoxy(0,24);
 			printf("| Details have been updated successfully.....\n");
 			gotoxy(0,26);
 	
 			printf("|    Below are Customer Details after updation ---> \n");
 			printf("|	USERNAME					: %s\n",Normalacc.cred.Uname); 
			printf("|	ACCOUNT TYPE(2-Single user/3-Joint Account	: %d\n",Normalacc.cred.USERTYPE); 
			//printf("|	ACCOUNT NO.					: %d\n",Normalacc.account); 
			printf("|	USER ID FOR LOG IN				: %s\n",Normalacc.cred.UID); 
			//printf("|	ACCOUNT STATUS					: %c\n",Normalacc.cred.status);
			printf("|	GENDER						: %s\n",Normalacc.gender); 
			printf("|	AGE						: %d\n",Normalacc.age); 
 		
	}
	else
	{
		gotoxy(15,12);
		printf("OOPS !! ENTERED CUSTOMER DETAILS NOT FOUND, PLEASE CHECK ID AND RETRY. \n");
	}
	printf("\n\n|	Press enter to return back to admin menu\t");
 		
 				getchar();
 				getchar();
 				return;
}

void search_account(int connFD)
{	
	boxify();
	struct AccountDetails account;
	bool accountfound; 
	printHL(30,3,"-",25);
 	gotoxy(20,4);
 	printf("<<<  ACCOUNT DETAILS SEARCH PAGE >>>\n");
 	printHL(30,5,"-",25);
 	gotoxy(5,9);
 	
 	printf("ENTER ACCOUNT NO. TO SEARCH DETAILS ---> \n");
 	int accountno;
 	gotoxy(52,9);
 	scanf("%d",&accountno);
 	write(connFD, &accountno, sizeof(accountno));
 	
 	read(connFD, &accountfound, sizeof(accountfound));
 	if(accountfound)
 	{
 		
 		read(connFD, &account, sizeof(account));
 		
 	
 		printf("|   Below are the ACCOUNT Details ---> \n");
 		printf("|	ACCOUNT NO.						: %d\n",account.AccountNo); 
 		printf("|	USER NO. of Account holder 1				: %d\n",account.UserNo[0]); 
 		printf("|	USER NO. of Account holder 2(-1 if single user account)	: %d\n",account.UserNo[1]); 
		printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account		: %d\n",account.account_type); 
		printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)			: %c\n",account.status); 
		printf("|	CURRENT BALANCE						: %ld\n",account.balance); 
		 
		
	}
	else
	{
		gotoxy(5,12);
		printf("OOPS !! ENTERED ACCOUNT DETAILS NOT FOUND,\n|	Hit ENTER to return back to MENU\n");
 			getchar();
 			getchar();
 			
	}
	printf("\n\n|	Press enter to return back to admin menu\t");
 		
 				getchar();
 				getchar();
 				return;
}

void delete_account(int connFD)
{
	boxify();
	struct AccountDetails account,tempacc;
	bool accountfound; 
	printHL(30,3,"-",25);
 	gotoxy(20,4);
 	printf("<<<  ACCOUNT DELETE PAGE >>>\n");
 	printHL(30,5,"-",25);
 	gotoxy(5,9);
 	
 	printf("ENTER THE ACCOUNT NO.TO BE DELETED ---> \n");
 	int accountno;
 	gotoxy(52,9);
 	scanf("%d",&accountno);
 	write(connFD, &accountno, sizeof(accountno));
 	
 	read(connFD, &accountfound, sizeof(accountfound));
 	
 	if(accountfound)
 	{	
 		
 		read(connFD, &account, sizeof(account));
 		if(account.balance==0)
 		{
 	
 			printf("|   Below are the details of Account to be deleted ---> \n");
 			printf("|	ACCOUNT NO.						: %d\n",account.AccountNo); 
 			printf("|	USER NO. of Account holder 1				: %d\n",account.UserNo[0]); 
 			printf("|	USER NO. of Account holder 2(-1 if single user account)	: %d\n",account.UserNo[1]); 
			printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account		: %d\n",account.account_type); 
			printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)			: %c\n",account.status); 
			printf("|	CURRENT BALANCE						: %ld\n",account.balance); 
			printf("\n\n|  PLEASE PRESS [0] TO CONFIRM THAT YOU REALLY WANT TO DELETE ABOVE ACCOUNT!! \n");
			printf("| Please confirm ---> \t");
			char confirm='1';
			getchar();
			confirm=getchar();
			write(connFD, &confirm, sizeof(confirm));
			if(confirm=='0')
			{ 	account.status='D';
				write(connFD, &account, sizeof(account));
				read(connFD, &tempacc, sizeof(tempacc));
				printf("|	UPDATED ACCOUNT STATUS(A-Active/D-Deactivated)	: %c\n",tempacc.status); 
				printf("|    Account no. %d deactivated successfully !! \n",accountno); 
	
			}
		
			else
			{	printf("|  ACCOUNT DELETION ABORTED !! REDIRECTING TO ADMIN MENU- HIT ENTER\n");
		 		getchar();
		 		getchar();
 				return;
			}
		}
		else
		{
			printf("|  ACCOUNT BALANCE IS NOT NULL ACCOUNT DELETION ABORTED !!\n|   REDIRECTING TO ADMIN MENU- HIT ENTER\n");
		 	getchar();
		 	getchar();
 			return;
		}
	}
	else
	{
		gotoxy(15,12);
		printf("OOPS !! ENTERED ACCOUNT DETAILS NOT FOUND, Hit ENTER to return back to MENU\n");
 			getchar();
 			getchar();
 			return;
	}
	printf("\n\n|	Press enter to return back to admin menu\t");
 		
 				getchar();
 				getchar();
 				return;	
}


#endif
