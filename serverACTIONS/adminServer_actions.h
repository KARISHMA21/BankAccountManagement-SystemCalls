#include "dbStructures.h"
//#include<stdlib.h>
#include <string.h>
//#include<unistd.h>
#include<fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
 #include <crypt.h>
void adminaction(int connfd,int clientno);
int addcustomer(int connfd, int account_type,int accountno);
static int i=0;
void adminlogin(int userType, int connfd,int clientno)
{ 
	
	struct Credentials cred;
	char temp_ID[20], temp_PWD[20];
	int loginstatus=0;
	read(connfd, temp_ID, sizeof(temp_ID));		//R S2
	read(connfd, temp_PWD, sizeof(temp_PWD));	//R S3
	
	int loginfd=open("Credentials.txt",O_RDONLY);
    	if(loginfd==-1)
        {
        	perror("Unable to open file");
        	printf("Exiting %d....\n",clientno);
        	_exit(0);
	}
	lseek(loginfd,0,SEEK_SET); 
    	while(read(loginfd,&cred,sizeof(cred)))
	{	
        	if(strcmp(cred.UID,temp_ID)==0) 
        	{ 	if(strcmp(cred.PWD,temp_PWD)==0) 
        		{	if(cred.USERTYPE==userType)
        			{	loginstatus= 1;                //login successfull
        				break;
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
        write(connfd, &loginstatus, sizeof(loginstatus));		//sending login status back to client  W2 c1
        //close(loginfd);  
        if(loginstatus==1)
	{	printf("Login Sucessful\n");
		adminaction(connfd,clientno);
		
	}
	else
		printf("CLient %d :Invalid username or password\n",clientno);
/*
    if(flag==4)
    {
        gotoxy(leftpos,17);printf("Login Sucessful");
        getch();
    }
    else
    {
        gotoxy(leftpos,17);
        getchar();
        admin_settings_signin();
    }
	*/
	
}

void adminaction(int connfd,int clientno)
{
	int admin_action;
	
	
	while(1)   //STAY IN WHILE TILL LOGOUT 
	{	printf(" THe I is %d: \n",++i);
		read(connfd, &admin_action, sizeof(admin_action));     //R S4
		printf(" THe admin_action is %d: \n",admin_action);
		
		// ADDING NEW USER -------------------------------------------------------//
		
		if(admin_action==1)
		{	admin_action=-1;
			printf("Adding new account..\n");
			struct AccountDetails newAccount, prevAccount;
			int fd=open("AccountsDB.txt",O_RDONLY);
			perror("AccountsDB.txt Open status");
			if (fd == -1)
			{
				if(errno == ENOENT)  //No previous accounts exist
   			 	{	newAccount.AccountNo = 0;
   			 	}
   			 	else
   			 	{	perror("Error while opening account file");
   			 		_exit(0);
   			 	}
        				
    			}
    			else  //Extract and last saved account details 
    			{
        			int offset = lseek(fd, -sizeof(struct AccountDetails), SEEK_END);
        			if (offset == -1)
        			{
        			    perror("Error seeking to last Account record!");
        			    _exit(0);
        			}

       				 struct flock lock ; //= {F_RDLCK, SEEK_SET, offset, sizeof(struct AccountDetails), getpid()};
       				 lock.l_pid=getpid();
				 lock.l_type=F_RDLCK;
				 lock.l_whence=SEEK_SET;
				 lock.l_start=offset;
		                 lock.l_len=sizeof(struct AccountDetails);
       				 int lockingStatus = fcntl(fd, F_SETLKW, &lock);
       				 if (lockingStatus == -1)
       				 {
        			    	perror("Error!! READ LOCK COULD NOT BE SET FOR ACCOUNT DETAILS");
          			   	 _exit(0);
      				 }
      				 

        			read(fd, &prevAccount, sizeof(prevAccount));
      
  				lock.l_type = F_UNLCK;
        			fcntl(fd, F_SETLK, &lock);  //Releasing the read lock

        			close(fd);

       				newAccount.AccountNo = prevAccount.AccountNo + 1;
       				
    			}
    			newAccount.status = 'A';
    			newAccount.balance = 0;
    			int account_type;
    			read(connfd, &account_type, sizeof(account_type));       //R S5
    			if(account_type !=1 && account_type !=2)
    			{
    				printf(" 144 CLIENT %d : Account addition aborted ...\n",clientno);	
    				continue;
    			}	
    			
    			newAccount.account_type = account_type;
    			int confirm=0;
    			read(connfd, &confirm, sizeof(confirm));		//R s6
    			if(confirm==1)
    			
    			{
    				newAccount.UserNo[0]=addcustomer(connfd,account_type,newAccount.AccountNo);  //adding customer 1 details
    				int user=1;
    				printf("140-%d\n",user++);
    				if(account_type==2)    					//joint user acount type
    				{	read(connfd, &confirm, sizeof(confirm));
    					if(confirm==1)
    						newAccount.UserNo[1]=addcustomer(connfd,account_type,newAccount.AccountNo);
    					//printf("143-%d\n",user);
    					else
    					{
    						printf("CLIENT %d : Account addition aborted ...\n",clientno);	
    					}
    				}
    				else
    					newAccount.UserNo[1]=-1;
    				int allusercreated=0;
    				read(connfd, &allusercreated, sizeof(allusercreated));
    				if(confirm==1 && allusercreated ==1)
    				{	
    					fd = open("AccountsDB.txt", O_CREAT | O_APPEND | O_RDWR, 0666);
    			
  					write(fd,&newAccount,sizeof(newAccount));
					read(fd, &newAccount, sizeof(newAccount));
					
    					write(connfd, &newAccount, sizeof(newAccount));
    					perror(" user add write status");
    				}
    			}
    			else
    				printf("CLIENT %d : Account addition aborted ...\n",clientno);	
			
   
		}

		// SEARCH USER DETAILS -------------------------------------------------------//
		else if(admin_action==2)
		{	
			char temp_ID[20]; 
			bool Customerfound=false;
			struct CustomerDetails CustData;
			read(connfd, temp_ID, sizeof(temp_ID));
			int fd=open("CustomerDetailsDB.txt",O_RDONLY);
			if(fd==-1)
        		{
        		perror("Unable to open file");
        		printf("Exiting....\n");
        		_exit(0);
			}
			lseek(fd,0,SEEK_SET);
			while(read(fd,&CustData,sizeof(CustData)))
			{	
        			if(strcmp(CustData.cred.UID,temp_ID)==0)   //Customer found in records
        			{	Customerfound=true;
        				write(connfd, &Customerfound, sizeof(Customerfound));
        				write(connfd, &CustData, sizeof(CustData));
        				break;
        			}	
        		
        		}
        		if(!Customerfound)
        			write(connfd, &Customerfound, sizeof(Customerfound));  //customer not found
        	
        	}
        	// MODIFY USER DETAILS -------------------------------------------------------//
        	else if(admin_action==3)
		{	
			char temp_ID[20], newname[20],newgender[10];
			int whattoedit, newage; 
			bool Customerfound=false;
			struct CustomerDetails CustData;
			read(connfd, temp_ID, sizeof(temp_ID));
			int fd=open("CustomerDetailsDB.txt",O_RDWR);
			if(fd==-1)
        		{
        			perror("Unable to open file");
        			printf("Exiting....\n");
        			_exit(0);
			}
			lseek(fd,0,SEEK_SET);
			while(read(fd,&CustData,sizeof(CustData)))
			{	int recordno=0;
        			if(strcmp(CustData.cred.UID,temp_ID)==0)   //Customer found in records
        			{	Customerfound=true;
        				write(connfd, &Customerfound, sizeof(Customerfound));
        				write(connfd, &CustData, sizeof(CustData));
        				read(connfd, &whattoedit, sizeof(whattoedit)); //1-->name, 2-->gender,3-->age
        				if(whattoedit==1)
        				{	read(connfd, &newname, sizeof(newname));
        					strcpy(CustData.cred.Uname,newname);
        			
        				}
        				else if(whattoedit==2)
        				{
        					read(connfd, newgender, sizeof(newgender));
        					strcpy(CustData.gender,newgender);	
        				}
        				else if(whattoedit==3)
        				{
        					read(connfd, &newage, sizeof(newage));
						CustData.age=newage;
        				}
        				lseek(fd,recordno*sizeof(CustData),SEEK_SET);
        				write(fd,&CustData,sizeof(CustData));
        				printf("Customer details updated successfully!!\n");
        				write(connfd, &CustData, sizeof(CustData));
        				break;
        			}
        			recordno++;
        		
        		
        		}//while to read records closes
        		if(!Customerfound)
        			write(connfd, &Customerfound, sizeof(Customerfound));  //customer not found
        	
		
		}//admin_action==3: modify details closed 
		
		else if(admin_action==4)  //Find account details
		{
			int tempaccountno; 
			bool accountfound=false;
			struct AccountDetails account;
			read(connfd, &tempaccountno, sizeof(tempaccountno));
			int fd=open("AccountsDB.txt",O_RDONLY);
			if(fd==-1)
        		{
        			perror("Unable to open file");
        			break;
			}
			lseek(fd,0,SEEK_SET);
			while(read(fd,&account,sizeof(account)))
			{	
        			if(account.AccountNo==tempaccountno)   //account no. found in records
        			{	accountfound=true;
        				write(connfd, &accountfound, sizeof(accountfound));
        				write(connfd, &account, sizeof(account));
        				break;
        			}	
        		
        		}
        		if(!accountfound)
        			write(connfd, &accountfound, sizeof(accountfound));  //customer not found
		} //Find account details closed
		
		else if(admin_action==5)  //Deactivate account
		{	char confirm='1';
			int tempaccountno; 
			bool accountfound=false;
			struct AccountDetails account,temp;
			read(connfd, &tempaccountno, sizeof(tempaccountno));
			int fd=open("AccountsDB.txt",O_RDWR);
			if(fd==-1)
        		{
        			perror("Unable to open file");
        			break;
			}
			lseek(fd,0,SEEK_SET);
			while(read(fd,&account,sizeof(account)))
			{	int recordno=0;
        			if(account.AccountNo==tempaccountno)   //account no. found in records
        			{	accountfound=true;
        				write(connfd, &accountfound, sizeof(accountfound));
        				write(connfd, &account, sizeof(account));
        				if(account.balance==0)
        				{
        					read(connfd, &confirm, sizeof(confirm));
        					if(confirm=='0')
        					{
        						lseek(fd,recordno*sizeof(account),SEEK_SET);
        					
        						read(connfd,&temp,sizeof(temp));
        						write(fd, &temp, sizeof(temp));
        						printf("Account deactivated successfully!!\n");
        						lseek(fd,recordno*sizeof(account),SEEK_SET);
        						read(fd,&account,sizeof(account));
        						write(connfd, &account, sizeof(account));
        					}
        					
        				}
        				
        					
        				break;
        			}
        			recordno++;	
        		
        		}
        		if(!accountfound)
        			write(connfd, &accountfound, sizeof(accountfound));  //customer not found
        		
		} //Find account details closed
		
		else if(admin_action==6)
		{
			printf("ADMIN HAS LOGGED OUT\n");
			break;
		}
		else
		{
			printf("NO APPROPRIATE INPUT ADMIN HAS LOGGED OUT\n");
			break;
		}
		 
		 
	}//while login
	
}

int addcustomer(int connfd, int account_type,int accountno)   //returns generated USER ID 
{
	//int user=1;
    	//printf("241-%d\n",user++);

    
	char tempPWD[20];
    	char tempUname[25];
    	char tempgender[10]; // M -> Male, F -> Female, O -> Other
    	int tempage;
	char buf[50];
	struct CustomerDetails Nacc,previous;
	
			read(connfd, tempUname, sizeof(tempUname));					//R s7
			strcpy(Nacc.cred.Uname,tempUname);
			read(connfd, tempgender, sizeof(tempgender));					//R s8
			strcpy(Nacc.gender,tempgender);	
			read(connfd, &tempage, sizeof(tempage));					//R s9
			Nacc.age=tempage;
		
			read(connfd, tempPWD, sizeof(tempPWD));						//R s10
			strcpy(Nacc.cred.PWD, crypt(tempPWD, "056"));
    			

			
		
			//printf("261-%d\n",user++);
			int fd=open("CustomerDetailsDB.txt",O_RDONLY);
			perror("CustomerDetailsDB.txt Open status");
			if (fd == -1)
			{	if(errno == ENOENT)
      				{	Nacc.cred.UserNo = 0;	  // Customer file was never created
      					//printf("267-%d\n",Nacc.cred.UserNo);
        			}
    				else
   				 {
      					  perror("Error while opening customer file");
        				  _exit(0);
   				 }
   			}
    			else
    			{
				int offset = lseek(fd, -sizeof(Nacc), SEEK_END);
				struct flock lock;// = {F_RDLCK, SEEK_SET, offset, sizeof(struct AccountDetails), getpid()};
       				 lock.l_pid=getpid();
				 lock.l_type=F_RDLCK;
				 lock.l_whence=SEEK_SET;
				 lock.l_start=offset;
		                 lock.l_len=sizeof(previous);
       				 int lockingStatus = fcntl(fd, F_SETLKW, &lock);
       				 if (lockingStatus == -1)
       				 {
        			    	perror("Error!! READ LOCK COULD NOT BE SET FOR ACCOUNT DETAILS");
          			   	 _exit(0);
      				 }
      				 

        			read(fd, &previous, sizeof(previous));
      
  				lock.l_type = F_UNLCK;
        			fcntl(fd, F_SETLK, &lock);  //Releasing the read lock

        			close(fd);
 			
				
    				Nacc.cred.UserNo=previous.cred.UserNo+1;
    			}
    				Nacc.cred.USERTYPE=2;  //1--> ADMIN ; 2--> NORMAL , 3 ---> JOINT
    				strcpy(Nacc.cred.UID, Nacc.cred.Uname);
    		
    				sprintf(buf, "%d", Nacc.cred.UserNo);
    				strcat(Nacc.cred.UID, buf);
    				if(account_type==1)
    					strcat(Nacc.cred.UID, "-N");
    				else
    					strcat(Nacc.cred.UID, "-J");
  				//Nacc.cred.status='A'; 
  				Nacc.AccountNo=accountno;
  				
  				fd = open("CustomerDetailsDB.txt", O_CREAT | O_APPEND | O_RDWR, 0666);
  				write(fd,&Nacc,sizeof(Nacc));
				read(fd, &Nacc, sizeof(Nacc));
				
			
				write(connfd, &Nacc, sizeof(Nacc));				//	W2 c2
				return Nacc.cred.UserNo;
			
}


