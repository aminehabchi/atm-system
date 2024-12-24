#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sqlite3.h>
// struct Date
// {
//     int month, day, year;
// };

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    char phone[10];
    char accountType[10];
    int accountNbr;
    double amount;
    // struct Date deposit;
    // struct Date withdraw;
    char time[10];
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(char a[50], char pass[50]);
void initMenu(struct User *u);
int registerMenu(char a[50], char pass[50]);


int checkUserIfExist(sqlite3 *db, char name[50]);

int getPassword(struct User u);

// system function
void success(struct User u, sqlite3 *db);
void mainMenu(struct User u, sqlite3 *db);
void createNewAcc(struct User u, sqlite3 *db);
void checkAllAccounts(struct User u, sqlite3 *db);
void checkAccounts(struct User u, sqlite3 *db, int accountNmber);
void updateAcctInfo(struct User u, sqlite3 *db);
void removeExistAccnt(struct User u, sqlite3 *db);
void makeTransaction(struct User u, sqlite3 *db);
void transferOwner(struct User u, sqlite3 *db);
// database

int CreateTable();
