#include "header.h"

void read_line(const char *prompt, char *buffer, size_t size)
{
    size_t len;
    int ch;

    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }

    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
    else
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
    }
}

int read_int(const char *prompt)
{
    char line[80];
    char *end;
    long value;

    while (1)
    {
        read_line(prompt, line, sizeof(line));
        value = strtol(line, &end, 10);

        while (*end && isspace((unsigned char)*end))
            end++;

        if (line[0] != '\0' && *end == '\0')
            return (int)value;

        printf("Invalid number. Please try again.\n");
    }
}

int contains_ignore_case(const char *text, const char *pattern)
{
    size_t i, j;

    if (pattern[0] == '\0')
        return 1;

    for (i = 0; text[i] != '\0'; i++)
    {
        for (j = 0; pattern[j] != '\0'; j++)
        {
            if (text[i + j] == '\0')
                break;

            if (tolower((unsigned char)text[i + j]) !=
                tolower((unsigned char)pattern[j]))
                break;
        }

        if (pattern[j] == '\0')
            return 1;
    }

    return 0;
}

void get_current_date(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm *today = localtime(&now);

    if (today == NULL)
    {
        snprintf(buffer, size, "01-01-1970");
        return;
    }

    strftime(buffer, size, "%d-%m-%Y", today);
}

void get_due_date(char *buffer, size_t size, int days)
{
    time_t now = time(NULL);
    struct tm date_value = *localtime(&now);

    date_value.tm_mday += days;
    mktime(&date_value);
    strftime(buffer, size, "%d-%m-%Y", &date_value);
}

int calculate_late_days(const char *due_date, const char *return_date)
{
    int dd, dm, dy, rd, rm, ry;
    struct tm due = {0}, returned = {0};
    time_t due_time, return_time;
    double seconds;

    if (sscanf(due_date, "%d-%d-%d", &dd, &dm, &dy) != 3 ||
        sscanf(return_date, "%d-%d-%d", &rd, &rm, &ry) != 3)
        return 0;

    due.tm_mday = dd;
    due.tm_mon = dm - 1;
    due.tm_year = dy - 1900;
    due.tm_hour = 12;

    returned.tm_mday = rd;
    returned.tm_mon = rm - 1;
    returned.tm_year = ry - 1900;
    returned.tm_hour = 12;

    due_time = mktime(&due);
    return_time = mktime(&returned);

    if (due_time == (time_t)-1 || return_time == (time_t)-1)
        return 0;

    seconds = difftime(return_time, due_time);
    if (seconds <= 0)
        return 0;

    return (int)(seconds / (60 * 60 * 24) + 0.5);
}
