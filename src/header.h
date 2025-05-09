#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sqlite3.h>
#include <ctype.h>
typedef struct
{
    int day;
    int month;
    int year;
} Date;

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
    char time[10];
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// main
void initMenu(struct User *u, sqlite3 *db);
void mainMenu(struct User u, sqlite3 *db);

// authentication functions
void loginMenu(char a[50], char pass[50]);
int registerMenu(char a[50], char pass[50], sqlite3 *db);

// system function
void createNewAcc(struct User u, sqlite3 *db);                    // 1
void updateAcctInfo(struct User u, sqlite3 *db);                  // 2
void checkAccounts(struct User u, sqlite3 *db, int accountNmber); // 3
void checkAllAccounts(struct User u, sqlite3 *db);                // 4
void makeTransaction(struct User u, sqlite3 *db);                 // 5
void removeExistAccnt(struct User u, sqlite3 *db);                // 6
void transferOwner(struct User u, sqlite3 *db);                   // 7
void success(struct User u, sqlite3 *db);

// database
int CreateTable();
int getPassword(struct User u, sqlite3 *db);
int checkUserIfExist(sqlite3 *db, char name[50]);
int checkAccountIfExist(sqlite3 *db, int userId, int accountNbr);
int InsertAccInfo(sqlite3 *db, struct User u, struct Record r);
int registerInfo(char a[50], char pass[50], sqlite3 *db);
double getAmount(int id, int accountId, sqlite3 *db);

// funcs
int isAlphaNemric(char ch);
int isPrintble(char ch);
int isAlphaNemric(char ch);
char *scanString(int maxlen, int f(char ch));
int scanInt();
void printAcountInfo(struct Record r);
double scanDouble();
char *accountType(int n);
void accountDetials(char accountType[10], double amount,char *time);
void scanDate(Date *date);
int isValidDate(int day, int month, int year);
void dateToString(Date date, char *str);