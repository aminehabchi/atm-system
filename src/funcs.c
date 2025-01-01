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

    if (read == -1 || read > maxlen)
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
    if (read == -1 || read > 10)
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
    printf("Amountdeposited: $%2f\n", r.amount);
    printf("Type Of Account:%s\n\n", r.accountType);
}
