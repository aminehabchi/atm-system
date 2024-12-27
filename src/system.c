#include "header.h"

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

    while (option < 0 || option > 2)
    {
        printf("Enter 1 to go to the main menu and 0 to exit!\n");
        option = scanInt();
    }

    system("clear");
    if (option == 1)
    {
        mainMenu(u, db);
    }
    else if (option == 0)
    {
        sqlite3_close(db);
        exit(1);
    }
}

void createNewAcc(struct User u, sqlite3 *db)
{
    char *buffer = NULL;
    struct Record r;

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    buffer = NULL;
    while (buffer == NULL)
    {
        printf("\nEnter today's date(mm/dd/yyyy):");
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(r.time, buffer);
    free(buffer);
    r.accountNbr = 0;
    while (r.accountNbr <= 0)
    {
        printf("\nEnter the account number:");
        r.accountNbr = scanInt();
    }

    if (checkAccontIfExist(db, r.accountNbr) != -1)
    {
        goto noAccount;
    }

    //***************************** */
    buffer = NULL;
    while (buffer == NULL)
    {
        printf("\nEnter the country:");
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(r.country, buffer);
    free(buffer);
    //***************************** */
    buffer = NULL;
    while (buffer == NULL)
    {
        printf("\nEnter the phone number:");
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(r.phone, buffer);
    free(buffer);
    buffer = NULL;

    /***************************** */
    r.amount = 0;
    while (r.amount <= 0)
    {
        printf("\nEnter amount to deposit: $");
        r.amount = scanDouble();
    }

    /***************************** */

    while (buffer == NULL)
    {
        printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(r.accountType, buffer);
    free(buffer);
    buffer = NULL;

    /********************************/
    if (InsertAccInfo(db, u, r) == 1)
    {
        exit(1);
    }
    success(u, db);
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
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        struct Record r;

        r.accountNbr = sqlite3_column_int(stmt, 0);
        strcpy(r.country, (char *)sqlite3_column_text(stmt, 1));
        strcpy(r.phone, (char *)sqlite3_column_text(stmt, 2));
        strcpy(r.accountType, (char *)sqlite3_column_text(stmt, 3));
        strcpy(r.time, (char *)sqlite3_column_text(stmt, 4));
        r.amount = sqlite3_column_double(stmt, 5);
        printAcountInfo(r);
    }

    sqlite3_finalize(stmt);
}
void updateAcctInfo(struct User u, sqlite3 *db)
{
    int nbr = 0;
    while (nbr == 0)
    {
        printf("Enter the account number you want to update: ");
        nbr = scanInt();
    }
    int n;
    while (n <= 0)
    {
        printf("Which information do you want to update?\n");
        printf("1 -> Phone number\n");
        printf("2 -> Country\n");

        n = scanInt();
    }

    char str[100];

    if (n != 1 && n != 2)
    {
        printf("Invalid option. Returning...\n");
        return;
    }

    char *buffer = NULL;
    while (buffer == NULL)
    {
        system("clear");
        if (n == 1)
        {
            printf("Enter the new phone number: ");
        }
        else if (n == 2)
        {
            printf("Enter the new country: ");
        }
        printf("\nEnter the phone number:");
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(str, buffer);
    free(buffer);
    buffer = NULL;

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
    int id = 0;
    while (id <= 0)
    {
        printf("enter Account Number:");
        id = scanInt();
    }

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
    while (getchar() != '\n')
        ;
noAccount:
    int id = 0;
    while (id <= 0)
    {
        printf("enter Account Number:");
        id = scanInt();
    }

    if (checkAccontIfExist(db, id) != -1)
    {
        goto noAccount;
    }
Retry:
    int option = 0;
    while (option <= 0 || option > 2)
    {
        printf("withdraw -> 1\n deposit -> 2\n");
        option = scanInt();
    }
    char message[30];
    if (option == 1)
    {
        strcpy(message, "enter amount you to withdraw:");
    }
    else if (option == 2)
    {
        strcpy(message, "enter amount you to deposit:");
    }
    else
    {
        goto Retry;
    }

    double TranAmount;
    while (TranAmount <= 0)
    {
        printf("%s", message);
        TranAmount = scanDouble();
    }

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
    double accntAmount;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        accntAmount = sqlite3_column_double(stmt, 0);
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
    sqlite3_bind_double(stmt, 1, accntAmount);
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
    while (getchar() != '\n')
        ;
    int nbr = 0;
    while (nbr <= 0)
    {
        printf("enter acccont number you want to transfer ownership:");
        nbr = scanInt();
    }

    /// checkIfAccountExist
    /// checkIfUserExist
    struct User ToU;

    char *buffer = NULL;
    while (buffer == NULL)
    {

        printf("which user you want transfer ownership to (user name):");
        printf("\nEnter the phone number:");
        buffer = scanString(50, isAlphaNemric);
    }
    strcpy(ToU.name, buffer);
    free(buffer);
    buffer = NULL;

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