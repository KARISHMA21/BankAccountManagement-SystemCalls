#ifndef GRAPHICS
#define GRAPHICS
void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

void printHL(int x,int y,char *c,int offset)
{	if(offset>0)
		gotoxy(offset,y);
//	if(y==0)
	for(int i=0;i<x;i++)
		printf("%s",c);
	
//	else
//	{	gotoxy(,y);
//		for(int i=0;i<x;i++)
//			printf("%c",c);
//	}	
}
void printVL(int x,int y,char *c,int offset)
{	gotoxy(x,offset);
	
		for(int i=0;i<y;i++)
		{	printf("%s",c);
			gotoxy(x,++offset);
		}
	
}

void boxify()
{	
	char *heading="| ONLINE BANKING MANAGEMENT SYSTEM |";
	system("clear");
	printVL(0,30,"|",0);
	printVL(80,30,"|",0);
	gotoxy(0,0);
	printHL(22,0,"=",0);
	gotoxy(23,0);
        printf("%s",heading);
 	printHL(22,0,"=",0);
}

#endif
