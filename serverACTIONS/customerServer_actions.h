#ifndef USERServer_FUNCTIONS
#define USERServer_FUNCTIONS
#include "dbStructures.h"
//#include<stdlib.h>
#include <string.h>
//#include<unistd.h>
#include<fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semIdentifier_a;
struct CustomerDetails customerdata;
struct AccountDetails accountdata;
struct sembuf p = { 0, -1, SEM_UNDO};//down operation -->lock
struct sembuf v = { 0, +1, SEM_UNDO};//up operation --> unlock
void customeraction(int connfd,int UserNo, int accountno);

void normalaction(int connfd);
void jointaction(int connfd);
void initsemaphore(struct CustomerDetails customerdata);
static int j=0;

struct CustomerDetails readcustomerdata(int connfd,int UserNo)
{
	int cfd=open("CustomerDetailsDB.txt",O_RDONLY);
    	if(cfd!=-1)
	{
		int offset = lseek(cfd,UserNo*sizeof(customerdata),SEEK_SET);
		struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(customerdata), getpid()};

    		int lStatus = fcntl(cfd, F_SETLKW, &lock);
    		if (lStatus == -1)
    		{
        		perror("Read lock could not be applied on CustomerDetailsDB.txt");
        		_exit(0);
    		}
		read(cfd,&customerdata,sizeof(customerdata));
		lock.l_type = F_UNLCK;
		fcntl(cfd, F_SETLK, &lock);
		perror("42 customer data release F_RDLCK status");
		return customerdata;
	}
	else
	{	perror("CustomerDetailsDB.txt open failed");
		_exit(0);
	}
}
struct AccountDetails readaccdata(int connfd,int accountno)
{	
	//printf(" accountno inside readaccdata %d",accountno);
	int acfd=open("AccountsDB.txt",O_RDONLY);
	//perror("53 readaccdata open status");
    	if(acfd!=-1)
	{
		int offset = lseek(acfd,accountno*sizeof(accountdata),SEEK_SET);
		struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(accountdata), getpid()};

