#include "header.h"
int isAlphaNemric(char ch)
{
    if (isalnum(ch))
    {
        return 1;
    }
    return -1;
}
int isPrintble(char ch)
{
    if (' ' < ch && ch <= 'z')
    {
        return 1;
    }
    return -1;
}

int isNums(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return 1;
    }
    return -1;
}
int indexOfChar(char *str, char ch)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ch)
        {
            return i;
        }
    }
    return -1;
}

int checkInput(char *s, int f(char ch))
{
    int i = 0;
    while (s[i] != '\0')
    {
        if (f(s[i]) == -1)
        {
            return -1;
        }
        i++;
    }
    return 1;
}
char *scanString(int maxlen, int f(char ch))
{
    char *line = NULL;
    size_t len = 0; // Buffer size
    size_t read;    // Number of characters read

    read = getline(&line, &len, stdin);
    if (read <= 1 || read > maxlen)
    {
        free(line);
        return NULL;
    }
    int i = indexOfChar(line, '\n');
    if (i != -1)
    {
        line[i] = '\0';
    }
    if (checkInput(line, f) == -1)
    {
        free(line);
        return NULL;
    }
    return line;
}

int scanInt()
{
    char *line = NULL;
    size_t len = 0; // Buffer size
    size_t read;    // Number of characters read

    read = getline(&line, &len, stdin);

    // 10 is length(2147483647)
    if (read <= 1 || read > 10)
    {
        free(line);
        return 0;
    }

    int i = indexOfChar(line, '\n');
    if (i != -1)
    {
        line[i] = '\0';
    }
    if (checkInput(line, isNums) == -1)
    {
        free(line);
        return 0;
    }
    int r = atoi(line);
    free(line);
    return r;
}

int isDouble(char *s)
{
    int i = 0;
    int p = 0;

    while (s[i] != '\0')
    {
        if (s[i] == '.')
        {
            p++;
            if (p > 1)
            {
                return -1; // More than one decimal point
            }
        }
        else if (s[i] >= '0' && s[i] <= '9')
        {
        }
        else
        {
            return -1; // Invalid character
        }
        i++;
    }

    return 1;
}

double scanDouble()
{
    char *line = NULL;
    size_t len = 0; // Buffer size
    size_t read;    // Number of characters read
    read = getline(&line, &len, stdin);
    // 10 is length(2147483647)
    if (read == -1 || read > 11)
    {
        free(line);
        return 0;
    }
    int i = indexOfChar(line, '\n');
    if (i != -1)
    {
        line[i] = '\0';
    }
    if (isDouble(line) == -1)
    {
        free(line);
        return 0;
    }
    return atof(line);
}

void printAcountInfo(struct Record r)
{
    printf("Account number:%d\n", r.accountNbr);
    printf("Deposit Date:%s\n", r.time);
    printf("country:%s\n", r.country);
    printf("Phone number:%s\n", r.phone);
    printf("Amountdeposited: $%.2f\n", r.amount);
    printf("Type Of Account:%s\n\n", r.accountType);
}
void accountDetials(char accountType[10], double amount, char *time)
{
    char *day = strtok(time, "/");
    printf("%s\n", accountType);
    if (strcmp(accountType, "saving") == 0)
    {
        amount = amount * 0.07 / 12;
    }
    else if (strcmp(accountType, "fixed03") == 0)
    {
        amount = amount * 0.04 / 12;
    }
    else if (strcmp(accountType, "fixed01") == 0)
    {
        amount = amount * 0.05 / 12;
    }
    else if (strcmp(accountType, "fixed02") == 0)
    {
        amount = amount * 0.08 / 12;
    }
    else
    {
        printf("\nYou will not get interests because the account is of type current\n");
        return;
    }
    printf("\n-> You will get $%.2f as interest on day %s of every month\n", amount, day);
}
char *accountType(int n)
{
    switch (n)
    {
    case 1:
        return "saving";
    case 2:
        return "current";
    case 3:
        return "fixed01";
    case 4:
        return "fixed02";
    case 5:
        return "fixed03";
    }
    return "";
}

int isValidDate(int day, int month, int year)
{
    if (month < 1 || month > 12)
        return 0; // Invalid month
    if (day < 1)
        return 0; // Invalid day

    // Days in each month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Leap year check
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        daysInMonth[2] = 29;

    return (day <= daysInMonth[month]); // Check if day is valid for the month
}

void scanDate(Date *date)
{
    char input[20]; // Buffer for input
    int day, month, year;

    while (1)
    {
        printf("Enter date (dd/mm/yyyy): ");
        if (!fgets(input, sizeof(input), stdin))
        {
            printf("Error reading input. Try again.\n");
            continue;
        }

        // Validate format using sscanf
        if (sscanf(input, "%d/%d/%d", &day, &month, &year) != 3)
        {
            printf("Invalid format! Use dd/mm/yyyy.\n");
            continue;
        }

        // Check if date is valid
        if (!isValidDate(day, month, year))
        {
            printf("Invalid date! Please enter a real date.\n");
            continue;
        }

        // Assign values if valid
        date->day = day;
        date->month = month;
        date->year = year;
        break;
    }
}
void dateToString(Date date, char *str)
{
    sprintf(str, "%02d/%02d/%04d", date.day, date.month, date.year);
}