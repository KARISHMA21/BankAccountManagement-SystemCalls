#include "dbStructures.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
void main()
{
	char buf[20];
	struct Credentials cred,previous;
	int fd=open("Credentials.txt",O_RDWR|O_CREAT,0744);
	
	int offset = lseek(fd, -sizeof(cred), SEEK_END);
	read(fd, &previous, sizeof(previous));
	
		printf("Enter Admin name:\n");
		scanf("%s",cred.Uname);
		cred.USERTYPE=1;  //1--> ADMIN ; 2--> NORMAL , 3 ---> JOINT
    		cred.UserNo=previous.UserNo+1;
    		strcpy(cred.UID, cred.Uname);
    		sprintf(buf, "%d", cred.UserNo);
    		strcat(cred.UID, buf);
    		//cred.UID=cred.Uname+cred.UserNo;
    		printf("Enter password:\n");
		scanf("%s",cred.PWD);
  		cred.status='A'; 
		
	
	
	write(fd,&cred,sizeof(cred));
	read(fd, &cred, sizeof(cred));
	printf("The Uname is %s\n",cred.Uname); 
	printf("The user type is %d\n",cred.USERTYPE); 
	printf("The UserNo is %d\n",cred.UserNo); 
	printf("The UID is %s\n",cred.UID); 
	printf("The PWD is %s\n",cred.PWD); 
	printf("The status is %c\n",cred.status);
   
	
	close(fd);
	/*
	fd=open("RailwayDB.txt",O_RDONLY);
	read(fd,&RailDB,sizeof(RailDB));
	printf("Train No. : %d\n",RailDB.train_no);
	printf("Ticket No. : %d\n",RailDB.ticket_no);
	*/
	
	
}