    		int lStatus = fcntl(acfd, F_SETLKW, &lock);
    		if (lStatus == -1)
    		{
        		perror("Read lock could not be applied on AccountsDB.txt");
        		_exit(0);
    		}
		read(acfd,&accountdata,sizeof(accountdata));
		//perror("66 readaccdata read status");
		//printf(" 67 accountno inside readaccdata %d",accountdata.AccountNo);
		lock.l_type = F_UNLCK;
		fcntl(acfd, F_SETLK, &lock);
		//perror("70 release F_RDLCK status");
		return accountdata;
	}
	else
	{	perror("AccountsDB.txt open failed");
		_exit(0);
	}
}
void customerlogin(int userType, int connfd,int clientno)
{ 
	
	
	char temp_ID[20], temp_PWD[20];
	int loginstatus=0;
	read(connfd, temp_ID, sizeof(temp_ID));
	read(connfd, temp_PWD, sizeof(temp_PWD));
	
	int loginfd=open("CustomerDetailsDB.txt",O_RDONLY);
    	if(loginfd==-1)
        {
        	perror("Unable to open file");
        	printf("Exiting....\n");
        	_exit(0);
	}
	lseek(loginfd,0,SEEK_SET); 
    	while(read(loginfd,&customerdata,sizeof(customerdata)))
	{	//printf("customerdata.cred.UID : %s\n",customerdata.cred.UID);
		//printf("temp_ID: %s\n",temp_ID);
		char encryptedpassword[20];
        	if(strcmp(customerdata.cred.UID,temp_ID)==0)
        	{ 	strcpy(encryptedpassword, crypt(temp_PWD, "056"));
        		if(strcmp(customerdata.cred.PWD,encryptedpassword)==0) 
        		{	if(customerdata.cred.USERTYPE==userType)
        			{	//finding associated account details for this customer
        				bool accountfound=false; 
					//printf("48 Custmore user no %d\n",customerdata.cred.UserNo);
					int accfd=open("AccountsDB.txt",O_RDONLY);
					if(accfd==-1)
        				{
        					perror("Unable to open file");
        					printf("Exiting....\n");
        					_exit(0);
					}
					while(read(accfd,&accountdata,sizeof(accountdata)))
					{	//printf("57 account user no 1 %d\n",accountdata.UserNo[0]);
					//	printf("58 account user no 2 %d\n",accountdata.UserNo[1]);
						
        					if(customerdata.cred.UserNo==accountdata.UserNo[0] || (customerdata.cred.UserNo==accountdata.UserNo[1] && accountdata.UserNo[1]!=-1 )) //Customer found in records
        					{	accountfound=true;
        					
        						printf("Found the Account of %s customer(%d/%d)...\n",temp_ID,accountdata.UserNo[0],accountdata.UserNo[1]);
        						if(accountdata.status=='A')
        						{	
        							loginstatus= 1;                //login successfull
        							
        							break;
        						}
        						else
        						{ 	
        							loginstatus=11110 ;
        						  	printf("OOPS!! But the Account of this customer was deactivated\n");
        						  	break;
        						}
        						
        					}
        						
        		
        				}
        				if(loginstatus==0)
        				{	printf("The Account associated with Customer ID %s  could not be found..Kindly check and retry\n",temp_ID);
        					loginstatus=1110;
        				}
        				break; //when UID and PWD Match CustomerDetailsDB.txt search stopped
        			}	
        			else
        			{	loginstatus= 110;		//credentials used are of different user
        				break;
        			}
        		}
        		else
        		{	loginstatus= 10;		//UID found but password not match
        			break;
        		}
        		
        	}
        	
        }
        if(loginstatus==0)
        	printf("UID %s NOT FOUND IN DATABASE..Kindly check and retry\n",temp_ID);
        
        
        write(connfd, &loginstatus, sizeof(loginstatus));		//sending login status back to client  
        //close(loginfd);  
        if(loginstatus==1)
	{	printf("Login Sucessful\n");
			//printf("159 Custmore user no %d\n",customerdata.cred.UserNo);
			//printf("160 account  no %d\n",accountdata.AccountNo);
			customeraction(connfd,customerdata.cred.UserNo,accountdata.AccountNo);
		
	}
	else
		printf("Invalid username or password\n");
}

