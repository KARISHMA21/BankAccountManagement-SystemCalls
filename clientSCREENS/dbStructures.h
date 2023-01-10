#ifndef record_structures
#define record_structures

//*************** LOGIN DETAILS ******************************//
struct Credentials
{
    int USERTYPE;  //1--> ADMIN ; 2--> NORMAL , 3 ---> JOINT
    int UserNo; 
    char UID[20];
    char PWD[20];
    char Uname[25];
    
};

//*************** CUSTOMER DETAILS ******************************//
struct CustomerDetails
{
    struct Credentials	cred;	
    //int id; // 0, 1, 2 ....
    //char name[25];
    char gender[10]; // M -> Male, F -> Female, O -> Other
    int age;
    // Login Credentials
    //char login[30]; // Format : name-id (name will the first word in the structure member `name`)
    //char password[30];
    // Bank data
   int AccountNo; // Account number the customer owns
};

//*************** ACCOUNT DETAILS ******************************//
struct AccountDetails
{
    struct CustomerDetails custinfo;	
    int AccountNo;     // 0, 1, 2, ....
    int UserNo[2];         // saves associated User no.s , -1 if single user account
    int account_type; // 1 -> Regular account, 2 -> Joint account
    char status;    //A--> active, D-->Deactivated
    long int balance;      // Amount of money in the account
   // int transactions[MAX_TRANSACTIONS];  // A list of transaction IDs. Used to look up the transactions. // -1 indicates unused space in array
};

//*************** TRANSACTION DETAILS ******************************//
#include <time.h>

struct Transactions
{
    int txnID; // 0, 1, 2, 3 ...  ??
    int accountNumber;
    char TxnType; // W -> Withdraw, D -> Deposit
    long int Balanceb4;
    long int Balanceafter;
    long int txnamount;  //abs(Balanceafter - Balanceb4)
    time_t transactionTime;
};

#endif
