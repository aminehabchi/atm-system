#include "header.h"

const char *RECORDS = "../data/records.txt";

// void stayOrReturn(int notGood, void f(struct User u), struct User u)
// {
//     int option;
//     if (notGood == 0)
//     {
//         system("clear");
//         printf("\n✖ Record not found!!\n");
//     invalid:
//         printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
//         scanf("%d", &option);
//         if (option == 0)
//             f(u);
//         else if (option == 1)
//             mainMenu(u);
//         else if (option == 2)
//             exit(0);
//         else
//         {
//             printf("Insert a valid operation!\n");
//             goto invalid;
//         }
//     }
//     else
//     {
//         printf("\nEnter 1 to go to the main menu and 0 to exit:");
//         scanf("%d", &option);
//     }
//     if (option == 1)
//     {
//         system("clear");
//         mainMenu(u);
//     }
//     else
//     {
//         system("clear");
//         exit(1);
//     }
// }

void success(struct User u, sqlite3 *db)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u, db);
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
    const char *sql = "INSERT INTO records(userId,name,country,phone,accountType,accountNbr,time,amount) VALUES (?,?,?,?,?,?,?,?);";

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
    sqlite3_bind_int(stmt, 8, r.amount);

    while (sqlite3_step(stmt) == SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}
void createNewAcc(struct User u, sqlite3 *db)
{

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
    success(u, db);
}
void printAcountInfo(struct Record r)
{
    printf("Account number:%d\n", r.accountNbr);
    printf("Deposit Date:%s\n", r.time);
    printf("country:%s\n", r.country);
    printf("Phone number:%s\n", r.phone);
    printf("Amountdeposited: $%f\n", r.amount);
    printf("Type Of Account:%s\n\n", r.accountType);
}

void checkAllAccounts(struct User u, sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT accountNbr FROM records where userId=?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, u.id);
    system("clear");
    printf("\t\t\t======= All accounts for user, %s =====\n", u.name);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int accountNbr = sqlite3_column_int(stmt, 0);
        printf("________________________________\n");
        checkAccounts(u, db, accountNbr);
    }
    sqlite3_finalize(stmt);
    success(u, db);
    return;
}

void checkAccounts(struct User u, sqlite3 *db, int accountNmber)
{
    struct Record r;
    r.accountNbr = accountNmber;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT accountNbr,country,phone,accountType,time,amount FROM records where accountNbr=?;";

    // Prepare SELECT statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, r.accountNbr);
    system("clear");
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        struct Record r;

        r.accountNbr = sqlite3_column_int(stmt, 0);
        strcpy(r.country, (char *)sqlite3_column_text(stmt, 1));
        strcpy(r.phone, (char *)sqlite3_column_text(stmt, 2));
        strcpy(r.accountType, (char *)sqlite3_column_text(stmt, 3));
        strcpy(r.time, (char *)sqlite3_column_text(stmt, 4));
        r.amount = sqlite3_column_int(stmt, 5);
        printAcountInfo(r);
    }

    sqlite3_finalize(stmt);
}
void updateAcctInfo(struct User u, sqlite3 *db)
{
    int nbr;
    printf("Enter the account number you want to update: ");
    scanf("%d", &nbr);

    printf("Which information do you want to update?\n");
    printf("1 -> Phone number\n");
    printf("2 -> Country\n");

    int n;
    scanf("%d", &n);

    system("clear");
    char str[100];
    if (n == 1)
    {
        printf("Enter the new phone number: ");
    }
    else if (n == 2)
    {
        printf("Enter the new country: ");
    }
    else
    {
        printf("Invalid option. Returning...\n");

        return;
    }
    scanf("%s", str);

    sqlite3_stmt *stmt;
    const char *sql;
    if (n == 1)
    {
        sql = "UPDATE records SET phone = ? WHERE accountNbr = ?;";
    }
    else
    {
        sql = "UPDATE records SET country = ? WHERE accountNbr = ?;";
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));

        return;
    }

    sqlite3_bind_text(stmt, 1, str, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, nbr);

    // Execute the update statement
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        success(u, db);
    }
    else
    {
        printf("Update failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}
void removeExistAccnt(struct User u, sqlite3 *db)
{
    int id;
    printf("enter Account Number:");
    scanf("%d", &id);

    sqlite3_stmt *stmt;
    const char *sql;

    sql = "DELETE FROM records WHERE accountNbr=? AND userId=?;";
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, u.id);
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        success(u, db); // Call success if the update is successful
        return;
    }
    else
    {
        printf("Update failed: %s\n", sqlite3_errmsg(db));
    }

    // Finalize and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void makeTransaction(struct User u, sqlite3 *db)
{
    int id;
    printf("enter Account Number:");
    scanf("%d", &id);
Retry:
    int option;
    printf("withdraw -> 1\n deposit -> 2\n");
    scanf("%d", &option);
    if (option == 1)
    {
        printf("enter amount you to withdraw:");
    }
    else if (option == 2)
    {
        printf("enter amount you to deposit:");
    }
    else
    {
        goto Retry;
    }
    int TranAmount;
    scanf("%d", &TranAmount);
    /*********************************************/

    sqlite3_stmt *stmt;
    const char *sql = "SELECT amount FROM records WHERE accountNbr=? AND userId=?;";

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, u.id);
    int accntAmount;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        accntAmount = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    if (option == 1 && accntAmount < TranAmount)
    {
        system("clear");
        printf("the amount you chose to withdraw is superior to your aviable balance !!\n\n");
        mainMenu(u, db);
        return;
    }
    if (option == 1)
    {
        accntAmount -= TranAmount;
    }
    else
    {
        accntAmount += TranAmount;
    }

    sql = "UPDATE records SET amount=? WHERE accountNbr=? AND userId=?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, accntAmount);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, u.id);
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        success(u, db);
    }
    else
    {
        printf("Update failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void transferOwner(struct User u, sqlite3 *db)
{
    int nbr;
    printf("enter acccont number you want to transfer ownership:");
    scanf("%d", &nbr);
    /// checkIfAccountExist
    /// checkIfUserExist
    struct User ToU;
    printf("which user you want transfer ownership to (user name):");
    scanf("%s", ToU.name);
    
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM users WHERE uname=?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, ToU.name, -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        ToU.id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    sql = "UPDATE records SET userId=?,name=? WHERE accountNbr=?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, ToU.id);
    sqlite3_bind_text(stmt, 2, ToU.name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, nbr);
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        success(u, db);
    }
    else
    {
        printf("aaaa");
    }
    printf("\n%d %s %d\n", ToU.id, ToU.name, nbr);
}

/***550***/