void customeraction(int connfd,int UserNo, int accountno)
{
	int cust_action;
	initsemaphore(customerdata);
	
	 
   	 
	while(1)   //STAY IN WHILE TILL LOGOUT 
	{
		//printf(" THe I is %d: \n",++i);
		read(connfd, &cust_action, sizeof(cust_action));
		//printf(" THe cust_action is %d: \n",cust_action);
		
		
		// VIEW ACCOUNT DETAILS
		if(cust_action==1)
		{	
			//printf("159 Custmore user no %d\n",customerdata.cred.UserNo);
			//printf("160 account  no %d\n",accountdata.AccountNo);
			customerdata=readcustomerdata(connfd,UserNo);
			accountdata=readaccdata(connfd,accountno);
			
			
			write(connfd, &customerdata, sizeof(customerdata));
			write(connfd, &accountdata, sizeof(accountdata));
        		
		}// VIEW ACCOUNT DETAILS CLOSE
		
		// DEPOSIT MONEY
		else if(cust_action==2)
		{	long int depositamount=0;
		
			int semopStatus = semop(semIdentifier_a, &p, 1);
			if (semopStatus == -1)
    			{
        			perror("Error while locking critical section");
        			
        			return;
    			}
    			customerdata=readcustomerdata(connfd,UserNo);
			accountdata=readaccdata(connfd,accountno);
			int writecustdata=write(connfd, &customerdata, sizeof(customerdata));
			if(writecustdata==-1)
			{	
			 	perror("Sending customer details to client failed");
			 	semop(semIdentifier_a,&v,1); //release lock
			 	return;
			}
			int writeaccdata=write(connfd, &accountdata, sizeof(accountdata));
			if(writeaccdata==-1)
			{	
			 	perror("Sending account details to client failed");
			 	semop(semIdentifier_a,&v,1); //release lock
			 	return;
			}
			int confirm;
			int retry;
			for(retry=1;retry<4;retry++)
			{
				int readflag=read(connfd, &confirm, sizeof(confirm));
				if(readflag==-1)
				{	
			 		perror("Reading confirmation flag failed");
			 		semop(semIdentifier_a,&v,1); //release lock
			 		return;
				}
				if(confirm==1)
				{
				
					read(connfd, &depositamount, sizeof(depositamount));
					accountdata.balance=accountdata.balance + depositamount;
					int accfd=open("AccountsDB.txt",O_RDWR);
               				int offset = lseek(accfd, accountdata.AccountNo*sizeof(accountdata), SEEK_SET);
               				 struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(accountdata), getpid()};
              				 int lockingStatus = fcntl(accfd, F_SETLKW, &lock);
               				 if (lockingStatus == -1)
               				 {
              				      perror("Error obtaining write lock on account file!");
              				      semop(semIdentifier_a,&v,1); //release lock
              					return;
                			}
               				int flagw=write(accfd, &accountdata, sizeof(accountdata));
               				perror("Money deposit status");
               				if (flagw == -1)
               				 {
                    				perror("Error storing updated deposit money in account record!");
                    				semop(semIdentifier_a,&v,1); //release lock
                    				
                			}
               				else
               				{	
               					struct Transactions newTxn;
               					int txnfd = open("Passbook.txt", O_CREAT | O_APPEND | O_RDWR, 0666);
						perror("Passbook open status");
						if(txnfd!=0)
   						{
   							// Get last transaction details
   					 		int offset = lseek(txnfd, -sizeof(newTxn), SEEK_END); 
    							if (offset >=0)  //// At least one transaction is saved
    							{
        					
       								struct Transactions prevTxn;
       						 		read(txnfd, &prevTxn, sizeof(prevTxn));
								perror("prevTxn read status");
        							newTxn.txnID = prevTxn.txnID + 1;
    							}
    							else // No transaction records exist
        							newTxn.txnID = 0;
        						
							newTxn.accountNumber = accountdata.AccountNo;
    							newTxn.Balanceb4 = accountdata.balance-depositamount;
    							newTxn.Balanceafter = accountdata.balance;
    							newTxn.TxnType = 'D';         
    							newTxn.txnamount=depositamount;
    							newTxn.transactionTime = time(NULL);
    							int passbookwriteflag = write(txnfd, &newTxn, sizeof(newTxn));
   							perror("Passbook write status");
   							write(connfd, &passbookwriteflag, sizeof(passbookwriteflag));  //telling client account deposit is successfully
   							if(passbookwriteflag!=-1)
               						{	//write(connfd, &accountdata, sizeof(accountdata));  //sending account data to client
               							struct AccountDetails tempaccountdata;
               							lseek(accfd, accountdata.AccountNo*sizeof(tempaccountdata), SEEK_SET);
               							read(accfd, &tempaccountdata, sizeof(tempaccountdata));
               							write(connfd, &tempaccountdata, sizeof(tempaccountdata));  //sending account data to client
               							lock.l_type = F_UNLCK;
                						fcntl(accfd, F_SETLK, &lock);
                						semop(semIdentifier_a,&v,1); //release lock

               						}
               						else
               						{
               							printf("Passbook update withrawl failed\n");
               						//reverting back the updated account balance
               							accountdata.balance-=depositamount;
               							int offset = lseek(accfd, accountdata.AccountNo*sizeof(accountdata), SEEK_SET);
               							write(accfd, &accountdata, sizeof(accountdata));
               							perror("Reverting account balance status");
               							lock.l_type = F_UNLCK;
                						fcntl(accfd, F_SETLK, &lock);
                						semop(semIdentifier_a,&v,1); //release lock
               						}
               					}
               					else 
               					{	printf("Passbook open  failed\n");
               					//reverting back the updated account balance
               						accountdata.balance-=depositamount;
               						int offset = lseek(accfd, accountdata.AccountNo*sizeof(accountdata), SEEK_SET);
               						write(accfd, &accountdata, sizeof(accountdata));
               						perror("Reverting account balance status");
               						lock.l_type = F_UNLCK;
                					fcntl(accfd, F_SETLK, &lock);
               						semop(semIdentifier_a,&v,1); //release lock
               					}
               				
               			//	else
               			 //	{	printf("AccountsDB.txt update failed...\n");
               			 		semop(semIdentifier_a,&v,1); //release lock
               			 //	}
               					
               			   }
               				break;
               			
               			 
               			}   //if confirm closed
              			else
               			{
               				
               				printf("Withrawl aboted : NO confirmation received..\n");
               			 	semop(semIdentifier_a,&v,1); //release lock
               			}
               			break;
               			
               		}//for close if no. of retries
               		if(retry==4)
               			{
               				printf("Withrawl aboted : NO confirmation received..\n");
               			 	semop(semIdentifier_a,&v,1); //release lock
               			}	
			
        		
        		
	} // DEPOSIT MONEY CLOSE
		
	// WITHDRAW MONEY
		else if(cust_action==3)
		{	
			long int withrawl=0;
			bool validtxn=false;
			int semopStatus = semop(semIdentifier_a, &p, 1);
			if (semopStatus == -1)
    			{
        			perror("Error while locking critical section");
        			
        			return;
    			}
    			customerdata=readcustomerdata(connfd,UserNo);
			accountdata=readaccdata(connfd,accountno);
			write(connfd, &customerdata, sizeof(customerdata));
			write(connfd, &accountdata, sizeof(accountdata));
			int confirm;
			int retry;
			for(retry=1;retry<4;retry++)
			{ 	read(connfd, &confirm, sizeof(confirm));
				if(confirm==1)
				{
					read(connfd, &withrawl, sizeof(withrawl));
				//if(withrawl<accountdata.balance)
				//{	validtxn=true;
				//	write(connfd, &validtxn, sizeof(validtxn));
					accountdata.balance=accountdata.balance-withrawl;
					int accfd=open("AccountsDB.txt",O_RDWR);
               				int offset = lseek(accfd, accountdata.AccountNo*sizeof(accountdata), SEEK_SET);
               				 struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(accountdata), getpid()};
              				 int lockingStatus = fcntl(accfd, F_SETLKW, &lock);
               				 if (lockingStatus == -1)
               				 {
              				      perror("Error obtaining write lock on account file!");
              				      semop(semIdentifier_a,&v,1); //release lock
              					return;
                			}
               				int flagw= write(accfd, &accountdata, sizeof(accountdata));
               				perror("Money withdrawl status");
               				if(flagw!=-1)
               				{
               					struct Transactions newTxn;
               					int txnfd = open("Passbook.txt",O_RDWR);
						perror("Passbook open status");
						if(txnfd!=0)
   						{
   							// Get last transaction details
   					 		int offset = lseek(txnfd, -sizeof(newTxn), SEEK_END); 
    							if (offset >=0)  //// At least one transaction is saved
    							{
        					
       								struct Transactions prevTxn;
       						 		read(txnfd, &prevTxn, sizeof(prevTxn));
								perror("prevTxn read status");
        							newTxn.txnID = prevTxn.txnID + 1;
    							}
    							else // No transaction records exist
        							newTxn.txnID = 0;
        						
							newTxn.accountNumber = accountdata.AccountNo;
    							newTxn.Balanceb4 = accountdata.balance+withrawl;
    							newTxn.Balanceafter = accountdata.balance;
    							newTxn.TxnType = 'W';         
    							newTxn.txnamount=withrawl;
    							newTxn.transactionTime = time(NULL);
    							int passbookwriteflag = write(txnfd, &newTxn, sizeof(newTxn));
   							perror("Passbook write status");
   							write(connfd, &passbookwriteflag, sizeof(passbookwriteflag));  //telling client account withrawl is successful
   							if(passbookwriteflag!=-1)
               						{	struct AccountDetails tempaccountdata;
               							lseek(accfd, accountdata.AccountNo*sizeof(tempaccountdata), SEEK_SET);
               							read(accfd, &tempaccountdata, sizeof(tempaccountdata));
               							write(connfd, &tempaccountdata, sizeof(tempaccountdata));  //sending account data to client
               							lock.l_type = F_UNLCK;
                						fcntl(accfd, F_SETLK, &lock);
                						semop(semIdentifier_a,&v,1); //release lock

               						}
               						else
               						{
               							printf("Passbook update withrawl failed\n");
               						//reverting back the updated account balance
               							accountdata.balance+=withrawl;
               							int offset = lseek(accfd, accountdata.AccountNo*sizeof(accountdata), SEEK_SET);
               							write(accfd, &accountdata, sizeof(accountdata));
               							perror("Reverting account balance status");
               							lock.l_type = F_UNLCK;
                						fcntl(accfd, F_SETLK, &lock);
                						semop(semIdentifier_a,&v,1); //release lock
               						}
               					}
               				      else 
               					{	printf("Passbook open withrawl failed\n");
               					//reverting back the updated account balance
               						accountdata.balance+=withrawl;
               						int offset = lseek(accfd, accountdata.AccountNo*sizeof(accountdata), SEEK_SET);
               						write(accfd, &accountdata, sizeof(accountdata));
               						perror("Reverting account balance status");
               						lock.l_type = F_UNLCK;
                						fcntl(accfd, F_SETLK, &lock);
                						semop(semIdentifier_a,&v,1); //release lock
               					}
               				}
               				else
               			 	{	printf("AccountsDB.txt update failed...\n");
               			 		semop(semIdentifier_a,&v,1); //release lock
               			 	}
               				break;
               			}
               			else
               			{
               				
               				printf("Withrawl aboted : NO confirmation received..\n");
               			 	semop(semIdentifier_a,&v,1); //release lock
               			}
               			break;
               			
               		}
               		if(retry==4)
               			{
               				printf("Withrawl aboted : NO confirmation received..\n");
               			 	semop(semIdentifier_a,&v,1); //release lock
               			}
               			
               			//{
               		//		write(connfd, &validtxn, sizeof(validtxn));
               		//		printf(" INSUFFIENT FUNDS IN ACCOUNT NO. %ld FOR WITHDRAW TRANSACTION",accountdata.AccountNo);
			//	}
			
			
			
        		
        		
	} //WITHDRAW MONEY CLOSED
		// ACCOUNT BALANCE ENQUIRY
		else if(cust_action==4)
		{	customerdata=readcustomerdata(connfd,UserNo);
			accountdata=readaccdata(connfd,accountno);
			write(connfd, &customerdata, sizeof(customerdata));
			write(connfd, &accountdata, sizeof(accountdata));
        		
		} //ACCOUNT BALANCE ENQUIRY CLOSED
		
	// OPEN PASSBOOK
		else if(cust_action==5)
		{
			customerdata=readcustomerdata(connfd,UserNo);
			accountdata=readaccdata(connfd,accountno);
			write(connfd, &customerdata, sizeof(customerdata));
			write(connfd, &accountdata, sizeof(accountdata));
			int txncount=0; 
			struct Transactions Txns;
					
					int passbkfd=open("Passbook.txt",O_RDONLY);
					if(passbkfd==-1)
        				{
        					perror("Unable to passbook");
        					printf("Exiting....\n");
        					_exit(0);
					}
					
					while(read(passbkfd,&Txns,sizeof(Txns)))
					{	
        					if(Txns.accountNumber==accountdata.AccountNo) //Txt for account no. found in records
        					{	txncount++;
        						//printf("\n| TIME_of_TXN\tTXN_TYPE\tBALANCE BEFORE\tTXN AMOUNT\tBALANCE AFTER\t");
							//printf("\n|  %s\t%c\t%ld\t%ld\t%ld\t",ctime(&Txns.transactionTime),Txns.TxnType,Txns.Balanceb4,Txns.txnamount,Txns.Balanceafter);
        						
        						write(connfd, &txncount, sizeof(txncount));  
        						printf("\nFound the txncount %d of Account no: %d ....\n",txncount,accountdata.AccountNo);
        						write(connfd, &Txns, sizeof(Txns));      //sending txn detail client
        							
        					}
        				}
        				write(connfd, &txncount, sizeof(txncount)); 
        				printf("Transaction search in passbook completed....\n");
        				
        				
		}// OPEN PASSBOOK closes
		
	//PASSWORD UPDATE 	
		else if(cust_action==6)
		{	
			int match=0;
			char tmppwd[20],encryptedpassword[20], newpwd[20];
			struct sembuf semOp = {0, -1, SEM_UNDO};
   			 int semopStatus = semop(semIdentifier_a, &p, 1);
   			 if (semopStatus == -1)
   			 {
   			     perror("Error while locking critical section");
    			     return;
    			}

			customerdata=readcustomerdata(connfd,UserNo);
			int flag= write(connfd, &customerdata, sizeof(customerdata));
			if (flag == -1)
   			 {
       				 perror("Error writing data to to client!");
        			semop(semIdentifier_a,&v,1); //release lock
        			return ;
    			}

			int rflag= read(connfd, &tmppwd, sizeof(tmppwd));
			if (rflag == -1)
   			 {
       				perror("Error while reading data from client !");
        			semop(semIdentifier_a,&v,1); //release lock
        			return;
    			}
			strcpy(encryptedpassword, crypt(tmppwd, "056"));
			if(strcmp(customerdata.cred.PWD,encryptedpassword)==0)
			 	match=1;
			flag=write(connfd, &match, sizeof(match));
			if (flag == -1)
   			 {
       				 perror("Error writing data to to client!");
        			semop(semIdentifier_a,&v,1); //release lock
        			return;
    			}
			if(match)
			{	int match2=0;
				flag= read(connfd, &match2, sizeof(match2));
				if (flag == -1)
   				 {
       					 perror("Error writing data to to client!");
        				semop(semIdentifier_a,&v,1); //release lock
        				return;
    				}
				if(match2==1)
				{
					read(connfd, &newpwd, sizeof(newpwd));
					strcpy(customerdata.cred.PWD, crypt(newpwd, "056"));
					int cfd=open("CustomerDetailsDB.txt",O_RDWR);
					int offset= lseek(cfd,customerdata.cred.UserNo*sizeof(customerdata),SEEK_SET);
					struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(customerdata), getpid()};
            				int lockingStatus = fcntl(cfd, F_SETLKW, &lock);
           				 if (lockingStatus == -1)
            				{
              					  perror("Error obtaining write lock on customer record!");
              					  semop(semIdentifier_a,&v,1); //release lock
        					return;
           				 }
        				int writestatus=write(cfd,&customerdata,sizeof(customerdata));
        				if(writestatus!=-1)
        					printf("Password updated successfully!!\n");
        				write(connfd, &writestatus, sizeof(writestatus));
        				lock.l_type = F_UNLCK;
           				lockingStatus = fcntl(cfd, F_SETLK, &lock);
        				semop(semIdentifier_a,&v,1); 
        			}
        			else
        			  semop(semIdentifier_a,&v,1);
			}
			else
				semop(semIdentifier_a,&v,1);
				
			 
		}
	//PASSWORD UPDATE CLOSED	
		else if(cust_action==7)
		{
			printf("Customer HAS LOGGED OUT\n");
			break;
		}
		else
		{
			printf("NO APPROPRIATE INPUT Customer HAS LOGGED OUT\n");
			break;
		}
		// break;
	
	
	}

}  //customeraction : customer action handler function closed

void initsemaphore(struct CustomerDetails customerdata)
{
	// Get a semaphore for the user
        key_t semKey_acc = ftok("CustomerDetailsDB.txt", customerdata.AccountNo); // Generate a key based on the account number hence, different account customer will have different semaphores
	
        union semun
        {
            int val; // Value of the semaphore
        } semSet1;
        

        int semctlStatus;
        semIdentifier_a = semget(semKey_acc, 1, 0); // Get the semaphore if it exists
      if (semIdentifier_a == -1)
        {
            semIdentifier_a = semget(semKey_acc, 1, IPC_CREAT | 0700); // Create a new semaphore
            if (semIdentifier_a == -1)
            {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet1.val = 1; // Set a binary semaphore
            semctlStatus = semctl(semIdentifier_a, 0, SETVAL, semSet1);
            if (semctlStatus == -1)
            {
                perror("Error while initializing a binary sempahore!");
                _exit(1);
            }
           
        }
     
        

}
#endif
