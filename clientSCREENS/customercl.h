#ifndef CUSTOMER_FUNCTIONS
#define CUSTOMER_FUNCTIONS
#include <stdbool.h>
#include "graphics.h"
#include "dbStructures.h"
static int customer_action=-1;
void login(int USER_TYPE,int client_fd);
void view_accountdetails(int connFD);
void deposit(int connFD);
void withdraw(int connFD);
void findbalance(int connFD);
void open_passbook(int connFD);
void changepassword(int connFD);
//void add_account(int connFD);
//void search_customer(int connFD);
//void modify_customer(int connFD);
//void search_account(int connFD);
//void delete_account(int connFD);
void customermenu(int connFD);

void customer_handler(int USER_TYPE,int client_fd)
{	
	
		login(USER_TYPE,client_fd);     //1: Defined below
		
 	
}

//-----------LOGIN FUNCTION--------------------------------//
void login(int USER_TYPE,int client_fd){
	int login_status;
	
	
	boxify();
	
 	printHL(30,3,"-",25);
 	gotoxy(28,4);
 	printf("<<< Welcome Customer >>>\n");
 	printHL(30,5,"-",25);
 	
 	gotoxy(15,9);
 	
 	char UID[20],PWD[20];
 	printf("Please enter your USER Id and Password and hit Enter key---> \n");
 	gotoxy(20,11);
 	printf("---> USER ID	:\t");
 	scanf("%s",UID);
 	gotoxy(20,13);
 	printf("---> PASSWORD	:\t");
 	scanf("%s",PWD);
 	
 	
 	//gotoxy(45,11);
 	//printf("\n");
 
 	//gotoxy(45,13);
 	
 	getchar();
 	//sending tp customerlogin function in customerServer_actions.h
 	write(client_fd, UID, sizeof(UID));   
	write(client_fd, PWD, sizeof(PWD));
	read(client_fd, &login_status, sizeof(login_status));
	gotoxy(5,15);
	if(login_status==1)    			//login successfull
	{	printf("LOGGING IN ...... HIT ENTER TO LOGIN\n"); 
		getchar();
		customermenu(client_fd);
		return;
		
	}
	else if (login_status==110)		//credentials used are of different user
		printf("Credentials used are of different user......\n");
	else if (login_status==10)		//UID found but password not match
		printf("UID found but password not match......\n");
	else if (login_status==0)               //UID not found
		printf("Please check UID not found....\n");
	else if (login_status==1110)               //UID not found
		printf("The Account associated with Customer ID %s  could not be found..\n| Kindly check and retry\n",UID);
	else if (login_status==11110) 
		printf("Found the Account of %s customer,\n|		But this Account has been deactivated...\n\n|	YOU NO LONGER HAVE ACCESS TO THIS ACCOUNT..EXITING..\n\n",UID); 
	
	gotoxy(5,23);
	printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->\n");
	getchar();
	getchar();
	
	return;
}

