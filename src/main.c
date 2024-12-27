#include "header.h"

void mainMenu(struct User u, sqlite3 *db)
{
    int option = 0;
    // system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");

    while (option > 8 || option <= 0)
    {
        option = scanInt();
    }

    switch (option)
    {
    case 1:
        createNewAcc(u, db);
        break;
    case 2:
        updateAcctInfo(u, db);
        break;
    case 3:
        int n;
        while (n <= 0)
        {
            printf("%d enter Account Number:", n);
            n = scanInt();
        }
        system("clear");
        checkAccounts(u, db, n);
        success(u, db);
        break;
    case 4:
        checkAllAccounts(u, db);
        break;
    case 5:
        makeTransaction(u, db);
        break;
    case 6:
        removeExistAccnt(u, db);
        break;
    case 7:
        transferOwner(u, db);
        break;
    case 8:
        sqlite3_close(db);
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};

void initMenu(struct User *u, sqlite3 *db)
{
    int r = 0;
    system("clear");

    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    int option = 0;
    while (!r)
    {
        option = 0;
        while (option <= 0 || option > 3)
        {
            option = scanInt();
        }

        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            u->id = getPassword(*u, db);
            printf("%s %s", u->name, u->password);
            if (u->id != -1)
            {
                printf("\n\nPassword Match!");
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
            u->id = registerMenu(u->name, u->password, db);
            if (u->id != -1)
            {
                printf("\n\ninfo save");
            }
            else
            {
                printf("\n User Used\n");
                exit(1);
            }
            r = 1;
            break;
        case 3:
            sqlite3_close(db);
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
};

int main()
{

    int a = CreateTable();
    if (a == 1)
    {
        return 0;
    }
    sqlite3 *db;

    if (sqlite3_open("database.db", &db))
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    struct User u;
    initMenu(&u, db);
    mainMenu(u, db);

    return 1;
}
