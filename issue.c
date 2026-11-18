#include "header.h"

static void trim_newline(char *text)
{
    text[strcspn(text, "\r\n")] = '\0';
}

void read_issues(ISSUE **head)
{
    FILE *fp = fopen("Issued.txt", "r");
    ISSUE *new_issue, *last = NULL;
    char line[128];

    if (fp == NULL)
        return;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        new_issue = (ISSUE *)calloc(1, sizeof(ISSUE));
        if (new_issue == NULL)
            break;

        trim_newline(line);
        if (sscanf(line, "%d", &new_issue->issue_id) != 1)
        {
            free(new_issue);
            break;
        }

        if (fgets(line, sizeof(line), fp) == NULL ||
            sscanf(line, "%d", &new_issue->book_id) != 1 ||
            fgets(line, sizeof(line), fp) == NULL ||
            sscanf(line, "%d", &new_issue->user_id) != 1 ||
            fgets(new_issue->user_name, sizeof(new_issue->user_name), fp) == NULL ||
            fgets(new_issue->issue_date, sizeof(new_issue->issue_date), fp) == NULL ||
            fgets(new_issue->due_date, sizeof(new_issue->due_date), fp) == NULL ||
            fgets(new_issue->return_date, sizeof(new_issue->return_date), fp) == NULL ||
            fgets(line, sizeof(line), fp) == NULL ||
            sscanf(line, "%d", &new_issue->fine) != 1)
        {
            free(new_issue);
            break;
        }

        trim_newline(new_issue->user_name);
        trim_newline(new_issue->issue_date);
        trim_newline(new_issue->due_date);
        trim_newline(new_issue->return_date);
        new_issue->next = NULL;

        if (*head == NULL)
            *head = new_issue;
        else
            last->next = new_issue;
        last = new_issue;
    }

    fclose(fp);
}

void save_issues(ISSUE *head)
{
    FILE *fp = fopen("Issued.txt", "w");

    if (fp == NULL)
    {
        printf("Unable to open Issued.txt for saving.\n");
        return;
    }

    while (head != NULL)
    {
        fprintf(fp, "%d\n%d\n%d\n%s\n%s\n%s\n%s\n%d\n",
                head->issue_id, head->book_id, head->user_id,
                head->user_name, head->issue_date, head->due_date,
                head->return_date, head->fine);
        head = head->next;
    }

    fclose(fp);
}

void issue_book(LIB *books, ISSUE **issues)
{
    int book_id, user_id, next_id = 1;
    LIB *book;
    ISSUE *new_issue, *temp;
    char user_name[NAME_SIZE];

    book_id = read_int("Enter Book ID to issue: ");
    book = find_book_by_id(books, book_id);

    if (book == NULL)
    {
        printf("Book ID not found.\n");
        return;
    }

    if (book->quantity <= 0)
    {
        printf("This book is currently not available.\n");
        return;
    }

    user_id = read_int("Enter User ID: ");
    read_line("Enter User Name: ", user_name, sizeof(user_name));

    for (temp = *issues; temp != NULL; temp = temp->next)
        if (temp->issue_id >= next_id)
            next_id = temp->issue_id + 1;

    new_issue = (ISSUE *)calloc(1, sizeof(ISSUE));
    if (new_issue == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    new_issue->issue_id = next_id;
    new_issue->book_id = book_id;
    new_issue->user_id = user_id;
    snprintf(new_issue->user_name, sizeof(new_issue->user_name), "%s", user_name);
    get_current_date(new_issue->issue_date, sizeof(new_issue->issue_date));
    get_due_date(new_issue->due_date, sizeof(new_issue->due_date), 7);
    strcpy(new_issue->return_date, "Not Returned");
    new_issue->fine = 0;
    new_issue->next = NULL;

    if (*issues == NULL)
        *issues = new_issue;
    else
    {
        temp = *issues;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_issue;
    }

    book->quantity--;
    printf("Book issued successfully.\n");
    printf("Issue ID: %d\nIssue Date: %s\nDue Date: %s\n",
           new_issue->issue_id, new_issue->issue_date, new_issue->due_date);
}

void return_book(LIB *books, ISSUE *issues)
{
    int book_id, user_id, late_days;
    ISSUE *record;
    LIB *book;
    char today[DATE_SIZE];

    book_id = read_int("Enter Book ID: ");
    user_id = read_int("Enter User ID: ");

    record = issues;
    while (record != NULL)
    {
        if (record->book_id == book_id && record->user_id == user_id &&
            strcmp(record->return_date, "Not Returned") == 0)
            break;
        record = record->next;
    }

    if (record == NULL)
    {
        printf("Active issue record not found.\n");
        return;
    }

    get_current_date(today, sizeof(today));
    strcpy(record->return_date, today);
    late_days = calculate_late_days(record->due_date, today);
    record->fine = late_days * 5;

    book = find_book_by_id(books, book_id);
    if (book != NULL)
        book->quantity++;

    printf("Book returned successfully.\n");
    printf("Return Date: %s\nLate Days: %d\nFine Amount: Rs. %d\n",
           record->return_date, late_days, record->fine);
}

void display_issued_books(ISSUE *head)
{
    if (head == NULL)
    {
        printf("No issue records available.\n");
        return;
    }

    printf("\n%-8s %-8s %-8s %-20s %-12s %-12s %-15s %-8s\n",
           "IssueID", "BookID", "UserID", "User Name",
           "Issue Date", "Due Date", "Return Date", "Fine");
    printf("-----------------------------------------------------------------------------------------------\n");

    while (head != NULL)
    {
        printf("%-8d %-8d %-8d %-20s %-12s %-12s %-15s %-8d\n",
               head->issue_id, head->book_id, head->user_id,
               head->user_name, head->issue_date, head->due_date,
               head->return_date, head->fine);
        head = head->next;
    }
}

void free_issues(ISSUE *head)
{
    ISSUE *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