void customermenu(int connFD)
{
	while(1)
	{
		boxify();
		
		printHL(30,3,"-",25);
 		gotoxy(25,4);
 		printf("<<< Welcome Dear Customer >>>\n");
 		printHL(30,5,"-",25);
 		gotoxy(15,9);
 	
 		printf("IIIT BANK CUSTOMER SERVICES MENU ---> \n");
 	
 		gotoxy(25,11);
 		printf("---> VIEW ACCOUNT DETAILS	[1]\n");
 		gotoxy(25,12);
 		printf("---> DEPOSIT MONEY		[2]\n");
 		gotoxy(25,13);
 		printf("---> WITHDRAW MONEY		[3]\n");
 		gotoxy(25,14);
 		printf("---> ACCOUNT BALANCE ENQUIRY	[4]\n");
 		gotoxy(25,15);
 		printf("---> OPEN PASSBOOK 		[5]\n");
 		gotoxy(25,16);
 		printf("---> PASSWORD CHANGE		[6]\n");
 		gotoxy(25,17);
 		printf("---> LOGOUT			[7]\n");
 		
 		gotoxy(10,23);
 		printf("INFO: Select the required action (1-6) or Press 7 to Exit. \n");
 		gotoxy(55,9);
 		
 		
 		scanf("%d",&customer_action);
 		
 		gotoxy(10,20);
 		switch (customer_action)
            	{
            		case 1:	// VIEW ACCOUNT DETAILS
            			printf("You have selected option 1 : VIEW ACCOUNT DETAILS\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &customer_action, sizeof(customer_action));
                		view_accountdetails(connFD);
                		break;
           		case 2:
                		// DEPOSIT MONEY
                		printf("You have selected option 2 : DEPOSIT MONEY\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &customer_action, sizeof(customer_action));
                		deposit(connFD);
                		break;
         
                	case 3:
                		// WITHDRAW MONEY
                		printf("You have selected option 3 : WITHDRAW MONEY\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &customer_action, sizeof(customer_action));
                		withdraw(connFD);
                		break;
                	case 4:
                		// ACCOUNT BALANCE ENQUIRY	
                		printf("You have selected option 4 : ACCOUNT BALANCE ENQUIRY\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &customer_action, sizeof(customer_action));
                		findbalance(connFD);
                		break;
                	case 5:
                		// OPEN PASSBOOK	
                		printf("You have selected option 5 : OPEN PASSBOOK\n| HIT ENTER To Continue......\n");
            			getchar();
            			getchar();
            			write(connFD, &customer_action, sizeof(customer_action));
                		open_passbook(connFD);
                		break;
                	
                	case 6:
                		// PASSWORD CHANGE	
                		printf("You have selected option 6 : PASSWORD CHANGE\n| HIT ENTER to continue......\n");
                		getchar();
            			getchar();
            			write(connFD, &customer_action, sizeof(customer_action));
                		changepassword(connFD);
                		break;
                		
                	case 7:
                		//LOG OUT
                		printf("You have selected option 7 : LOGOUT\n|	LOGGING OUT press 1 to stay login or any other value to LOGOUT...... ");
                		int ret;
				scanf("%d",&ret);
				if(ret==1)
				{	continue;
							
				}
            			else
                		{
                			write(connFD, &customer_action, sizeof(customer_action));					//W2 S4
                			gotoxy(5,22);
					printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->>\n");
					getchar();
					getchar();
					return;
					
					
				}
                	
            		default:	
            				printf("INCORRECT INPUT \n|	LOGGING OUT...... \n");
                			gotoxy(5,22);
                		
                			customer_action=7;    //forced logout
                			write(connFD, &customer_action, sizeof(customer_action));					//W2 S4
					printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->>\n");
					getchar();
					getchar();
					return;
					
		}			
            
		
	}	
}


void view_accountdetails(int connFD)
{
	boxify();
	struct CustomerDetails custdata;
	struct AccountDetails accdata;
	
	read(connFD, &custdata, sizeof(custdata));
	
		printHL(30,3,"-",25);
 		gotoxy(20,4);
 		printf("<<<  BELOW ARE YOUR ACCOUNT DETAILS >>>\n");
 		printHL(30,5,"-",25);
 		//gotoxy(5,9);
 		gotoxy(8,8);
		printf("PERSONAL DETAILS  ---> \n");
		
		printHL(55,9,"-",8);
 		printf("\n|	USERNAME					: %s\n",custdata.cred.Uname); 
		printf("|	USER ID FOR LOG IN				: %s\n",custdata.cred.UID);  
		printf("|	GENDER						: %s\n",custdata.gender); 
		printf("|	AGE						: %d\n",custdata.age);
	
	
	read(connFD, &accdata, sizeof(accdata));
 	
 		gotoxy(8,15);
		printf("ACCOUNT DETAILS  ---> \n");
		printHL(55,16,"-",8);
 		printf("\n|	ACCOUNT NO.					: %d\n",accdata.AccountNo);
		printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",accdata.account_type);
		printf("|	USER NO. of Account holder 1			: %d\n",accdata.UserNo[0]); 
		if(accdata.UserNo[1]!=-1)
 			printf("|	USER NO. of Account holder 2			: %d\n",accdata.UserNo[1]);  
		printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)		: %c\n",accdata.status); 
		printf("|	CURRENT BALANCE					: %ld\n",accdata.balance); 
		
	
		
 			
 			gotoxy(5,24);
			printf("<<---------Press enter to return to the menu ------------->>\t");
			
 			getchar();
 			getchar();
 			return;

 		
 		
}

void deposit(int connFD)
{
	boxify();
	struct CustomerDetails custdata;
	struct AccountDetails accdata;
	int depositstatus;
			read(connFD, &custdata, sizeof(custdata));
	
			read(connFD, &accdata, sizeof(accdata));
		
			printHL(30,3,"-",25);
			gotoxy(25,4);
			printf("<<<  MONEY DEPOSIT PAGE >>>\n");
			printHL(30,5,"-",25);
			//gotoxy(5,9);
			gotoxy(8,8);
			printf("Below are your account details --> \n");
			printf("\n|	ACCOUNT NO.					: %d\n",accdata.AccountNo);
			printf("|	USERNAME					: %s\n",custdata.cred.Uname); 
			printf("|	USER ID FOR LOG IN				: %s\n",custdata.cred.UID);  
			printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",accdata.account_type);
			printf("|	USER NO. of Account holder 1			: %d\n",accdata.UserNo[0]); 
			if(accdata.UserNo[1]!=-1)
				printf("|	USER NO. of Account holder 2			: %d\n",accdata.UserNo[1]);  
			printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)		: %c\n",accdata.status); 
			printf("|	CURRENT BALANCE					: %ld\n",accdata.balance); 
			printf("\n| Do you wish to deposit money to ACCOUNT NO. %d?(yes-[1]/no-[0]\t",accdata.AccountNo);
			//printf("\n\n| ENTER 1 TO CONTINUE or ANYOTHER KEY TO RETURN TO PREVIOUS MENU \n");
			int confirm;
			scanf("%d",&confirm);
			write(connFD, &confirm, sizeof(confirm));
			long int depositamount=0;
			if(confirm==1)
			{	int retry;
				for(retry=1;retry<4;retry++)
				{
					printf("\n|	Enter the amount you want to deposit:\t");
					scanf("%ld",&depositamount);
					printf("\n|	You want to deposit %ld? Press 1 to confirm\n|	Press 0 to re-enter amount(%d try left)\t",depositamount,3-retry);
					int confirm;
					scanf("%d",&confirm);
					//write(connFD, &confirm, sizeof(confirm));
					if(confirm==1 && depositamount>0)
					{	write(connFD, &depositamount, sizeof(depositamount));
						read(connFD, &depositstatus, sizeof(depositstatus));
						if(depositstatus>0)
						{	read(connFD, &accdata, sizeof(accdata));
							printf("\n|	%ld Rs. DEPOSITED SUCCESSFULLY TO YOUR ACCOUNT NO. %d\n",depositamount,accdata.AccountNo);
							printf("\n|	The updated Account balance is --> %ld Rs. \n\n",accdata.balance);
							getchar();
							break;
							
						}
						else
							printf("|	We apologize the Money deposit failed, please try again\n");
						
					}
					else if(confirm!=1)
						break;
						
					else
						printf("\n|  OOPS.. Deposit amount can not be negative, retry ..\n");
						
					
					
				}
				if(retry==4)
					printf("\n|  OOPS.. You exceeded the no. of retries , Returning >>>>\n");
				
				
			}
			else 
			{	printf("\n|	NO Confirmation , MONEY DEPOSIT ABORTED RETURNING TO MENU ......\n");
				
	
			}
			
			gotoxy(5,24);
			printf("<<---------Press hit enter to return to the menu page ------------->>\t");
			
 			getchar();
 			getchar();
 			return;
	
		
 		
}

void withdraw(int connFD)
{
	boxify();
	struct CustomerDetails custdata;
	struct AccountDetails accdata;
	int withdrawstatus;
	 
			read(connFD, &custdata, sizeof(custdata));
	
			read(connFD, &accdata, sizeof(accdata));
		
			printHL(30,3,"-",25);
			gotoxy(25,4);
			printf("<<<  MONEY WITHDRAW PAGE >>>\n");
			printHL(30,5,"-",25);
			//gotoxy(5,9);
			gotoxy(8,8);
			printf("Below are your account details --> \n");
			printf("\n|	ACCOUNT NO.					: %d\n",accdata.AccountNo);
			printf("|	USERNAME					: %s\n",custdata.cred.Uname); 
			printf("|	USER ID FOR LOG IN				: %s\n",custdata.cred.UID);  
			printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",accdata.account_type);
			printf("|	USER NO. of Account holder 1			: %d\n",accdata.UserNo[0]); 
			if(accdata.UserNo[1]!=-1)
				printf("|	USER NO. of Account holder 2			: %d\n",accdata.UserNo[1]);  
			printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)		: %c\n",accdata.status); 
			printf("|	CURRENT BALANCE					: %ld\n",accdata.balance); 
			printf("\n| Do you wish to withdraw money from ACCOUNT NO. %d?(yes-[1]/no-[0]\t",accdata.AccountNo);
			//printf("\n\n| ENTER 1 TO CONTINUE or ANYOTHER KEY TO RETURN TO PREVIOUS MENU \n");
			int confirm;
			scanf("%d",&confirm);
			long int withdraw=0;
			write(connFD, &confirm, sizeof(confirm));
			if(confirm==1)
			{	int retry;
				for(retry=1;retry<4;retry++)
				{
					printf("\n|	Enter the amount you want to withdraw:\t");
					scanf("%ld",&withdraw);
					if(withdraw<=accdata.balance && withdraw>0)
					{
						printf("\n|	You want to withdraw %ld? Your new balance will be: %ld\n",withdraw,accdata.balance-withdraw);
						printf("|	Press 1 to confirm\n|	Press 0 to re-enter amount(%d try left)\t",3-retry);
					}
					else if(withdraw>0)
					{	
						printf("\n|	OOPS.. YOU DON'T HAVE ENOUGH ACCOUNT BALANCE FOR THIS WITHDRAWL: %ld Rs.",accdata.balance);
						printf("|	Press 0 to re-enter amount(%d try left), else press anyother value to goback\t",3-retry);
					}
					else 
					{	
						printf("\n|	OOPS.. INVALID WITHDRAWL AMOUNT, please retry\n");
						printf("|	Press 0 to re-enter amount(%d try left), else press anyother value to goback\t",3-retry);
					}
					
					int confirm;
					scanf("%d",&confirm);
					if(confirm==1)
					{	write(connFD, &withdraw, sizeof(withdraw));
						read(connFD, &withdrawstatus, sizeof(withdrawstatus));
						if(withdrawstatus>0)
						{
							read(connFD, &accdata, sizeof(accdata));
							printf("\n|	%ld Rs. WITHRAWN SUCCESSFULLY FROM YOUR ACCOUNT NO. %d\n",withdraw,accdata.AccountNo);
							printf("\n|	The updated Account balance is --> %ld Rs. \n\n",accdata.balance);
							break;
						}
						
						else
							printf("|	We apologize the Money withdraw failed, please try again\n");
						
					}
					else if(confirm>1)
						break;
					
				}
				if(retry==4)
					printf("\n|  OOPS.. You exceeded the no. of retries , returning to menu....\n");
				
				
			}
			else 
			{	printf("\n|	No Confirmation received , aborting the withdrawl process...\n");
				
	
			}
			gotoxy(5,24);
			printf("<<--------- Press hit enter to return to the menu page ------------->>\t");
			
 			getchar();
 			getchar();
 			return;
 		
}

void findbalance(int connFD)
{
	boxify();
	struct CustomerDetails custdata;
	struct AccountDetails accdata;
	
	read(connFD, &custdata, sizeof(custdata));
	
		printHL(30,3,"-",25);
 		gotoxy(20,4);
 		printf("<<<  BELOW ARE YOUR ACCOUNT DETAILS MR./MS. %s>>>\n",custdata.cred.Uname);
 		printHL(30,5,"-",25);
 		//gotoxy(5,9);
 		gotoxy(0,8);
		//printf("PERSONAL DETAILS  ---> \n");
		
		//printHL(55,9,"-",8);
 		//printf("\n|	USERNAME					: %s\n",custdata.cred.Uname); 
		printf("|	USER ID FOR LOG IN				: %s\n",custdata.cred.UID);  
		//printf("|	GENDER						: %s\n",custdata.gender); 
		//printf("|	AGE						: %d\n",custdata.age);
	
	
		read(connFD, &accdata, sizeof(accdata));
 	
 		//gotoxy(8,15);
		//printf("ACCOUNT DETAILS  ---> \n");
		//printHL(55,16,"-",8);
 		printf("|	ACCOUNT NO.					: %d\n",accdata.AccountNo);
 		printf("|	CURRENT BALANCE					: %ld\n",accdata.balance); 
		printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",accdata.account_type);
		//printf("|	USER NO. of Account holder 1			: %d\n",accdata.UserNo[0]); 
		//if(accdata.UserNo[1]!=-1)
 		//	printf("|	USER NO. of Account holder 2			: %d\n",accdata.UserNo[1]);  
		//printf("|	ACCOUNT STATUS(A-Active/D-Deactivated)		: %c\n",accdata.status); 
		
		
	
		
 			gotoxy(5,24);
			printf("<<--------- Press hit enter to return to the menu page ------------->>\t");
			
 			getchar();
 			getchar();
 			return;
 		}
 		


void open_passbook(int connFD)
{	
	boxify();
	struct CustomerDetails custdata;
	struct AccountDetails accdata;
	struct Transactions passbooktxn;
	
	read(connFD, &custdata, sizeof(custdata));
	read(connFD, &accdata, sizeof(accdata));
	int txnfound=0,txncount=0;
	printHL(30,3,"-",25);
 	gotoxy(20,4);
 	printf("<<<  PASSBOOK OF MR/MS %s >>>\n",custdata.cred.Uname);
 	printHL(30,5,"-",25);
 	//gotoxy(5,9);
 	gotoxy(8,8);
	printf("ACCOUNT DETAILS  ---> \n");
		
	printHL(55,9,"-",8);
 	printf("\n|	USERNAME					: %s\n",custdata.cred.Uname); 
	printf("|	USER ID FOR LOG IN				: %s\n",custdata.cred.UID);  
	printf("|	ACCOUNT NO.					: %d\n",accdata.AccountNo);
	printf("|	ACCOUNT TYPE(1-Single user/2-Joint Account	: %d\n",accdata.account_type);
	printf("|	CURRENT BALANCE					: %ld\n",accdata.balance); 
	
  //----Fetching transaction details-------------------------------
  	gotoxy(8,17);
	printf("Below is the transaction history  ---> \n");
	printHL(75,18,"-",2);	
	read(connFD, &txncount, sizeof(txncount));  
	printf("\n|   TIME_of_TXN\t     TXN_TYPE   BALANCE_BEFORE   TXN_AMOUNT   BALANCE_AFTER\t");
	printHL(75,20,"-",2);
	if(txncount>0)
		txnfound++;
	while(txncount==txnfound)
	{	//printf("\n txnfound %d txncount %d\n",txnfound,txncount);
		read(connFD, &passbooktxn, sizeof(passbooktxn));  
		//perror("read1 status");
		read(connFD, &txncount, sizeof(txncount));  
		//perror("read2 status");
		//printf(" 524 txncount %d, txnfound %d\n",txncount, txnfound);
		struct tm tm = *localtime(&passbooktxn.transactionTime);
		printf("\n| %d-%d-%d %d:%d:%d\t",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
		printf(" %c\t    %ld\t   %ld\t\t%ld       \t",passbooktxn.TxnType,passbooktxn.Balanceb4,passbooktxn.txnamount,passbooktxn.Balanceafter);
		//printf(" 528 txncount %d, txnfound %d\n",txncount, txnfound);
		//printf("\n| %s\t%d\t%ld\t%ld\t%ld\t",ctime(&passbooktxn.transactionTime),passbooktxn.TxnType,passbooktxn.Balanceb4,passbooktxn.txnamount,passbooktxn.Balanceafter);
		
    //printf("%d-%d-%d %d:%d:%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
		txnfound++;
		//printf(" 533 txncount %d, txnfound %d\n",txncount, txnfound);
	}
	
	printf("\n\n\n|   PASSBOOK PRINTING COMPLETED....\n"); 

	
			
			printf("\n\n| <<--------- Press hit enter to return to the menu page ------------->>\t");
			
 			getchar();
 			getchar();
 			return;
	
}

void changepassword(int connFD)
{
	boxify();
	struct CustomerDetails custdata;
	read(connFD, &custdata, sizeof(custdata));
	char currentpwd[20],newpwd[2][20];
	
	printHL(30,3,"-",25);
 	gotoxy(20,4);
 	printf("<<<  PASSWORD CHANGE PAGE >>>\n");
 	printHL(30,5,"-",25);
 	gotoxy(8,8);
	printf("MR/MS %s BELOW ARE YOUR ACCOUNT DETAILS  ---> \n",custdata.cred.Uname);
	printHL(55,9,"-",8);
 	printf("\n|	USERNAME					: %s\n",custdata.cred.Uname); 
	printf("|	USER ID FOR LOG IN				: %s\n",custdata.cred.UID);  
	printf("|	ACCOUNT NO.					: %d\n",custdata.AccountNo);
	
	printf("|  PLEASE CONFIRM YOU CURRENT PASSWORD :\t");
	scanf("%s",currentpwd);
	write(connFD, &currentpwd, sizeof(currentpwd));
	int match=0;
	read(connFD, &match, sizeof(match));
	if(match)
	{
		gotoxy(15,15);
		printf("---> NEW PASSWORD(20char length)	:\t");
 		
 		scanf("%s",newpwd[0]);
 		gotoxy(15,16);
 		printf("---> RENTER NEW PASSWORD		:\t");
 		scanf("%s",newpwd[1]);
 		int match2=0;
 		if(strcmp(newpwd[0],newpwd[1])==0)
 		{	match2=1;
 			write(connFD, &match2, sizeof(match2));
 			int writestatus=-1;
 			write(connFD, &newpwd[0], sizeof(newpwd[0]));
 			read(connFD, &writestatus, sizeof(writestatus));
 			if(writestatus!=-1)
 			{   printf("|	YOUR PASSWORD HAS BEEN UPDATED SUCCESSFULLY,\n|	  LOGGING OUT.. LOGIN WITH NEW PASSWORD\n");
 				
 				gotoxy(5,22);
                		
                			customer_action=7;    //forced logout
                			write(connFD, &customer_action, sizeof(customer_action));					//W2 S4
					printf("<<---------THANK YOU FOR VISITING IIITB BANK OF INDIA [-.-]------------->>\n");
					getchar();
					getchar();
					_exit(0);
 				/*printf("\n\n| <<--------- Press hit enter to return to the menu page ------------->>\t");
			
 				getchar();
 				getchar();
 				return;*/
	
 			
 			}
 			
 		}
 		else
 		{	printf("| OOPS !! ENTERED PASSWORDS DONOT MATCH, PLEASE CHECK  AND RETRY. \n");
 			printf("\n\n| <<--------- Press hit enter to return to the menu page ------------->>\t");
			
 			getchar();
 			getchar();
 			return;
	
		 	
		}
 	}
 	else
 		{	printf("| OOPS !! ENTERED PASSWORD IS INCORRECT, PLEASE CHECK  AND RETRY. \n");
 			printf("\n\n| <<--------- Press hit enter to return to the menu page ------------->>\t");
			
 			getchar();
 			getchar();
 			return;
	
		 	
		}
}
	
	
	
		
	



#endif
