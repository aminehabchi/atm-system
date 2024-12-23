#include "header.h"

const char *RECORDS = "../data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return 0;
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}
int checkAccontIfExist(sqlite3 *db, int accountNbr)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT accountNbr FROM records where accountNbr=?;";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    // sqlite3_bind_text(stmt, 1, accountNbr, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 1, accountNbr);
    // Execute statement and loop through results
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return id;
    }
    sqlite3_finalize(stmt);
    return -1;
}
int InsertAccInfo(sqlite3 *db, struct User u, struct Record r)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO records(userId,name,country,phone,accountType,accountNbr,time) VALUES (?,?,?,?,?,?,?);";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, u.id);
    sqlite3_bind_text(stmt, 2, u.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, r.phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, r.accountType, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, r.accountNbr);
    sqlite3_bind_text(stmt, 7, r.time, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) ==  SQLITE_DONE)
    {
        
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}
void createNewAcc(struct User u)
{
    sqlite3 *db;
    char *Error = 0;
    int rc = sqlite3_open("database.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    struct Record r;

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%s", r.time);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);
    if (checkAccontIfExist(db, r.accountNbr) != -1)
    {
        goto noAccount;
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%s", r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    if (InsertAccInfo(db, u, r) == 1)
    {
        exit(1);
    }
    success(u);
}

void checkAllAccounts(struct User u)
{
}
